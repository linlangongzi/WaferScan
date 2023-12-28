#include "ReportWriterQtExcel.h"
#include <QJsonObject>

#ifdef ARS_LIB_QTXLSX

#include "xlsxcellrange.h"
#include "xlsxdocument.h"
#include "xlsxworkbook.h"
#include <QDate>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QPixmap>
#include <QRegularExpression>

using namespace QXlsx;

using ReportRow = ReportWriter::ReportRow;
using ReportTable = ReportWriter::ReportTable;
using ReportData = ReportWriter::ReportData;

static const QRegularExpression tablePattern(R"(!\((.*?)\))");                   /*!< 表头 */
static const QRegularExpression keyPattern(R"(\$\((.*?)\))");

bool RangeContains(const CellRange &range, int row, int col)
{
    return row >= range.firstRow() && row <= range.lastRow() && col >= range.firstColumn() && col <= range.lastColumn();
}

/*! 获取指定位置的合并单元格，如果单元格未被合并，则返回该单元格 */
CellRange MergedCell(int row, int col, const QList<CellRange> &ranges)
{
    foreach (const CellRange &range, ranges) {
        if (RangeContains(range, row, col)) {
            return range;
        }
    }
    return CellRange(row, col, row, col);
}

/*! 在指定的行范围内，搜索能够完全被这些行包含的合并单元格 */
QList<CellRange> FindMergesInRowRange(int start, int end, const QList<CellRange> &sourceMerges)
{
    QList<CellRange> merges;
    foreach (const CellRange &merge, sourceMerges) {
        if (merge.firstRow() >= start && merge.lastRow() <= end) {
            merges << merge;
        }
    }
    return merges;
}

/*! 获取指定行中，所有单元格及合并单元格所占的最大矩形区域 */
CellRange MergedRow(Worksheet *sheet, int row)
{
    CellRange dimension = sheet->dimension();
    CellRange merged(row, dimension.firstColumn(), row, dimension.lastColumn());
    foreach (const CellRange &range, sheet->mergedCells()) {
        int first = range.firstRow();
        int last = range.lastRow();
        if (row >= first && row <= last) {
            merged.setFirstRow(qMin(merged.firstRow(), first));
            merged.setLastRow(qMax(merged.lastRow(), last));
        }
    }
    return merged;
}

/*! 获取一个区域的宽和高，单位为 points */
QSizeF CellSize(Worksheet *sheet, const CellRange &range)
{
    double height = 0;
    for (int row = range.firstRow(); row <= range.lastRow(); ++row) {
        height += sheet->rowHeight(row);
    }
    double width = 0;
    for (int col = range.firstColumn(); col <= range.lastColumn(); ++col) {
        width += sheet->columnWidth(col) * sheet->baseColumnWidth() * 3 / 4;
    }
    return QSizeF(width, height);
}

/*! 搜索表头关键字 */
QString FindTable(Worksheet *sheet, const CellRange &range)
{
    for (int row = range.firstRow(); row <= range.lastRow(); ++row) {
        for (int col = range.firstColumn(); col <= range.lastColumn(); ++col) {
            Cell *cell = sheet->cellAt(row, col);
            if (cell == nullptr) {
                continue;
            }
            QString text = cell->value().toString();
            QRegularExpressionMatch match = tablePattern.match(text);
            if (match.hasMatch()) {
                text.replace(tablePattern, QString());
                sheet->writeInlineString(row, col, text);
//                qDebug() << "QtExcel: found table" << match.captured(1) << "at" << CellReference(row, col).toString() << range.toString();
                return match.captured(1);
            }
        }
    }
    return QString();
}

/*! 在指定区域放置图片，并调整该区域的行高 */
void PlaceImage(Worksheet *dest, const QImage &image, const CellRange &range)
{
    if (image.isNull()) {
        return;
    }
    QSizeF cellSize = CellSize(dest, range);
    qreal width = double(image.width()) / image.dotsPerMeterX();
    qreal height = double(image.height()) / image.dotsPerMeterY();
    double imageHeight = cellSize.width() / width * height;
    QSizeF imageSize(cellSize.width(), imageHeight);

//    qDebug() << "QtExcel: inserting image" << image << "to" << range.toString() << "cell size" << cellSize << "imageSize" << imageSize;
    dest->insertImage(range.firstRow() - 1, range.firstColumn() - 1, image, QSize(imageSize.width() * 4 / 3, imageSize.height() * 4 / 3));

    qreal totalHeight = qMax(cellSize.height(), imageHeight) + 1;
//    qDebug() << "QtExcel: setting row height for" << range.toString() << "to" << totalHeight;
    for (int row = range.firstRow(); row <= range.lastRow(); ++row) {
        int height = qRound(totalHeight / (range.lastRow() - row + 1));
        totalHeight -= height;
        dest->setRowHeight(row, row, height);
    }
}

/*! 以一个区域为模板，在目标位置展开为多行，并使用数据表替换模板字段 */
void CopyRow(Worksheet *source, const CellRange &sourceRange, Worksheet *dest, int destOffset, const ReportTable &table)
{
    qDebug() << "QtExcel: copy" << sourceRange.toString() << "to" << destOffset;
    QList<CellRange> mergesInRow = FindMergesInRowRange(sourceRange.firstRow(), sourceRange.lastRow(), source->mergedCells());

    for (int expandIndex = 0; expandIndex < table.count(); ++expandIndex) {
        const QVariantMap &dataRow = table[expandIndex];
//        qDebug() << "QtExcel: Processing" << sourceRange.toString() << "with data" << dataRow;
        const QStringList keys = dataRow.keys();

        for (int row = sourceRange.firstRow(); row <= sourceRange.lastRow(); ++row) {
            int destRow = row + destOffset;
            dest->setRowHeight(destRow, destRow, qMax(dest->rowHeight(destRow), source->rowHeight(row)));
            dest->setRowFormat(destRow, destRow, source->rowFormat(row));
            dest->setRowHidden(destRow, destRow, source->isRowHidden(row));
            for (int col = sourceRange.firstColumn(); col <= sourceRange.lastColumn(); ++col) {
                Cell *cell = source->cellAt(row, col);
                Format format = cell ? cell->format() : Format();
                format.setLocked(false);                                        /* 使此格式变为有效格式。dest->write时传入无效格式，将导致此单元格维持原格式 */

                QVariant value = source->read(row, col);
                if (value.type() == QVariant::String) {
                    QString s = value.toString();
                    QRegularExpressionMatchIterator i = keyPattern.globalMatch(s);
                    while (i.hasNext()) {
                        QRegularExpressionMatch match = i.next();
                        const QString key = match.captured(1);
                        if (!keys.contains(key)) {
                            continue;
                        }
                        QVariant v = dataRow[key];
                        if (v.canConvert<QString>()) {
                            s.replace(match.captured(0), v.toString());
                            continue;
                        }
                        if (v.type() == QVariant::Image) {
                            QImage image = v.value<QImage>();
                            s.remove(match.captured(0));
                            CellRange range = MergedCell(row, col, mergesInRow).translated(destOffset, 0);
                            PlaceImage(dest, image, range);
                        }
                    }
                    value = s;
                    bool isNumber = false;
                    double number = s.toDouble(&isNumber);
                    if (isNumber) {
                        value = number;
                    }
                }
//                qDebug() << "QtExcel: Writing" << CellReference(destRow, col).toString() << value;
                dest->write(destRow, col, value, format);
            }
        }

        /* 行内的合并单元格 */
        foreach (const CellRange &range, mergesInRow) {
            dest->mergeCells(range.translated(destOffset, 0));
        }

        destOffset += sourceRange.rowCount();
    }
}

/*! 以输入表为模板，将数据表展开并写入到输出表中 */
void ExpandTables(Worksheet *inputSheet, Worksheet *outputSheet, const ReportData &data)
{
    CellRange dimension = inputSheet->dimension();
    /* 复制列宽 */
    for (int col = dimension.firstColumn(); col <= dimension.lastColumn(); ++col) {
        outputSheet->setColumnWidth(col, col, inputSheet->columnWidth(col));
    }
//    qDebug() << "QtExcel: expanding thru" << dimension.toString();
    int rowInserted = 0;
    QString prevContext;
    int row = dimension.firstRow();
    int firstRow = row;
    while (row <= dimension.lastRow()) {
        const CellRange source = MergedRow(inputSheet, row);
        const QString context = FindTable(inputSheet, source);
//        qDebug() << "QtExcel: at row" << row << source.toString() << "got table" << context;
        if (prevContext != context) {
//            qDebug() << "QtExcel: Context swithing from" << prevContext << "to" << context;
            ReportTable table = data[prevContext];
            if (table.count() > 0) {
                CopyRow(inputSheet, CellRange(firstRow, dimension.firstColumn(), row - 1, dimension.lastColumn()), outputSheet, rowInserted, table);
            }
            prevContext = context;
            firstRow = row;
            rowInserted += table.count() - 1;
        }
        row += source.rowCount();
    }
//    qDebug() << "QtExcel: finished scan, first" << firstRow << "now" << row;
    ReportTable table = data[prevContext];
    if (row >= firstRow && table.count() > 0) {
        CopyRow(inputSheet, CellRange(firstRow, dimension.firstColumn(), row - 1, dimension.lastColumn()), outputSheet, rowInserted, table);
    }
    /* 移除模板之后的内容 */
    int nextRow = dimension.lastRow() + rowInserted + 1;
    for (int row = nextRow; row <= dimension.lastRow(); ++row) {
        for (int col = dimension.firstColumn(); col <= dimension.lastColumn(); ++col) {
            outputSheet->writeBlank(row, col);
        }
    }
    foreach (const CellRange &range, FindMergesInRowRange(nextRow, dimension.lastRow(), outputSheet->mergedCells())) {
        outputSheet->unmergeCells(range);
    }
}

#endif

bool ReportWriterQtExcel::Generate(const QString &templatePath, const QString &outputPath, const ReportWriter::ReportData &report)
{
#ifdef ARS_LIB_QTXLSX
    Document inputDoc(templatePath);
//    Document outputDoc;
    foreach (const QString &sheetName, inputDoc.sheetNames()) {
        qDebug() << "QtExcel: processing sheet" << sheetName;
        if (!inputDoc.selectSheet(sheetName)) {
            qDebug() << "QtExcel: skipping sheet" << sheetName;
            continue;
        }
        Worksheet *inputSheet = inputDoc.currentWorksheet();
        if (!inputSheet) {
            continue;
        }
        const QString swapSheetName = "__" + sheetName;
        inputDoc.renameSheet(sheetName, swapSheetName);
        inputDoc.workbook()->copySheet(inputDoc.sheetNames().indexOf(swapSheetName), sheetName);
        inputDoc.selectSheet(sheetName);
        Worksheet *outputSheet = inputDoc.currentWorksheet();
        foreach (const CellRange &range, outputSheet->mergedCells()) {
            outputSheet->unmergeCells(range);
        }

        ExpandTables(inputSheet, outputSheet, report);
        inputDoc.deleteSheet(swapSheetName);
    }
    if (!inputDoc.saveAs(outputPath)) {
        return SetLastError(tr("写入报表文件失败"));
    }
    return SetLastError();
#else
    Q_UNUSED(templatePath);
    Q_UNUSED(outputPath);
    Q_UNUSED(report);
    return SetLastError(tr("不支持Excel报表"));
#endif
}

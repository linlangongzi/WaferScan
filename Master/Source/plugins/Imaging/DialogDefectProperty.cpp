#include "DialogDefectProperty.h"
#include "ui_DialogDefectProperty.h"
#include <QAction>

DialogDefectProperty::DialogDefectProperty(DefectItem *pItem, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDefectProperty),
    m_pDefectItem(pItem),
    m_nRow(0)
{
    ui->setupUi(this);

    InitControls();
    SetControlValues();
}

DialogDefectProperty::~DialogDefectProperty()
{
    ResetMapData();
    delete ui;
}

void DialogDefectProperty::ResetMapData()
{
    qDeleteAll(m_mapValues);
    m_mapValues.clear();
}

void DialogDefectProperty::InitControls()
{
    setWindowTitle("Comments");

    QStringList headers;
    headers << tr("属性") << tr("值") << tr("单位");

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setColumnWidth(0, 150);
    ui->tableWidget->setColumnWidth(1, 300);
    ui->tableWidget->setColumnWidth(2, 50);

    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    QAction *remove = new QAction(tr("删除图像"), this);
    connect(remove, SIGNAL(triggered()), this, SLOT(RemoveImage()));
    ui->treeWidget->addAction(remove);
}

void DialogDefectProperty::SetControlValues()
{
    QString str;

//    AddItem("Name", tr("Name"), m_pDefectItem->GetDefect().GetInfo()["Name"].toString(),        tr(""), true);
    AddItem( "Area", tr("面积"), str.setNum( m_pDefectItem->defect().GetArea() ),               tr("mm²") );
//    AddItem( "OuterArea", tr("OuterArea"), str.setNum( m_pDefectItem->GetDefect().GetOuterArea() ),     tr("mm²") );
//    AddItem( "InnerArea", tr("InnerArea"), str.setNum( m_pDefectItem->GetDefect().GetInnerArea() ),     tr("mm²") );
    AddItem( "Length", tr("长度"), str.setNum( m_pDefectItem->defect().GetLength() ),           tr("mm") );
    AddItem( "Width", tr("宽度"), str.setNum( m_pDefectItem->defect().GetWidth() ),             tr("mm") );
    AddItem( "CharacteristicSize", tr("特征长度"), str.setNum( m_pDefectItem->defect().GetCharacteristicSize() ), tr("mm") );
    AddItem("PixelCount", tr("像素数"), str.setNum(m_pDefectItem->defect().pixelCount), tr("个"));
    QVector2D center = m_pDefectItem->defect().GetCenter();
    AddItem( "Center", tr("中心点"), QString( tr("(%1, %2)") ).arg( center.x() ).arg( center.y() ),          tr("mm") );
    AddItem("Comment", tr("注释"), m_pDefectItem->defect().comment,  tr(""), true);

    int index = 1;
    foreach (const QImage &image, m_pDefectItem->defect().screenshots()) {
        auto *item = new QTreeWidgetItem(ui->treeWidget);
        item->setData(0, Qt::DisplayRole, index);
        item->setData(1, Qt::DecorationRole, image);
        index++;
    }
    ui->treeWidget->resizeColumnToContents(0);
    ui->treeWidget->resizeColumnToContents(1);
}

void DialogDefectProperty::AddItem(const QString &name, const QString &displayName, const QString &value, const QString &unit, bool bEditable /* = false*/)
{
    STVALUE *pstValue = new STVALUE(value, unit, bEditable);
    m_mapValues.insert(name, pstValue);
    ui->tableWidget->insertRow(m_nRow);

    auto *itemKey = new QTableWidgetItem(displayName);
    itemKey->setData(Qt::UserRole, name);
    itemKey->setFlags(itemKey->flags() & ~Qt::ItemIsEditable);
//    itemKey->setBackgroundColor( QColor(0, 200, 200) );
    ui->tableWidget->setItem(m_nRow, 0, itemKey);

    auto *itemValue = new QTableWidgetItem(value);
    if (bEditable) {
        itemValue->setFlags(itemValue->flags()|Qt::ItemIsEditable);
    } else {
        itemValue->setFlags(itemValue->flags()&~Qt::ItemIsEditable);
//        itemValue->setBackgroundColor( QColor(230, 230, 230) );
    }
    ui->tableWidget->setItem(m_nRow, 1, itemValue);

    auto *itemUnit = new QTableWidgetItem(unit);
    itemUnit->setFlags(itemValue->flags()&~Qt::ItemIsEditable);
//    itemUnit->setBackgroundColor( QColor(230, 230, 230) );
    ui->tableWidget->setItem(m_nRow, 2, itemUnit);

    ui->tableWidget->setRowHeight(m_nRow, 25);

    m_nRow++;
}

void DialogDefectProperty::on_pushButtonSave_clicked()
{
    int nRows = ui->tableWidget->rowCount();
    for (int i = 0; i < nRows; ++i)
    {
        QTableWidgetItem *itemKey = ui->tableWidget->item(i, 0);
        QString strKey = itemKey->data(Qt::UserRole).toString();

        STVALUE *pstValue = m_mapValues.value(strKey);
        if (nullptr != pstValue)
        {
            if (pstValue->bEditable)
            {
                QTableWidgetItem *itemValue = ui->tableWidget->item(i, 1);
                pstValue->strValue = itemValue->text();
            }
        }
    }

    GenerateDefectValues();

    accept();
}

void DialogDefectProperty::GenerateDefectValues()
{
    UTDefect defect = m_pDefectItem->defect();

    STVALUE *pstName = m_mapValues.value("Name");
    if (nullptr != pstName) {
//        defect.SetInfo("Name", pstName->strValue);
    }

    STVALUE *pstValue = m_mapValues.value("Comment");
    if (nullptr != pstValue) {
        defect.comment = pstValue->strValue;
    }

    QList<QImage> images;
    QTreeWidgetItem *root = ui->treeWidget->invisibleRootItem();
    for (int index = 0; index < root->childCount(); ++index) {
        images << root->child(index)->data(1, Qt::DecorationRole).value<QImage>();
    }
    defect.setScreenshots(images);

    m_pDefectItem->setDefect(defect);
}

void DialogDefectProperty::on_pushButtonCancel_clicked()
{
    reject();
}

void DialogDefectProperty::RemoveImage()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if (!item) {
        return;
    }
    ui->treeWidget->invisibleRootItem()->removeChild(item);
    delete item;
}

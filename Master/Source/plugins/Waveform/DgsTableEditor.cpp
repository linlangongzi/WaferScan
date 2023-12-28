#include "DgsTableEditor.h"
#include "ui_DgsTableEditor.h"
#include "MaterialComboBox.h"
#include <QChart>
#include <QDateEdit>
#include <QInputDialog>
#include <QLoggingCategory>
#include <QMessageBox>
#include <QSplineSeries>
#include <QtMath>
#include <Ultrasonic/UltrasonicManager.h>
#include <allrising/ThrottleUpdate.h>

using namespace QtCharts;
static Q_LOGGING_CATEGORY(debug, "wave.dgsEditor");

DgsTableEditor::DgsTableEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DgsTableEditor),
    m_handler(this, &DgsTableEditor::dgsTable, &DgsTableEditor::setDgsTable)
{
    ui->setupUi(this);

    m_model = new DgsCurveModel(this);
    m_sortModel = new QIdentityProxyModel(this);
    m_sortModel->setSourceModel(m_model);
    ui->treeView->setModel(m_sortModel);

    QChart *chart = ui->chartView->chart();
    chart->legend()->hide();
    m_depthAxis = new QLogValueAxis(chart);
    m_depthAxis->setBase(10);
    m_gainAxis = new QValueAxis(chart);
    m_depthAxis->setTitleText(tr("深度 mm"));
    m_gainAxis->setTitleText(tr("增益 dB"));
    chart->addAxis(m_depthAxis, Qt::AlignBottom);
    chart->addAxis(m_gainAxis, Qt::AlignLeft);

    ui->treeView->addAction(ui->actionAddCurveItem);
    ui->treeView->addAction(ui->actionRemoveCurveItem);
    ui->treeView->addAction(ui->actionAddPointItem);
    ui->treeView->addAction(ui->actionRemovePointItem);
    ui->treeView->addAction(ui->actionCleanAll);

    m_handler.addHandler(ui->material, &MaterialComboBox::setCurrentUuid, &MaterialComboBox::currentUuidChanged, &DgsTable::materialUuid);
    m_handler.add(ui->waterPathLength, &DgsTable::waterPathLength);
    m_handler.addHandler(ui->date, &QDateEdit::setDate, &QDateEdit::dateChanged, &DgsTable::date);
    m_handler.addHandler(m_model, &DgsCurveModel::setDgsCurves, &DgsCurveModel::dgsCurvesChanged, &DgsTable::dgsCurves);
}

DgsTableEditor::~DgsTableEditor()
{
    delete ui;
}

DgsTable DgsTableEditor::dgsTable() const
{
    return m_dgsTable;
}

void DgsTableEditor::setDgsTable(const DgsTable &dgsTable)
{
    if (dgsTable == m_dgsTable) {
        return;
    }
    m_dgsTable = dgsTable;

    m_handler.setControls(m_dgsTable);

    emit dgsTableChanged(m_dgsTable);

    ThrottleUpdate::call(this, &DgsTableEditor::updateChart);
}

void DgsTableEditor::on_recoverDate_clicked()
{
    ui->date->setDate(QDate::currentDate());
}

void DgsTableEditor::on_deleteDate_clicked()
{
    ui->date->setDate(QDate(2000, 1, 1));
}

void DgsTableEditor::on_actionAddCurveItem_triggered()
{
    bool ok = false;
    DgsCurve dgsCurve;
    dgsCurve.diameter = QInputDialog::getDouble(this, QString(), tr("请输入平底孔当量"), 0.3, 0.3, std::numeric_limits<qreal>::max(), 1, &ok);
    if (!ok) {
        return;
    }

    auto *curveItem = new DgsCurveItem;
    curveItem->setDgsCurve(dgsCurve);
    m_model->rootItem()->appendChild(curveItem);
    m_model->notifyDgsCurvesChanged();

    QModelIndex index = m_sortModel->mapFromSource(curveItem->index());
    ui->treeView->setCurrentIndex(index);
}

void DgsTableEditor::on_actionAddPointItem_triggered()
{
    if (!currentCurveItem()) {
        return;
    }
    bool ok = false;
    qreal depth = QInputDialog::getDouble(this, QString(), tr("请输入缺陷深度"), 0.1, 0.1, std::numeric_limits<qreal>::max(), 1, &ok);
    if (!ok) {
        return;
    }
    ok = false;
    qreal gain = QInputDialog::getDouble(this, QString(), tr("请输入增益"), 0, std::numeric_limits<std::ptrdiff_t>::min(), std::numeric_limits<qreal>::max(), 1, &ok);
    if (!ok) {
        return;
    }
    auto *pointItem = new DgsCurvePointItem;
    pointItem->depth = depth;
    pointItem->gain = gain;
    currentCurveItem()->appendChild(pointItem);
    m_model->notifyDgsCurvesChanged();
}

void DgsTableEditor::on_actionRemoveCurveItem_triggered()
{
    if (!currentCurveItem()) {
        return;
    }
    if (QMessageBox::warning(this, QString(), tr("确定删除当前选中的曲线吗"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        m_model->destroyItem(currentCurveItem());
        m_model->notifyDgsCurvesChanged();
    }
}

void DgsTableEditor::on_actionRemovePointItem_triggered()
{
    if (!currentCurvePointItem()) {
        return;
    }
    if (QMessageBox::warning(this, QString(), tr("确定删除当前选中的点吗"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        m_model->destroyItem(currentCurvePointItem());
        m_model->notifyDgsCurvesChanged();
    }
}

void DgsTableEditor::on_actionCleanAll_triggered()
{
    if (QMessageBox::warning(this, QString(), tr("确定删除所有曲线吗"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        m_model->rootItem()->removeChildren();
        m_model->notifyDgsCurvesChanged();
    }
}

DgsCurveItem *DgsTableEditor::currentCurveItem() const
{
    QModelIndex index = m_sortModel->mapToSource(ui->treeView->currentIndex());
    if (!index.isValid()) {
        return nullptr;
    }
    auto *curveItem = m_model->itemForIndexAtLevel<1>(index);
    if (curveItem) {
        return curveItem;
    } else if (currentCurvePointItem()) {
        return currentCurvePointItem()->parent();
    } else {
        return nullptr;
    }
}

DgsCurvePointItem *DgsTableEditor::currentCurvePointItem() const
{
    QModelIndex index = m_sortModel->mapToSource(ui->treeView->currentIndex());
    if (index.isValid()) {
        return m_model->itemForIndexAtLevel<2>(index);
    }
    return nullptr;
}

void DgsTableEditor::updateChart()
{
    QChart *chart = ui->chartView->chart();
    chart->removeAllSeries();
    qreal minDepth = std::numeric_limits<qreal>::max();
    qreal maxDepth = std::numeric_limits<qreal>::min();
    qreal minGain = std::numeric_limits<qreal>::max();
    qreal maxGain = std::numeric_limits<qreal>::min();
    if (m_model->dgsCurves().isEmpty()) {
        m_depthAxis->setRange(0.1, 100);
        m_gainAxis->setRange(-10, 0);
        return;
    }
    for (const auto &dgsCurve : m_model->dgsCurves()) {
        auto *series = new QSplineSeries(chart);
        series->setPointsVisible(true);
        const auto &table = dgsCurve.interpolateTable;
        for (auto iter = table.constBegin(); iter != table.constEnd(); ++iter) {
            series->append(iter.key(), iter.value());
            minDepth = qMin(minDepth, iter.key());
            maxDepth = qMax(maxDepth, iter.key());
            minGain = qMin(minGain, iter.value());
            maxGain = qMax(maxGain, iter.value());
        }
        chart->addSeries(series);
        series->attachAxis(m_depthAxis);
        series->attachAxis(m_gainAxis);
    }
    qCDebug(debug) << "update dgsCurve, depth axis range is " << minDepth << " - " << maxDepth << ", gain axis range is " << minGain << " - " << maxGain;
    m_depthAxis->setRange(minDepth, maxDepth);
    m_gainAxis->setRange(minGain, maxGain);
}

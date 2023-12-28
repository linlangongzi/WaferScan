#include "PipePanelBScanMain.h"
#include "ui_PipePanelBScanMain.h"
#include <Plot/PlotViewEditor.h>
#include <QInputDialog>

PipePanelBScanMain::PipePanelBScanMain(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PipePanelBScanMain)
{
    ui->setupUi(this);
}

PipePanelBScanMain::~PipePanelBScanMain()
{
    delete ui;
}

QList<PlotView> PipePanelBScanMain::viewOptions() const
{
    return m_viewOptions;
}

void PipePanelBScanMain::recalculateAlarms()
{
    QList<PlotAlarmItem> alarms;
    for (auto *view: m_views) {
        alarms << view->alarmsTriggered();
    }

    emit alarmsUpdated(alarms);
}

qreal PipePanelBScanMain::getPositionMax() const
{
    qreal posMax = 0;
    for (auto bscan : m_views) {
        qreal newMax = bscan->getPositionMax();
        posMax = newMax > posMax ? newMax : posMax;
    }

    return posMax;
}

void PipePanelBScanMain::SetOrderInfo(const OrderInfo &info)
{
    Q_UNUSED(info);
}

void PipePanelBScanMain::OnSegmentUpdate(const UTDataSegment &segment)
{
    bool alarmChanged = false;
    for (auto *view: m_views) {
        alarmChanged |= view->appendData(segment);
    }

    if (alarmChanged) {
        recalculateAlarms();
    }
}

void PipePanelBScanMain::setViewOptions(const QList<PlotView> &views)
{
    if (views == m_viewOptions) {
        return;
    }

    qDeleteAll(m_views);
    m_views.clear();
    m_viewOptions.clear();

    for (const auto &view: views) {
        createView(view);
    }

    emit viewOptionsChanged(views);
}

void PipePanelBScanMain::addViewOption(const PlotView &option)
{
    createView(option);
    emit viewOptionsChanged(m_viewOptions);
}

void PipePanelBScanMain::removeView(int index)
{
    if (uint(index) >= uint(m_views.count())) {
        return;
    }
    m_viewOptions.removeAt(index);
    delete m_views.takeAt(index);

    emit viewOptionsChanged(m_viewOptions);
}

void PipePanelBScanMain::resetData()
{
    for (auto *view: m_views) {
        view->resetData();
    }
    emit alarmsUpdated(QList<PlotAlarmItem>());
}

void PipePanelBScanMain::createView(const PlotView &option)
{
    auto *view = new PipePanelBScan(ui->splitter);
    ui->splitter->addWidget(view);
    view->setViewOptions(option);

    connect(view, &PipePanelBScan::viewOptionsChanged, this, [=](const PlotView &o) {
        int index = m_views.indexOf(view);
        m_viewOptions[index] = o;
        emit viewOptionsChanged(m_viewOptions);
    });

    m_views << view;
    m_viewOptions << option;
}

void PipePanelBScanMain::on_addView_clicked()
{
    PlotViewEditor dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        addViewOption(dialog.option());
    }
}

void PipePanelBScanMain::on_removeView_clicked()
{
    QStringList names;
    for (auto *view: m_views) {
        names << tr("%1: %2").arg(names.count() + 1).arg(view->viewOptions().name);
    }
    bool ok;
    const QString name = QInputDialog::getItem(this, QString(), tr("删除B扫描视图"), names, 0, false, &ok);
    if (!ok) {
        return;
    }
    removeView(names.indexOf(name));
}

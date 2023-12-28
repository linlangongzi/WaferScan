#include "PipePanelBScan.h"
#include <Plot/PlotViewEditor.h>
#include "ui_PipePanelBScan.h"
#include "GraphicsLineSeries.h"
#include <Pipe/PipeManager.h>
#include <QCheckBox>
#include <QGridLayout>
#include <allrising/Util.h>

PipePanelBScan::PipePanelBScan(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PipePanelBScan)
{
    ui->setupUi(this);

    ui->view->axis(Qt::BottomEdge)->setResizePolicy(GraphicsAxis::ResizeKeepSelection);
    ui->view->axis(Qt::LeftEdge)->setResizePolicy(GraphicsAxis::ResizeKeepSelection);

    cursorManager = new CursorManager(ui->view->rootItem(), this);
//    cursorManager->AddToScene(ui->view->graphicsScene());

    root = new QGraphicsItemGroup;
    root->setTransform(QTransform::fromScale(1, -1));
    root->setHandlesChildEvents(false);
    ui->view->view()->scene()->addItem(root);
    connect(ui->view, &AbstractGraphicsView::mouseMoved, this, &PipePanelBScan::OnMouseMoved);

    deadZoneLeft = new QGraphicsRectItem(root);
    deadZoneLeft->setBrush(Qt::lightGray);
    deadZoneLeft->setZValue(-1);
    deadZoneLeft->setPen(Qt::NoPen);
    deadZoneRight = new QGraphicsRectItem(root);
    deadZoneRight->setBrush(Qt::lightGray);
    deadZoneRight->setZValue(-1);
    deadZoneRight->setPen(Qt::NoPen);
}

PipePanelBScan::~PipePanelBScan()
{
    qDeleteAll(curves);
    curves.clear();
    delete ui;
}

PlotView PipePanelBScan::viewOptions() const
{
    return m_viewOptions;
}

qreal PipePanelBScan::getPositionMax() const
{
    return positionMax;
}

bool PipePanelBScan::appendData(const UTDataSegment &segment)
{
    bool alarmChanged = false;
    const qreal left = m_viewOptions.range.left();
    const qreal right = m_viewOptions.range.right();

    for (const auto &frame: segment) {
        const int channelId = frame.Context.GetChannelID();
        const int curveCount = m_viewOptions.curves.count();

        for (int curveIndex = 0; curveIndex < curveCount; ++curveIndex) {
            const auto &curve = m_viewOptions.curves[curveIndex];

            if (!curve.enable || !curve.channels.hasAxis(channelId)) {
                continue;
            }

            const qreal pos = frame.GetPose(curve.axis) + curve.offset.value(channelId);
            positionMax = pos > positionMax ? pos : positionMax;
            if (pos < left || pos > right) {
                continue;
            }

            const int position = qRound((pos - left) / curve.interval);
            const qreal feature = frame.FeatureValue(curve.featureIndex);
            const qreal featureBase = frame.FeatureValue(curve.featureBaseIndex);
            const qreal featureResult = (feature - featureBase) * curve.scale * (curve.comment.contains("correct") ? Angle2Cos(curve.angleCorrect.value(channelId)) : 1);
            alarmChanged |= curves[curveIndex]->add(pos, position, featureResult, channelId, m_viewOptions.isDeadZone(pos));
        }
    }

    return alarmChanged;
}

void PipePanelBScan::updateView()
{
    QRectF range = m_viewOptions.range;

    ui->name->setText(m_viewOptions.name);

    /* 刷新场景范围及单位 */
    ui->view->setRange(Qt::Horizontal, Range(range, Qt::Horizontal));
    ui->view->setRulerSelection(Qt::Horizontal, Range(range, Qt::Horizontal));

    ui->view->setRange(Qt::Vertical, Range(range, Qt::Vertical));
    ui->view->setRulerSelection(Qt::Vertical, Range(range, Qt::Vertical));

    ui->view->setRulerUnitName(Qt::BottomEdge, m_viewOptions.unitNameH);
    ui->view->setRulerUnitName(Qt::LeftEdge, m_viewOptions.unitNameV);
    root->setTransform(QTransform::fromScale(1, -1) * QTransform::fromTranslate(range.left(), 0));

    /* 刷新标记线 */
    resizeContainer(m_viewOptions.lines.size(), lines, [=]() {
        return new QGraphicsLineItem(root);
    }, [](QGraphicsLineItem *line) {
        delete line;
    });
    for (int index = 0; index < lines.count(); ++index) {
        const auto &option = m_viewOptions.lines.at(index);
        if (option.orientation == Qt::Horizontal) {
            lines[index]->setLine(range.left(), option.value, range.right(), option.value);
        } else {
            lines[index]->setLine(option.value, range.top(), option.value, range.bottom());
        }
        lines[index]->setPen(QPen(option.color, 0));
    }

    resizeContainer(m_viewOptions.curves.size(), curves, [=]() {
        auto *series = new GraphicsLineSeries(this);
        series->setItemParents(root);
        connect(series, &GraphicsLineSeries::FoundFeature, PipeManager::instance(), &PipeManager::OnFoundFeature);
        ui->legendLayout->insertWidget(ui->legendLayout->count() - 1, series);  /* 将Legend插在spacer之前 */
        return series;
    }, [](GraphicsLineSeries *series) {
        delete series;
    });

    for (int index = 0; index < curves.count(); ++index) {
        auto curve = curves[index];
        auto &option = m_viewOptions.curves.at(index);
        curve->setOption(option);
        curve->setSize(qCeil(range.width() / option.interval));
    }

    /* 刷新盲区色块 */
    range.setWidth(m_viewOptions.deadZoneLeft);
    deadZoneLeft->setRect(range);

    range.setWidth(m_viewOptions.deadZoneRight);
    range.moveRight(m_viewOptions.range.right());
    deadZoneRight->setRect(range);
}

/*!
 * \brief PipePanelBScan::CosBaseOnAngle
 * \param angle
 * \return 输入角度,返回余弦值
 */
qreal PipePanelBScan::Angle2Cos(const qreal &angle)
{
    static const qreal piRadian = M_PI / 180.0;
    return cos(angle * piRadian);
}

void PipePanelBScan::setViewOptions(PlotView viewOptions)
{
    if (m_viewOptions == viewOptions) {
        return;
    }
    m_viewOptions = viewOptions;

    updateView();

    emit viewOptionsChanged(m_viewOptions);
}

void PipePanelBScan::resetData()
{
    positionMax = 0;
    QList<PlotLine> alarmsAvailable;

    for (const auto &line: m_viewOptions.lines) {
        if (line.alarmMode != PlotLine::AlarmNone) {
            alarmsAvailable << line;
        }
    }

    for (auto curve: curves) {
        curve->reset();
        curve->setAlarms(alarmsAvailable);
    }
}

QList<PlotAlarmItem> PipePanelBScan::alarmsTriggered() const
{
    QList<PlotAlarmItem> alarms;
    for (auto curve: curves) {
        for (const auto &alarm: curve->alarms) {
            if (!alarm.alarm.channel.isEmpty()) {
                alarms << alarm.alarm;
            }
        }
    }
    return alarms;
}

void PipePanelBScan::OnMouseMoved(const QPointF &scenePos)
{
    QPointF waveformPos = scenePos;
    cursorManager->SetCursorPos(waveformPos);
    ui->view->axis(Qt::BottomEdge)->setCursorPos(scenePos.x());
    ui->view->axis(Qt::LeftEdge)->setCursorPos(scenePos.y());
}

void PipePanelBScan::on_optionsDlg_clicked()
{
    PlotViewEditor dialog;
    dialog.setOption(m_viewOptions);
    if (dialog.exec() == QDialog::Accepted) {
        setViewOptions(dialog.option());
    }
}

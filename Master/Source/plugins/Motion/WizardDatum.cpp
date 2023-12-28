#include "WizardDatum.h"
#include "ui_WizardDatum.h"
#include <Ultrasonic/UTInstrument.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <Motion/Axis.h>
#include <Motion/EndPoint.h>
#include <Motion/MotionManager.h>
#include <Motion/TrioDriver.h>
#include <Motion/TrioQueue.h>
#include <QCheckBox>
#include <QLoggingCategory>
#include <QVector>
#include <algorithm>
//#include <Ultrasonic/UltrasonicManager.h>

static Q_LOGGING_CATEGORY(debug, "motion.wizardDatum");

using namespace Motion;

WizardDatum::WizardDatum(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::WizardDatum)
{
    ui->setupUi(this);
    ui->single->setVisible(false);
    ui->axis->setVisible(false);

    for (Axis *axis: MotionManager::axisList()) {
        QCheckBox *box = new QCheckBox(axis->name(), ui->axisList);
        box->setProperty("AxisID", QVariant::fromValue(axis));
        if (axis->config().datumInput < 0) {
            box->setEnabled(false);
            box->setText(box->text() + tr(" (未配置复位传感器)"));
        } else {
            box->setChecked(true);
            ui->axis->addItem(axis->name(), QVariant::fromValue(axis));
        }
        ui->axisList->layout()->addWidget(box);
    }

//    ui->axisList->setEnabled(false);

    connect(this, &QWizard::currentIdChanged, this, &WizardDatum::onPageChanged);
    connect(MotionManager::driver()->queue(), &TrioQueue::StateUpdated, this, &WizardDatum::onQueueStateUpdated);
}

WizardDatum::~WizardDatum()
{
    delete ui;
}

bool AxisHomeSequenceSorter(Axis *axis1, Axis *axis2)
{
    if (axis1 && axis2) {
        qCDebug(debug) << "axis1.name =" << axis1->name() << "axis2.name =" << axis2->name();
        qCDebug(debug) << "axis1.datumSequence =" << axis1->config().datumSequence << "axis2.datumSequence =" << axis2->config().datumSequence;
        return axis1->config().datumSequence < axis2->config().datumSequence;
    }
    return axis1 > axis2;
}

QList<Axis *> WizardDatum::selectedAxis() const
{
    QList<Axis *> axisList;
    if (ui->single->isChecked()) {
        axisList << ui->axis->currentData().value<Axis *>();
    }
    if (ui->multiple->isChecked()) {
        for (auto *child: ui->axisList->children()) {
            auto *box = qobject_cast<QCheckBox *>(child);
            if (box && box->isChecked()) {
                axisList << box->property("AxisID").value<Axis *>();
            }
        }
    }
    qSort(axisList.begin(), axisList.end(), AxisHomeSequenceSorter);
    return axisList;
}

bool WizardDatum::validateCurrentPage()
{
    if (currentPage() == ui->pageAxisList) {
        return !selectedAxis().isEmpty();
    } else if (currentPage() == ui->pageDatum) {
        TrioQueue::State state = MotionManager::driver()->queue()->GetState();
        return state == TrioQueue::STATE_IDLE || state == TrioQueue::STATE_UNAVAILABLE;
    }
    return true;
}

void WizardDatum::done(int result)
{
    if (currentPage() == ui->pageDatum && result == QDialog::Rejected) {
        setStatus(tr("正在取消复位"));
        MotionManager::driver()->queue()->Stop();
        MotionManager::driver()->execute("RapidStop(2)");
    }
    return QWizard::done(result);
}

void WizardDatum::setStatus(const QString &text)
{
    if (ui->pageDatum->subTitle() == text) {
        return;
    }
    ui->history->appendPlainText(text);
    ui->pageDatum->setSubTitle(text);
}

void WizardDatum::onPageChanged(int pageId)
{
    if (selectedAxis().isEmpty()) {
        return;
    }
    if (page(pageId) != ui->pageDatum) {
        return;
    }
    TrioQueue *queue = MotionManager::driver()->queue();
    queue->Reset();
    QList<Axis *> requireCleanupAxisList;
    auto clearCleanupAxisList = [&] () {
        for (auto axis: requireCleanupAxisList) {
            AxisConfig config = axis->config();
            queue->WaitIdle(config.axisId);
            qreal datumDef = config.datumPosition;
            bool hasDef = !qFuzzyIsNull(datumDef);
            if (hasDef) {
                queue->ResetAxisPos(datumDef, config.axisId);
            }
            queue->MoveAbs(0, config.axisId);
            queue->WaitIdle(config.axisId);
            queue->SetAxisParameter(config.axisId, "CORNER_MODE", 1);
            queue->Wait(1000);
        }
        requireCleanupAxisList.clear();
    };
    // main datum process
    int prevSequence = -1;
    for (auto axis: selectedAxis()) {
        AxisConfig config = axis->config();
        if (config.datumInput < 0) {
            continue;
        }

        if (config.datumSequence != prevSequence) {
            clearCleanupAxisList();
        }

        queue->reportMoveStart(selectedAxis().indexOf(axis));
        queue->Home(config.axisId, config.datumSpeed, config.datumCreep, config.datumInput, config.datumReversed);
        queue->Home(config.axisId, config.datumSpeed, config.datumCreep, config.datumInput, config.datumReversed);
        requireCleanupAxisList << axis;
        prevSequence = config.datumSequence;
        queue->reportMoveEnd(selectedAxis().indexOf(axis));
    }
    clearCleanupAxisList();

    queue->Send();
}

void WizardDatum::onQueueStateUpdated(int state, int progress, int total)
{
    ui->progress->setMaximum(qMax(0, total));
    if (state == TrioQueue::STATE_SENDING) {
        setStatus(tr("正在发送复位指令..."));
    } else if (state == TrioQueue::STATE_STARTING) {
        setStatus(tr("正在启动复位程序"));
    } else if (state == TrioQueue::STATE_RUNNING) {
        const auto axisList = selectedAxis();
        Axis *axis = axisList.value(progress);
        QString name = axis ? axis->name() : QString();
        setStatus(tr("正在复位 %1").arg(name));
        ui->progress->setMaximum(axisList.size());
    } else if (state == TrioQueue::STATE_IDLE) {
        setStatus(tr("复位完成"));
        EndPoint *endPoint = MotionManager::endPoint(0);
        for (UTInstrument *intrument: UltrasonicManager::instruments()) {
            intrument->SetStandardPos(QVector<float>(endPoint->currentPos()));
        }
    } else if (state == TrioQueue::STATE_UNAVAILABLE) {
        setStatus(tr("复位失败"));
    }
    ui->progress->setValue(progress);
}

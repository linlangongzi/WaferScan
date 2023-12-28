#include "TemperatureManager.h"
#include "TemperatureView.h"
#include "ui_TemperatureView.h"
#include <QDateTimeAxis>
#include <QFileDialog>
#include <QInputDialog>
#include <QLineSeries>
#include <QSplineSeries>
#include <QTimer>
#include <QValueAxis>
#include <utils/treemodel.h>

using namespace QtCharts;

static void save(QSettings *settings, QObject *obj)
{
    QString name = obj->objectName();
    const char *userProp = obj->metaObject()->userProperty().name();
    if (!name.isEmpty() && userProp) {
        settings->setValue(name, obj->property(userProp));
    }
}

static void load(QSettings *settings, QObject *obj, const QVariant &defaultValue = QVariant())
{
    QString name = obj->objectName();
    const char *userProp = obj->metaObject()->userProperty().name();
    if (name.isEmpty() || userProp == NULL) {
        qDebug() << "Load: invalid name or userProp" << name << userProp;
        return;
    }
    QVariant value = settings->value(name);
    if (!value.isValid()) {
        qDebug() << "Load: invalid value" << value << "for obj" << name << userProp;
        value = defaultValue;
    }
    if (value.isValid()) {
        obj->setProperty(userProp, value);
    }
}

TemperatureView::TemperatureView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TemperatureView),
    timer(new QTimer(this))
{
    ui->setupUi(this);

    ui->sources->setModel(TemperatureManager::model());
    TemperatureManager::model()->setHeader(QStringList() << tr("通道") << tr("值") << tr("表达式"));

    connect(timer, &QTimer::timeout, this, &TemperatureView::appendData);
    connect(TemperatureManager::instance(), &TemperatureManager::sourcesChanged, this, &TemperatureView::onSourcesChanged);
    connect(ui->record, &QPushButton::toggled, [this](bool checked) {
        if (checked) {
            onSourcesChanged();
            startTime = QDateTime::currentDateTime();
            refreshTimeRange();
            timer->setInterval(ui->interval->value() * 1000);
            timer->start();
        } else {
            timer->stop();
        }
        ui->options->setDisabled(checked);
    });

    startTime = QDateTime::currentDateTime();

    ui->chart->setRenderHint(QPainter::Antialiasing);
    QChart *chart = ui->chart->chart();
    auto *axisX = new QDateTimeAxis(chart);
    chart->setAxisX(axisX);
    axisX->setFormat("MM-dd h:mm:ss");

    auto *axisY = new QValueAxis(chart);
    chart->setAxisY(axisY);
    axisY->setRange(ui->tempMin->value(), ui->tempMax->value());

    chart->legend()->setAlignment(Qt::AlignRight);
}

TemperatureView::~TemperatureView()
{
    delete ui;
}

void TemperatureView::loadSettings(QSettings *settings)
{
    load(settings, ui->interval, 1);
    load(settings, ui->tempMin, 0);
    load(settings, ui->tempMax, 100);
    load(settings, ui->range, 120);
    load(settings, ui->exportEnable);
    load(settings, ui->exportPath);
}

void TemperatureView::saveSettings(QSettings *settings)
{
    save(settings, ui->interval);
    save(settings, ui->tempMin);
    save(settings, ui->tempMax);
    save(settings, ui->range);
    save(settings, ui->exportEnable);
    save(settings, ui->exportPath);
}

void TemperatureView::appendData()
{
    QList<qreal> values = TemperatureManager::instance()->values();
    QList<QAbstractSeries *> series = ui->chart->chart()->series();
    QDateTime now = QDateTime::currentDateTime();
    qreal nowMsec = now.toMSecsSinceEpoch();
    qreal startMsec = nowMsec - ui->range->value() * 1000;

    for (int index = 0; index < series.count(); ++index) {
        qreal value = values.value(index, Q_SNAN);
        QXYSeries *line = qobject_cast<QXYSeries *>(series[index]);
        if (qIsFinite(value)) {
            line->append(nowMsec, value);
        }
        while (line->count() > 0 && line->at(0).x() < startMsec) {
            line->remove(0);
        }
    }
    if (startTime.toMSecsSinceEpoch() < startMsec) {
        startTime = QDateTime::fromMSecsSinceEpoch(startMsec);
        refreshTimeRange();
    }

    if (ui->exportEnable->isChecked()) {
        QFile file(ui->exportPath->text());
        if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QTextStream s(&file);
            s << now.toString();
            for (auto v: values) {
                s << "," << v;
            }
            s << endl;
        }
    }
}

void TemperatureView::refreshTimeRange()
{
    auto *axis = ui->chart->chart()->axisX();
    axis->setRange(startTime, startTime.addMSecs(ui->range->value() * 1000));
}

void TemperatureView::onSourcesChanged()
{
    QChart *chart = ui->chart->chart();
    chart->removeAllSeries();
    auto *root = TemperatureManager::model()->rootItem();
    for (int index = 0; index < root->childCount(); ++index) {
        auto *source = root->childAt(index);
        QLineSeries *series = new QLineSeries(chart);
        series->setName(source->path().toString());
        chart->addSeries(series);
        series->attachAxis(chart->axisX());
        series->attachAxis(chart->axisY());
        connect(series, &QXYSeries::hovered, this, &TemperatureView::onSeriesHover);
    }
}

void TemperatureView::onSeriesHover(const QPointF &pos, bool state)
{
    if (state) {
        QDateTime time = QDateTime::fromMSecsSinceEpoch(pos.x());
        QString tooltip = tr("时间: %1\n温度: %2").arg(time.toString()).arg(pos.y());
        ui->chart->setToolTip(tooltip);
    } else {
        ui->chart->setToolTip(QString());
    }
}

void TemperatureView::on_addSource_clicked()
{
    QString name = QInputDialog::getText(this, QString(), tr("数据源"));
    if (name.isEmpty()) {
        return;
    }

    TemperatureManager::instance()->addSource(name);
}

void TemperatureView::on_removeSource_clicked()
{
    QModelIndex current = ui->sources->currentIndex();
    TemperatureManager::instance()->removeSource(current);
}

void TemperatureView::on_tempMin_valueChanged(double arg1)
{
    ui->tempMax->setMinimum(arg1);
    ui->chart->chart()->axisY()->setMin(arg1);
}

void TemperatureView::on_tempMax_valueChanged(double arg1)
{
    ui->tempMin->setMaximum(arg1);
    ui->chart->chart()->axisY()->setMax(arg1);
}

void TemperatureView::on_range_valueChanged(double arg1)
{
    Q_UNUSED(arg1);
    refreshTimeRange();
}

void TemperatureView::on_exportBrowse_clicked()
{
    QString path = QFileDialog::getSaveFileName(this, QString(), ui->exportPath->text(), tr("csv (*.csv);;所有文件 (*)"));
    if (!path.isEmpty()) {
        ui->exportPath->setText(path);
    }
}

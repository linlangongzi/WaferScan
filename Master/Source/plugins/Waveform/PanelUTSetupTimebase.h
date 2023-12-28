#pragma once

#include <QWidget>
#include <Ultrasonic/UTSetup.h>

namespace Ui {
class PanelUTSetupTimebase;
}

class PanelUTSetupTimebase : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(UTSetupTimebase timebase READ timebase WRITE setTimebase NOTIFY timebaseChanged)
    Q_PROPERTY(bool readOnly READ isReadOnly WRITE setReadOnly)

public:
    explicit PanelUTSetupTimebase(QWidget *parent = nullptr);
    ~PanelUTSetupTimebase();
    UTSetupTimebase timebase() const;
    bool isReadOnly() const;
    void setSurfaceTracingVisible(bool visible);

public slots:
    void setTimebase(const UTSetupTimebase &timebase);
    void setReadOnly(bool readOnly);

signals:
    void timebaseChanged(const UTSetupTimebase &timebase);

private slots:
    void refreshControls();
    void on_changeMaterial_clicked();
    void on_preferTime_toggled(bool checked);
    void on_materialSpeed_valueChanged(qreal arg1);
    void onSurfaceTracingChanged(const UTSetupSurfaceTracing &surfaceTracing);

private:
    qreal toTime(qreal value) const;
    qreal toDistance(qreal value) const;

private:
    Ui::PanelUTSetupTimebase *ui;
    qreal m_delay = 0;
    qreal m_range = 0;
    qreal m_zero = 0;
    Material m_material;
    bool m_readOnly = false;
    UTSetupTimebase m_timebase;
};

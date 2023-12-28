#pragma once

#include <QWidget>
#include <Ultrasonic/UTSetupTimebase.h>
#include <allrising/ConnectionHelper.h>

namespace Ui {
class PanelUTSetupSurfaceTracing;
}

class PanelUTSetupSurfaceTracing : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(UTSetupSurfaceTracing surfaceTracing READ surfaceTracing WRITE setSurfaceTracing NOTIFY surfaceTracingChanged)

public:
    explicit PanelUTSetupSurfaceTracing(QWidget *parent = nullptr);
    ~PanelUTSetupSurfaceTracing();

    UTSetupSurfaceTracing surfaceTracing() const;

public slots:
    void setSurfaceTracing(const UTSetupSurfaceTracing &surfaceTracing);

signals:
    void surfaceTracingChanged(UTSetupSurfaceTracing surfaceTracing);
    
private:
    void refreshControls();

    Ui::PanelUTSetupSurfaceTracing *ui;
    UTSetupSurfaceTracing m_surfaceTracing;
    ConnectionHelper<PanelUTSetupSurfaceTracing, UTSetupSurfaceTracing> m_handler;
};

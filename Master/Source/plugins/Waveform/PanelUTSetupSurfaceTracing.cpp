#include "PanelUTSetupSurfaceTracing.h"
#include "ui_PanelUTSetupSurfaceTracing.h"

PanelUTSetupSurfaceTracing::PanelUTSetupSurfaceTracing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelUTSetupSurfaceTracing),
    m_handler(this, &PanelUTSetupSurfaceTracing::surfaceTracing, &PanelUTSetupSurfaceTracing::setSurfaceTracing)
{
    ui->setupUi(this);

    m_handler.add(ui->surfaceTracing, &UTSetupSurfaceTracing::enable);
    m_handler.add(ui->surfaceOffset, &UTSetupSurfaceTracing::offset);
    m_handler.add(ui->surfaceEarliest, &UTSetupSurfaceTracing::earlist);
    m_handler.add(ui->surfaceLatest, &UTSetupSurfaceTracing::latest);
    m_handler.add(ui->surfaceThreshold, &UTSetupSurfaceTracing::threshold);

    refreshControls();
}

PanelUTSetupSurfaceTracing::~PanelUTSetupSurfaceTracing()
{
    delete ui;
}

UTSetupSurfaceTracing PanelUTSetupSurfaceTracing::surfaceTracing() const
{
    return m_surfaceTracing;
}

void PanelUTSetupSurfaceTracing::setSurfaceTracing(const UTSetupSurfaceTracing &surfaceTracing)
{
    if (m_surfaceTracing == surfaceTracing) {
        return;
    }

    m_surfaceTracing = surfaceTracing;

    refreshControls();

    emit surfaceTracingChanged(m_surfaceTracing);
}

void PanelUTSetupSurfaceTracing::refreshControls()
{
    m_handler.setControls(m_surfaceTracing);
    ui->surfaceContainer->setEnabled(m_surfaceTracing.enable);
}

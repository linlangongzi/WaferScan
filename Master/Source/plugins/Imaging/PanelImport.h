#pragma once

#include "ImagingGlobal.h"
#include <QDialog>
#include <Ultrasonic/UTDataHeader.h>

class ImageRecorder;

namespace Ui {
class PanelImport;
}

class IMAGING_EXPORT PanelImport : public QDialog
{
    Q_OBJECT

public:
    explicit PanelImport(QWidget *parent = 0);
    ~PanelImport();

private slots:
    void on_load_clicked();

private:
    void readyToRead(const QString &fileName, int numGate, int numPos);

    Ui::PanelImport *ui;
    UTDataHeader m_header;
    ImageRecorder *m_recorder = Q_NULLPTR;
    QHash<int, int> m_gateLookup;
    QHash<int, qreal> m_syncOffset;
    qreal m_waveformScale = 1.0;
    int m_waveformMax = 1024;
};


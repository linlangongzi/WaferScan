#pragma once

#include <QFrame>
#include <Imaging/ImageOptions.h>
#include <Ultrasonic/UTData.h>

#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>

namespace Ui {
class Panel3DVolume;
}

namespace QtDataVisualization {
class Q3DSurface;
class Q3DScatter;
class QCustom3DVolume;
class QCustom3DLabel;
}

class ColorPalette;
class QAbstractButton;

class ImageViewer : public QDialog {
    Q_OBJECT
public:
    ImageViewer(QWidget *parent) :
        QDialog(parent),
        label(new QLabel(this))
    {
        QHBoxLayout *layout = new QHBoxLayout(this);
        setLayout(layout);
        layout->addWidget(label);
    }
public slots:
    void SetPixmap(const QPixmap &pixmap) {
        label->setPixmap(pixmap);
    }
private:
    QLabel *label;
};


class Panel3DVolume : public QFrame
{
    Q_OBJECT

public:
    explicit Panel3DVolume(QWidget *parent = 0);
    ~Panel3DVolume();

public slots:
    void handleFpsChange(qreal fps);
    void setPreserveOpacity(bool enabled);
    void SetAlphaMultiplier(int value);
    void setDrawSliceFrames(int enabled);

    void SetTransparentBand(int value);
    void OnImageOptionsUpdated(const ImageOptions &trans);
    void OnDataArrived(const QHash<QPoint, UTDataFrame> &segment);
    void OnRequestedDataArrived(const UTDataSegment &segment, void *reference);
    void OnColorPaletteChanged();
    void UpdateVolume();
    void ExportSlices();
    void UpdateAxisRange();

    void UpdateSliceImages();
    void UpdateSliceImageX();
    void UpdateSliceImageY();
    void UpdateSliceImageZ();
    void UpdateSlices();

signals:
    void RequestDataAll(void *reference);

private slots:
    void onLicenseUpdated();
    void OnOriginChanged(const QVector3D &newOrigin);
    void OnShaderHintUpdated(bool useHighDef);
    void on_measureFps_toggled(bool checked);
    void on_sliceShowX_clicked();
    void on_sliceShowY_clicked();
    void on_sliceShowZ_clicked();
    void on_surfaceTrimButtons_clicked(QAbstractButton *button);
    void on_shaderQuality_currentIndexChanged(int index);

private:
    bool eventFilter(QObject *o, QEvent *e);
    void showEvent(QShowEvent *);

private:
    typedef QtDataVisualization::Q3DScatter GraphType;
    Ui::Panel3DVolume *ui;
    GraphType *graph;
    QtDataVisualization::QCustom3DVolume *volumeItem;
    QtDataVisualization::QCustom3DLabel *memoryLabel;
    QVector<uchar> *textureData;
    int paletteTransparentBand;
    ImageOptions transform;
    QVector3D origin;
    float delay;
    float range;
    bool needsUpdate = false;
    ImageViewer *sliceViewX;
    ImageViewer *sliceViewY;
    ImageViewer *sliceViewZ;
};

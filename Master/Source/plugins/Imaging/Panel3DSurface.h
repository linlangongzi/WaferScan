#pragma once

#include <Imaging/UTImage.h>
#include <QFrame>

namespace QtDataVisualization {
class Q3DSurface;
class QCustom3DVolume;
}

namespace Ui {
class Panel3DSurface;
};

class Panel3DSurface : public QFrame
{
    Q_OBJECT

public:
    explicit Panel3DSurface(QWidget *parent = 0);

private:
    void showEvent(QShowEvent *);

private slots:
    void onLicenseUpdated();
    void OnImagesUpdated(UTImageList images);
    void OnFPSUpdated(qreal value);
    void UpdateSurface();

private:
    Ui::Panel3DSurface *ui;
    UTImageList imageList;
    QtDataVisualization::Q3DSurface *surface;                                   /*!< 3D曲面图，这个控件的析构由 QObject 父子关系负责处理 */
    QtDataVisualization::QCustom3DVolume *volume;
    bool needUpdate = false;
};

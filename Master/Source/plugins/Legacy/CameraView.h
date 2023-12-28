#pragma once

#include <QFrame>
#include <QCameraInfo>

namespace Ui {
class CameraView;
}

class QCamera;

class CameraView : public QFrame
{
    Q_OBJECT

public:
    explicit CameraView(QWidget *parent = 0);
    ~CameraView();

protected:
    void hideEvent(QHideEvent *e);
    void showEvent(QShowEvent *e);

private slots:
    void on_refresh_clicked();
    void on_cameraList_activated(int index);

private:
    Ui::CameraView *ui;
    int prevCameraIndex;
    QList<QCameraInfo> cameras;
    QCamera *camera;
};

#include "CameraView.h"
#include "ui_CameraView.h"

#include <QCameraInfo>
#include <QCamera>
#include <QCameraViewfinder>

CameraView::CameraView(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::CameraView),
    prevCameraIndex(-1),
    camera(nullptr)
{
    ui->setupUi(this);
    on_refresh_clicked();
}

CameraView::~CameraView()
{
    delete ui;
}

void CameraView::hideEvent(QHideEvent *e)
{
    if (camera) {
        camera->stop();
    }
    return QFrame::hideEvent(e);
}

void CameraView::showEvent(QShowEvent *e)
{
    if (camera) {
        camera->start();
    }
    return QFrame::showEvent(e);
}

void CameraView::on_refresh_clicked()
{
    QCameraInfo currentCamera = cameras.value(ui->cameraList->currentIndex());
    cameras = QCameraInfo::availableCameras();

    ui->cameraList->clear();
    foreach (const QCameraInfo &info, cameras) {
        ui->cameraList->addItem(info.description());
    }

    /* 恢复旧的index */
    int currentIndex = qMax(cameras.indexOf(currentCamera), 0);                 /* 选择之前的camera。如果之前的camera不再存在，选择第0项。 */
    prevCameraIndex = -1;
    ui->cameraList->setCurrentIndex(currentIndex);
    on_cameraList_activated(currentIndex);

    /* 如果没有发现任何camera，隐藏StackWidget并且提示 */
    if (cameras.isEmpty()) {
        ui->cameraGroup->setCurrentWidget(ui->cameraNone);
    } else {
        ui->cameraGroup->setCurrentWidget(ui->cameraView);
    }
}

void CameraView::on_cameraList_activated(int index)
{
    if (prevCameraIndex == index) {
        return;
    }
    if (camera) {
        camera->stop();
    }
    delete camera;

    QCameraInfo newCamera = cameras.value(index);
    if (newCamera.isNull()) {
        return;
    }
    camera = new QCamera(newCamera, this);
    prevCameraIndex = index;
    camera->setViewfinder(ui->cameraView);
    if (isVisible()) {
        camera->start();
    }
}

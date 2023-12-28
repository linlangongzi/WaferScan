#include "Panel3DVolume.h"
#include "ui_Panel3DVolume.h"
#include <Core/LicenseManager.h>
#include <Core/Platform.h>
#include <Imaging/ColorPalette.h>
#include <Imaging/UTDocument.h>
#include <Imaging/UTLayer.h>
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QOpenGLContext>
#include <QScreen>
#include <QtDataVisualization/Q3DCamera>
#include <QtDataVisualization/Q3DInputHandler>
#include <QtDataVisualization/Q3DScatter>
#include <QtDataVisualization/Q3DScene>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/Q3DTheme>
#include <QtDataVisualization/QCustom3DLabel>
#include <QtDataVisualization/QCustom3DVolume>
#include <QtDataVisualization/QValue3DAxis>
#include <QtMath>
#include <allrising/QtExtend.h>

enum ShaderQuality {
    ShaderLowQuality = 0,
    ShaderAutoQuality = 1,
    ShaderHighQuality = 2
};

using namespace QtDataVisualization;

const int waveformDepth(512);

static bool isOpenGLES()
{
#if defined(QT_OPENGL_ES_2)
    return true;
#elif (QT_VERSION < QT_VERSION_CHECK(5, 3, 0))
    return false;
#else
    return QOpenGLContext::currentContext()->isOpenGLES();
#endif
}

Panel3DVolume::Panel3DVolume(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Panel3DVolume),
    graph(new GraphType()),
    volumeItem(nullptr),
    memoryLabel(nullptr),
    textureData(nullptr),
    paletteTransparentBand(0),
    delay(0),
    range(0)
{
    ui->setupUi(this);
    sliceViewX = new ImageViewer(this);
    sliceViewY = new ImageViewer(this);
    sliceViewZ = new ImageViewer(this);

    QWidget *container = QWidget::createWindowContainer(graph);
    QSize screenSize = graph->screen()->size();
    container->setMinimumSize(QSize(screenSize.width() / 3, screenSize.height() / 3));
    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);

    ui->containerLayout->insertWidget(1, container, 1);

    graph->setMargin(0);
    graph->activeTheme()->setType(Q3DTheme::ThemeQt);
    graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetIsometricLeft);
    graph->scene()->activeCamera()->setWrapXRotation(true);
    graph->scene()->activeCamera()->setWrapYRotation(false);
    graph->setOrthoProjection(true);
//    graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);           /* 使用正等测时阴影会被屏蔽 */
    graph->activeTheme()->setBackgroundEnabled(false);
    graph->axisZ()->setReversed(true);
    graph->axisX()->setTitle(tr("Z"));
    graph->axisY()->setTitle(tr("Y"));
    graph->axisZ()->setTitle(tr("X"));
#ifndef ARS_FINAL_RELEASE
    graph->axisX()->setTitle(graph->axisX()->title() + "(Qt-X)");
    graph->axisY()->setTitle(graph->axisY()->title() + "(Qt-Y)");
    graph->axisZ()->setTitle(graph->axisZ()->title() + "(Qt-Z)");
#endif

    // Only allow zooming at the center and limit the zoom to 200% to avoid clipping issues
    static_cast<Q3DInputHandler *>(graph->activeInputHandler())->setZoomAtTargetEnabled(false);
    graph->scene()->activeCamera()->setMaxZoomLevel(1000.0f);
    graph->installEventFilter(this);

    if (!isOpenGLES()) {
        volumeItem = new QCustom3DVolume;
        // Adjust water level to zero with a minor tweak to y-coordinate position and scaling
        volumeItem->setTextureFormat(QImage::Format_Indexed8);

        volumeItem->setSliceFrameGaps(QVector3D(0.01f, 0.02f, 0.01f));
        volumeItem->setSliceFrameThicknesses(QVector3D(0.0025f, 0.005f, 0.0025f));
        volumeItem->setSliceFrameWidths(QVector3D(0.0025f, 0.005f, 0.0025f));
        volumeItem->setDrawSliceFrames(false);
        volumeItem->setShadowCasting(false);
        volumeItem->setScalingAbsolute(false);
        volumeItem->setPositionAbsolute(false);

        graph->addCustomItem(volumeItem);

    } else {
        // OpenGL ES2 doesn't support 3D textures, so show a warning label instead
        QCustom3DLabel *warningLabel = new QCustom3DLabel(
                    "QCustom3DVolume is not supported with OpenGL ES2",
                    QFont(),
                    QVector3D(0.0f, 0.5f, 0.0f),
                    QVector3D(1.5f, 1.5f, 0.0f),
                    QQuaternion());
        warningLabel->setPositionAbsolute(true);
        warningLabel->setFacingCamera(true);
        graph->addCustomItem(warningLabel);
        return;
    }

    connect(graph, &QAbstract3DGraph::currentFpsChanged, this, &Panel3DVolume::handleFpsChange);
    connect(ui->suppress, &QSlider::valueChanged, this, &Panel3DVolume::OnColorPaletteChanged);

    ui->shaderQuality->setCurrentIndex(ShaderAutoQuality);
//    volumeItem->setUseHighDefShader(false);
//    connect(ui->highDefShader, &QCheckBox::toggled, volumeItem, &QCustom3DVolume::setUseHighDefShader);
//    ui->highDefShader->setChecked(false);

    connect(ui->alpha, &QSlider::valueChanged, this, &Panel3DVolume::SetAlphaMultiplier);
    ui->alpha->setValue(ui->alpha->maximum() / 2);

    connect(ui->sliceEnableX, &QCheckBox::toggled, this, &Panel3DVolume::UpdateSlices);
    connect(ui->sliceEnableY, &QCheckBox::toggled, this, &Panel3DVolume::UpdateSlices);
    connect(ui->sliceEnableZ, &QCheckBox::toggled, this, &Panel3DVolume::UpdateSlices);
    connect(ui->sliceSliderX, &QSlider::valueChanged, this, &Panel3DVolume::UpdateSlices);
    connect(ui->sliceSliderY, &QSlider::valueChanged, this, &Panel3DVolume::UpdateSlices);
    connect(ui->sliceSliderZ, &QSlider::valueChanged, this, &Panel3DVolume::UpdateSlices);
    connect(ui->sliceSliderX, &QSlider::valueChanged, this, &Panel3DVolume::UpdateSliceImageX);
    connect(ui->sliceSliderY, &QSlider::valueChanged, this, &Panel3DVolume::UpdateSliceImageY);
    connect(ui->sliceSliderZ, &QSlider::valueChanged, this, &Panel3DVolume::UpdateSliceImageZ);

    connect(ui->scaleEnable, &QGroupBox::toggled, this, &Panel3DVolume::UpdateAxisRange);
    connect(ui->scaleEnableX, &QCheckBox::toggled, this, &Panel3DVolume::UpdateAxisRange);
    connect(ui->scaleEnableY, &QCheckBox::toggled, this, &Panel3DVolume::UpdateAxisRange);
    connect(ui->scaleEnableZ, &QCheckBox::toggled, this, &Panel3DVolume::UpdateAxisRange);
    connect(ui->scaleX, &QSlider::valueChanged, this, &Panel3DVolume::UpdateAxisRange);
    connect(ui->scaleY, &QSlider::valueChanged, this, &Panel3DVolume::UpdateAxisRange);
    connect(ui->scaleZ, &QSlider::valueChanged, this, &Panel3DVolume::UpdateAxisRange);

    connect(ui->palette, &ColorPaletteView::colorPaletteChanged, this, &Panel3DVolume::OnColorPaletteChanged);

    UTLayer *layer = document()->GetLayerByID(1);
    if (layer != nullptr) {
        connect(layer, &UTLayer::MatrixDataUpdated, this, &Panel3DVolume::OnDataArrived);
        connect(layer, &UTLayer::SegmentUpdate, this, &Panel3DVolume::OnRequestedDataArrived);
        connect(this, &Panel3DVolume::RequestDataAll, layer, &UTLayer::RequestDataAll);
        connect(document(), &UTDocument::originChanged, this, &Panel3DVolume::OnOriginChanged);
        connect(layer, &UTLayer::ImageOptionsChanged, this, &Panel3DVolume::OnImageOptionsUpdated);

//        int coloePaletteID = layer->GetColorPaletteID();
//        palette = platform()->ColorPalettes()->GetByID<ColorPalette>(coloePaletteID);
//        if (palette != NULL) {
//            connect(palette, &ColorPalette::ColorPaletteChanged, this, &Panel3DVolume::OnColorPaletteChanged);
//            connect(palette, &ColorPalette::ValueChanged, this, &Panel3DVolume::OnColorPaletteChanged);
//        }
        OnColorPaletteChanged();
    }

    LicenseManager::registerLicenseUpdate(this, &Panel3DVolume::onLicenseUpdated);
}

Panel3DVolume::~Panel3DVolume()
{
    delete graph;
    delete ui;
}

void Panel3DVolume::setPreserveOpacity(bool enabled)
{
    if (volumeItem) {
        volumeItem->setPreserveOpacity(enabled);
        UpdateSliceImages();
    }
}

void Panel3DVolume::SetAlphaMultiplier(int value)
{
    if (volumeItem) {
        float mult = float(value) / float(256 - value);
        volumeItem->setAlphaMultiplier(mult);
        ui->alphaValue->setText(QString::number(volumeItem->alphaMultiplier(), 'f', 3));
        UpdateSliceImages();
    }
}

void Panel3DVolume::setDrawSliceFrames(int enabled)
{
    if (volumeItem)
        volumeItem->setDrawSliceFrames(enabled);
}

void Panel3DVolume::SetTransparentBand(int value)
{
    paletteTransparentBand = value;
    OnColorPaletteChanged();
}

void Panel3DVolume::OnImageOptionsUpdated(const ImageOptions &trans)
{
    qDebug() << "Volumetric" << trans;
    QSize imageSize = trans.targetRect();
    int width = qMax(0, imageSize.width());
    int height = qMax(0, imageSize.height());

    textureData = nullptr;
    volumeItem->setTextureData(nullptr);                                           /* 重新分配内存之前，首先析构旧的纹理 */

    if (double(width) * height * waveformDepth < INT_MAX) {
        textureData = new QVector<uchar>(width * height * waveformDepth);
        textureData->squeeze();
        volumeItem->setTextureDimensions(waveformDepth, height, width);
        if (memoryLabel) {
            graph->removeCustomItem(memoryLabel);
            memoryLabel = nullptr;
        }
    } else {
        qDebug() << "3DVolume: texture too large";
        volumeItem->setTextureDimensions(0, 0, 0);
        if (!memoryLabel) {
            memoryLabel = new QCustom3DLabel(tr("图像尺寸过大，无法显示3D图像"), QFont(), QVector3D(0.0f, 0.5f, 0.0f), QVector3D(1.5f, 1.5f, 0.0f), QQuaternion());
            memoryLabel->setFacingCamera(true);
            memoryLabel->setPositionAbsolute(true);
            graph->addCustomItem(memoryLabel);
        }
    }
    volumeItem->setTextureData(textureData);
    transform = trans;

    ui->sliceSliderX->setRange(0, imageSize.width() - 1);
    ui->sliceSliderY->setRange(0, imageSize.height() - 1);
    ui->sliceSliderZ->setRange(0, waveformDepth - 1);

    UpdateAxisRange();
}

void Panel3DVolume::UpdateAxisRange()
{
    float scaleX = 1;
    float scaleY = 1;
    float scaleZ = 1;
    if (ui->scaleEnable->isChecked()) {
        float scaleFactorX = qPow(10, qreal(ui->scaleX->value()) / 100);
        float scaleFactorY = qPow(10, qreal(ui->scaleY->value()) / 100);
        float scaleFactorZ = qPow(10, qreal(ui->scaleZ->value()) / 100);
        scaleX = ui->scaleEnableX->isChecked() ? scaleFactorX : scaleX;
        scaleY = ui->scaleEnableY->isChecked() ? scaleFactorY : scaleY;
        scaleZ = ui->scaleEnableZ->isChecked() ? scaleFactorZ : scaleZ;
        ui->scaleX->setEnabled(ui->scaleEnableX->isChecked());
        ui->scaleY->setEnabled(ui->scaleEnableY->isChecked());
        ui->scaleZ->setEnabled(ui->scaleEnableZ->isChecked());
        ui->scaleValueX->setText(tr("x%1").arg(scaleFactorX, 3, 'f', 1, ' '));
        ui->scaleValueY->setText(tr("x%1").arg(scaleFactorY, 3, 'f', 1, ' '));
        ui->scaleValueZ->setText(tr("x%1").arg(scaleFactorZ, 3, 'f', 1, ' '));
    }

    QRectF sourceRect = transform.sourceRect();
    if (!sourceRect.isValid()) {
        return;
    }
    qreal sourceDepth = 128;
    float sourceWidth = qMax<float>(0, sourceRect.width());
    float sourceHeight = qMax<float>(0, sourceRect.height());

    float scaledWidth = sourceWidth * scaleX;
    float scaledHeight = sourceHeight * scaleY;
    float scaledDepth = sourceDepth * scaleZ;
    qreal longestAxis = qMax<float>(scaledDepth, scaledWidth);
    graph->setAspectRatio(longestAxis / scaledHeight);
    graph->setHorizontalAspectRatio(scaledDepth / scaledWidth);

    foreach (QValue3DAxis *axis, graph->axes()) {
        axis->setTitleVisible(true);
        axis->setAutoAdjustRange(false);
    }

    float offsetX = origin.x();
    float offsetY = origin.y();
//    float offsetZ = origin.z();
    /* 将3D材质绕Z轴旋转90度，可以交换X轴和Y轴，使A扫深度方向变为3DGraph中的Y轴，也就是常规迪卡尔坐标系的Z轴 */
//    volumeItem->setRotation(QQuaternion::fromAxisAndAngle(0, 0, 1, -90));
    /* 由于3DVolume将材质尺寸正规化为[-1, 1]，偏移材质时将中心移到场景中心 */
    volumeItem->setScaling(QVector3D(
        sourceDepth,
        sourceHeight,
        -sourceWidth
    ));
    volumeItem->setPosition(QVector3D(
        sourceDepth / 2,
        sourceRect.center().y() - offsetY,
        sourceRect.center().x() - offsetX
    ));
    graph->axisX()->setRange(0, sourceDepth);
    graph->axisY()->setRange(sourceRect.top() - offsetY, sourceRect.bottom() - offsetY);
    graph->axisZ()->setRange(sourceRect.left() - offsetX, sourceRect.right() - offsetX);
}

void Panel3DVolume::UpdateSliceImages()
{
    UpdateSliceImageX();
    UpdateSliceImageY();
    UpdateSliceImageZ();
}

void Panel3DVolume::UpdateSliceImageX()
{
    if (sliceViewX && sliceViewX->isVisible()) {
        int index = ui->sliceSliderX->value();
        sliceViewX->SetPixmap(QPixmap::fromImage(volumeItem->renderSlice(Qt::ZAxis, index)));
        qreal pos = transform.MapFromGrid(QPointF(index, 0)).x();
        sliceViewX->setWindowTitle(tr("X方向切片: %1").arg(pos));
    }
}

void Panel3DVolume::UpdateSliceImageY()
{
    if (sliceViewY && sliceViewY->isVisible()) {
        int index = ui->sliceSliderY->value();
        sliceViewY->SetPixmap(QPixmap::fromImage(volumeItem->renderSlice(Qt::YAxis, index)));
        qreal pos = transform.MapFromGrid(QPointF(index, 0)).y();
        sliceViewY->setWindowTitle(tr("Y方向切片: %1").arg(pos));
    }
}

void Panel3DVolume::UpdateSliceImageZ()
{
    if (sliceViewZ && sliceViewZ->isVisible()) {
        int index = ui->sliceSliderZ->value();
        sliceViewZ->SetPixmap(QPixmap::fromImage(volumeItem->renderSlice(Qt::XAxis, index)));
//        qreal pos = transform.MapFromGrid(QPointF(index, 0)).x();
        sliceViewZ->setWindowTitle(tr("Z方向切片: %1").arg(index));
    }
}

void Panel3DVolume::UpdateSlices()
{
    bool xEnabled = ui->sliceEnableX->isChecked();
    bool yEnabled = ui->sliceEnableY->isChecked();
    bool zEnabled = ui->sliceEnableZ->isChecked();
    bool anyEnabled = xEnabled || yEnabled || zEnabled;
    volumeItem->setDrawSliceFrames(anyEnabled);
    volumeItem->setDrawSlices(anyEnabled);

    volumeItem->setSliceIndexX(zEnabled ? ui->sliceSliderZ->value() : -1);
    volumeItem->setSliceIndexY(yEnabled ? ui->sliceSliderY->value() : -1);
    volumeItem->setSliceIndexZ(xEnabled ? ui->sliceSliderX->value() : -1);
}

void Panel3DVolume::onLicenseUpdated()
{
    bool develop = LicenseManager::instance()->hasDevelopLicense();
    ui->surfaceTrimButtons->setVisible(develop);
    ui->trimOffsetValue->setVisible(develop);
}

void Panel3DVolume::OnOriginChanged(const QVector3D &newOrigin)
{
    if (newOrigin != origin) {
        QMetaObject::invokeMethod(this, "UpdateAxisRange", Qt::QueuedConnection);
    }
    origin = newOrigin;
}

void Panel3DVolume::OnShaderHintUpdated(bool useHighDef)
{
    if (ui->shaderQuality->currentIndex() == ShaderAutoQuality) {
        volumeItem->setUseHighDefShader(useHighDef);
    }
}

void Panel3DVolume::OnDataArrived(const QHash<QPoint, UTDataFrame> &segment)
{
    if (!isVisibleTo(QApplication::activeWindow())) {
//        qDebug() << "Volume: not active, posponding";
        needsUpdate = true;
        return;
    }

    if (segment.isEmpty() || !textureData) {
        return;
    }
    qDebug() << "Volumetric rendering segment" << segment.count();
    const UTDataFrame &firstFrame = segment.cbegin().value();
    int sampleLength = 512;

    delay = firstFrame.Context.GetDelay();
    range = firstFrame.Context.GetRange();
    UpdateAxisRange();
    double sampleTime = float(sampleLength) / range;
    int imageWidth = volumeItem->textureDepth();                                /*!< 图像的宽度方向是3D纹理的Z方向 */
    int imageHeight = volumeItem->textureHeight();                              /*!< 图像的高度方向是3D纹理的Y方向 */
    bool isTrimming = ui->surfaceTrim->isChecked();
    int trimOffset = ui->trimOffset->value();
    QVector<uchar> &data = *textureData;

    for (auto it = segment.cbegin(); it != segment.cend(); ++it) {
        const UTDataFrame &frame = it.value();
        const QPoint point = it.key();
        if (isTrimming && !frame.IsCDataValid()) {
            continue;
        }
        int frameOffset = ((imageWidth - 1 - point.x()) * imageHeight + point.y()) * waveformDepth;
        int zOffset = 0;
        if (isTrimming) {
            zOffset = trimOffset + frame.syncTime() * sampleTime;
        }
        int count = qMin(frame.waveformLength(), waveformDepth);               /* 保证图像的Z深度及A扫描数据均不会越界 */
        int start = qBound(0, 0 - zOffset, count);                              /* 由于对A扫描数据取样时索引为[index + zOffset]， */
        int end = qBound(0, count - zOffset, count);                            /* index范围应当进行限制，确保不会越界 */
        for (int index = 0; index < start; ++index) {
            data[frameOffset + index] = 0;
        }
        for (int index = start; index < end; ++index) {
            qint16 value = frame.AData[index + zOffset] >> 2;
            data[frameOffset + index] = qMin<uchar>(0xFF, uchar(value));
        }
        for (int index = end; index < waveformDepth; ++index) {
            data[frameOffset + index] = 0;
        }
    }
    UpdateVolume();
//    qDebug() << "Volumetric done render." << clock() - start << "ms, texture size" << textureData->size();
}

void Panel3DVolume::OnRequestedDataArrived(const UTDataSegment &segment, void *reference)
{
    if (reference != this) {
        return;
    }
    const QSize rect = transform.targetRect();
    QHash<QPoint, UTDataFrame> points;
    for (int row = 0; row < rect.height(); ++row) {
        int rowOffset = row * rect.width();
        for (int col = 0; col < rect.width(); ++col) {
            int offset = rowOffset + col;
            points.insert(QPoint(col, row), segment[offset]);
        }
    }
    OnDataArrived(points);
}

void Panel3DVolume::OnColorPaletteChanged()
{
    UTImagePalette palette = ui->palette->colorPalette().GetColorPalette(true);
    qreal suppressPercent = qreal(ui->suppress->value()) / ui->suppress->maximum();
    int suppress = qRound(suppressPercent * 0x100);
    QVector<QRgb> colorTable = palette.colorTable;
    for (int index = 0; index < suppress; ++index) {
        colorTable[index] = 0x0;
    }
    ui->suppressValue->setText(tr("%1%").arg(suppressPercent * 100, 3, 'f', 1));
    volumeItem->setColorTable(colorTable);
    UpdateSliceImages();
}

void Panel3DVolume::UpdateVolume()
{
    volumeItem->setTextureData(textureData);
}

void Panel3DVolume::ExportSlices()
{
    QString dirPath = QFileDialog::getExistingDirectory(nullptr, tr("Select a location to export"), QString(), nullptr);
    QDir dir(dirPath);
    if (dir.exists()) {

    }
}

void Panel3DVolume::on_measureFps_toggled(bool checked)
{
    graph->setMeasureFps(checked);
    if (checked) {
        ui->fps->setText(QStringLiteral("..."));
    } else {
        ui->fps->setText(QString());
    }
}

void Panel3DVolume::handleFpsChange(qreal fps)
{
    const QString fpsFormat = QStringLiteral("FPS: %1");
    auto fps10 = int(fps * 10.0);
    ui->fps->setText(fpsFormat.arg(qreal(fps10) / 10.0));
}

void Panel3DVolume::on_sliceShowX_clicked()
{
    sliceViewX->show();
    UpdateSliceImageX();
}

void Panel3DVolume::on_sliceShowY_clicked()
{
    sliceViewY->show();
    UpdateSliceImageY();
}

void Panel3DVolume::on_sliceShowZ_clicked()
{
    sliceViewZ->show();
    UpdateSliceImageZ();
}

void Panel3DVolume::on_surfaceTrimButtons_clicked(QAbstractButton *button)
{
    if (ui->surfaceTrimButtons->buttonRole(button) == QDialogButtonBox::ApplyRole) {
        OnImageOptionsUpdated(transform);
        emit RequestDataAll(this);
    }
}

void Panel3DVolume::on_shaderQuality_currentIndexChanged(int index)
{
    switch (index) {
    case ShaderLowQuality:    volumeItem->setUseHighDefShader(false); break;
    case ShaderHighQuality:   volumeItem->setUseHighDefShader(true); break;
    case ShaderAutoQuality:   volumeItem->setUseHighDefShader(true); break;
    default:                    Q_UNREACHABLE();
    }
}

bool Panel3DVolume::eventFilter(QObject *o, QEvent *e)
{
    if (o == graph && ui->shaderQuality->currentIndex() == 1) {                 /* 智能切换渲染质量 */
        if (e->type() == QEvent::MouseButtonPress && static_cast<QMouseEvent *>(e)->button() == Qt::RightButton) {
            volumeItem->setUseHighDefShader(false);
        } else if (e->type() == QEvent::MouseButtonRelease) {
            volumeItem->setUseHighDefShader(true);
        }
    }
    return false;
}

void Panel3DVolume::showEvent(QShowEvent *)
{
    if (needsUpdate) {
        emit RequestDataAll(this);
        needsUpdate = false;
    }
}

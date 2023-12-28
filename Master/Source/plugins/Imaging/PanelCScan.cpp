#include "PanelCScan.h"
#include "ui_PanelCScan.h"
#include "CScanDefectParameter.h"
#include "DefectItemModel.h"
#include "DefectManager.h"
#include "DialogImageABCOptions.h"
#include "ProbeIndicatorManager.h"
#include "ReportWriterQtExcel.h"
#include "AnnotationLineItem.h"
#include <Acquisition/ScanTask.h>
#include <Ars1000/PipeInstrumentDriver.h>
#include <Ars500/ARS500.h>
#include <Core/ICore.h>
#include <Core/LicenseManager.h>
#include <Core/Platform.h>
#include <Imaging/CursorManager.h>
#include <Imaging/DialogImageOptions.h>
#include <Imaging/Magnifier.h>
#include <Imaging/ReportDlg.h>
#include <Imaging/UTDocument.h>
#include <Imaging/UTLayer.h>
#include <Legacy/MatlabEngine.h>
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <Motion/PanelAxisControl.h>
#include <QAbstractSpinBox>
#include <QBarSet>
#include <QClipboard>
#include <QDesktopServices>
#include <QFileDialog>
#include <QInputDialog>
#include <QLoggingCategory>
#include <QMessageBox>
#include <QProgressDialog>
#include <QToolTip>
#include <QtMath>
#include <Ultrasonic/UTChannel.h>
#include <Ultrasonic/UTDataHeader.h>
#include <Ultrasonic/UltrasonicManager.h>
#include <Waveform//PlotterRuler.h>
#include <Waveform/DataInfoWidget.h>
#include <Waveform/GraphicsAxis.h>
#include <Waveform/GraphicsEditablePolygon.h>
#include <Waveform/PanelAScan.h>
#include <Waveform/UTGraphicsScene.h>
#include <allrising/ThrottleUpdate.h>
#include <allrising/Util.h>
#include <QScreen>
#include <QWindow>
#include <limits>
#include <utils/algorithm.h>

static Q_LOGGING_CATEGORY(debug, "image.cscan")

using namespace QtCharts;

#ifdef QT_CONCURRENT_LIB
#   undef QT_CONCURRENT_LIB
#endif

#ifdef QT_CONCURRENT_LIB
#   include <QtConcurrent>
#   include <QFutureWatcher>
enum FutureType {
    FuturePixmapC,
    FutureImageSliceX,
    FuturePixmapSliceX,
    FutureImageSliceY,
    FuturePixmapSliceY,
};
struct FutureData {
    QVariant data;
    FutureType type;
    int reference;
};
typedef QFutureWatcher<FutureData> Watcher;
typedef QFuture<FutureData> Future;
#endif

using namespace Ars500;

QPointer<QDockWidget> PanelCScan::ascanDataPlotWidget;
QPointer<AScanGraphicsView> PanelCScan::ascanPlot;
QPointer<DataInfoWidget> PanelCScan::info;
QPointer<PanelCScan> PanelCScan::activeCScan;
QList<QPointer<PanelCScan> > PanelCScan::cscanList;
static const int DECIMAL = 4;
static const int DPI = 96;
static const QList<Qt::GlobalColor> cursorColorList{Qt::red, Qt::blue, Qt::green, Qt::yellow, Qt::gray};

static UTDataHeader dataHeader()
{
#ifdef PIPE_DETECT
    return PipeInstrumentDriver::InitDataHeader();
#else
    return UTDriverARS500::InitDataHeader();
#endif
}

class CScanSpinBox : public QDoubleSpinBox
{
public:
    explicit CScanSpinBox(QWidget *parent = nullptr) :
        QDoubleSpinBox(parent)
    {}
    QString textFromValue(double value) const override
    {
        QString text = QDoubleSpinBox::textFromValue(value);
        if (value > 0 && !qFuzzyIsNull(value)) {
            text.prepend("+");
        }
        return text;
    }
};

CScanRenderer::CScanRenderer(QObject *parent) :
    QObject(parent),
    index(-1),
    dirty(false)
{
    qRegisterMetaType<UTImageList>("UTImageList");
    qRegisterMetaType<UTImagePalette>("UTImagePalette");
}

void CScanRenderer::OnImageUpdated(UTImageList newImage)
{
    imageList = newImage;
    UpdateImage();
}

void CScanRenderer::OnImagePaletteUpdated(ColorPalette newPalette)
{
    ARS_CHECK(palette != newPalette);
    palette = newPalette;
    UpdateImage();
}

void CScanRenderer::OnImageIndexChanged(int newIndex)
{
    ARS_CHECK(index != newIndex);
    index = newIndex;
    UpdateImage();
}

void CScanRenderer::renderImage()
{
    emit BusyStatusChanged(true);
    UTImage image = imageList.value(index);

    CDataValue min = 0;
    CDataValue max = 0;
    image.GetMinMax(&min, &max, nullptr, nullptr);
    UTImagePalette imagePalette = palette.GetColorPalette();
    bool maxAuto = palette.isAutoMax();
    bool minAuto = palette.isAutoMin();
    imagePalette.min = minAuto ? min : palette.minValue();
    imagePalette.max = maxAuto ? max : palette.maxValue();
    emit PixmapUpdated(image.ToPixmap(imagePalette, &image), min, max);
    emit BusyStatusChanged(false);
}

void CScanRenderer::UpdateImage()
{
    ThrottleUpdate::call(this, &CScanRenderer::renderImage);
}

PanelCScan::PanelCScan(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PanelCScan)
{
    ui->setupUi(this);
    if (!cscanList.contains(this)) {
        cscanList.append(this);
    }
    UpdateActivePanelCScan(this);
    imageList.clear();

    imageRoot = new QGraphicsPathItem(ui->view->rootItem());
    pixmapItem = new QGraphicsPixmapItem(imageRoot);
    featureSliceX = new QGraphicsPathItem(ui->sliceX->rootItem());
    featureSliceY = new QGraphicsPathItem(ui->sliceY->rootItem());
    pixmapSliceX = new QGraphicsPixmapItem(ui->sliceX->rootItem());
    pixmapSliceY = new QGraphicsPixmapItem(ui->sliceY->rootItem());

    m_cursorManager = new GraphicsCursorManager(this);
    m_cursorManager->createItems(ui->view->rootItem(), ui->sliceX->rootItem(), ui->sliceY->rootItem());
    m_cursorManager->setMenuEnable(false);
    m_cursorManager->setCursorColor(Qt::black);
    m_cursorManager->setCursorVisible(true);
    m_cursorManager->setMovable(false);
    InitUI();

    QChart *chart = ui->histogram->chart();
    series = new QBarSeries(chart);
    chart->legend()->hide();
    chart->addSeries(series);
    axisBar = new QValueAxis(chart);
    axisVisual = new QValueAxis(chart);
    axisPixel = new QValueAxis(chart);
    axisPixel->setTitleText(tr("像素数"));
    chart->addAxis(axisBar, Qt::AlignTop);
    chart->addAxis(axisVisual, Qt::AlignBottom);
    chart->addAxis(axisPixel, Qt::AlignLeft);

    series->attachAxis(axisBar);
    series->attachAxis(chart->axisY());
    axisBar->hide();

    connect(ui->histSizeSlider, &QSlider::valueChanged, this, &PanelCScan::refreshStatistics);
    connect(ui->ratioMode, &QComboBox::currentTextChanged, this, &PanelCScan::refreshStatistics);
    connect(ui->threshold, &QDoubleSpinBox::editingFinished, this, &PanelCScan::refreshStatistics);
    connect(ui->statisticsRefresh, &QToolButton::clicked, this, &PanelCScan::calculateStatistics);
    connect(m_magnifier, &Magnifier::clicked, this, &PanelCScan::onMagnifierClicked);
    ui->waveform->setWaveformFill(false);
    ui->waveform->setSetupLocked(true);
    ui->view->view()->viewport()->installEventFilter(this);
}

PanelCScan::~PanelCScan()
{
    renderThread.quit();
    renderThread.wait();
    delete m_cursorManager;
    m_cursorManager = nullptr;
    if (!m_referCursorList.isEmpty()) {
        qDeleteAll(m_referCursorList);
        m_referCursorList.clear();
    }
    delete ui;
}

/*! 界面初始化 */
void PanelCScan::InitUI()
{
    ui->splitterMain->setCollapsible(0, false);
    ui->splitterMain->setCollapsible(1, false);

    ui->view->axis(Qt::BottomEdge)->setResizePolicy(GraphicsAxis::ResizeKeepScale);
    ui->view->axis(Qt::LeftEdge)->setResizePolicy(GraphicsAxis::ResizeKeepScale);
    ui->view->setRange(Qt::Horizontal, Range(-1e6, 1e6));
    ui->view->setRange(Qt::Vertical, Range(-1e6, 1e6));
    ui->sliceX->setRange(Qt::Horizontal, Range(-1e6, 1e6));
    ui->sliceY->setRange(Qt::Vertical, Range(-1e6, 1e6));

    InitAScanDataPlot();
    InitManagers();
    InitToolBar();
    InitBScan();

    m_magnifier = new Magnifier(this);
    m_magnifier->setVisible(false);

    SetCursorPosition(QPointF(Q_INFINITY, Q_INFINITY));

    ui->view->setRulerUnitName(Qt::LeftEdge | Qt::BottomEdge, tr("mm"));
    ui->view->view()->setZoomEnable(true);

    connect(document(), &UTDocument::FileOpened, this, &PanelCScan::OnDocumentOpen);
    connect(document(), &UTDocument::originChanged, this, &PanelCScan::setOrigin);

    connect(ui->view, &AbstractGraphicsView::mouseMoved, this, &PanelCScan::OnMouseMoved);
    connect(ui->view, &AbstractGraphicsView::contextMenuRequest, this, &PanelCScan::OnViewContextMenuRequest);
    connect(ui->view, &AbstractGraphicsView::rubberBandChanged, this, &PanelCScan::onRubberBandChanged);

    ui->palette->setAutoLimitSupported(true);
    ui->palette->addAction(ui->actionCopyColorToOthers);
    connect(ui->palette, &ColorPaletteView::colorPaletteChanged, this, &PanelCScan::RefreshUTImages);
    connect(ui->paletteSlice, &ColorPaletteView::colorPaletteChanged, this, &PanelCScan::refreshSliceData);
    connect(ui->defectRecord, &QAbstractButton::clicked, this, &PanelCScan::OnSelectRectDefect);
    connect(ui->treeView, &DefectTreeView::DefectRectUpdated, this, [this](QRectF rect) {
        ui->view->fitInViewAnimated(rect, Qt::KeepAspectRatio);
    });
    connect(ui->treeView, &DefectTreeView::RequestLocate, this, &PanelCScan::locateTo);

    connect(ui->view, &AbstractGraphicsView::mouseDoubleClicked, this, &PanelCScan::OnViewDoubleClicked);
    connect(ui->sliceX, &AbstractGraphicsView::mouseDoubleClicked, this, &PanelCScan::OnViewDoubleClicked);
    connect(ui->sliceY, &AbstractGraphicsView::mouseDoubleClicked, this, &PanelCScan::OnViewDoubleClicked);

#ifdef PIPE_DETECT
    ui->actionSliceDisable->setChecked(true);
    ui->actionSliceDisable->setVisible(false);
    ui->actionSliceFeature->setVisible(false);
    ui->actionSliceImage->setVisible(false);
    ui->actionSliceSync->setVisible(false);
    ui->actionSliceSyncOffset->setVisible(false);
    ui->actionHideSliceCursor->setVisible(false);
    ui->actionWaveformRuler->setVisible(false);
#endif

    ui->actionViewImage->setChecked(true);
    ui->actionViewProbe->setChecked(true);
    ui->actionViewDefect->setChecked(true);

    ui->actionAnnotationLineItem->setVisible(QApplication::arguments().contains("-appTest"));
}

/*! 初始化各控制显示选项的manager */
void PanelCScan::InitManagers()
{
    defectManager = new DefectManager(imageRoot, this);
    defectManager->AddToScene(ui->view->scene());

    probeIndicatorManager = new ProbeIndicatorManager(ui->view->rootItem(), this);
    probeIndicatorManager->AddToScene(ui->view->scene());

    polygonSelection = new GraphicsEditablePolygon(ui->view->rootItem());
    polygonSelection->setPen(QPen(Qt::black, 0));
    connect(polygonSelection, &GraphicsEditablePolygon::editableChanged, this, &PanelCScan::refreshStatistics);
    connect(polygonSelection, &GraphicsEditablePolygon::polygonChanged, this, &PanelCScan::refreshStatistics);

    pathItem = new QGraphicsPathItem(ui->view->rootItem());
    annotationManager = new AnnotationManager(pathItem, this);
}

void PanelCScan::InitToolBar()
{
    QToolBar *toolbar = ui->toolbar;

    connect(ui->actionZoomIn, &QAction::triggered, ui->view, &AbstractGraphicsView::onZoomIn);
    connect(ui->actionZoomOut, &QAction::triggered, ui->view, &AbstractGraphicsView::onZoomOut);
    connect(ui->actionZoomFit, &QAction::triggered, this, &PanelCScan::ResetView);
    connect(ui->actionCursorHide, &QAction::triggered, this, &PanelCScan::OnCursorHide);

    auto *sliceGroup = new QActionGroup(this);
    sliceGroup->addAction(ui->actionSliceDisable);
    sliceGroup->addAction(ui->actionSliceImage);
    sliceGroup->addAction(ui->actionSliceFeature);
    ui->actionSliceImage->setChecked(true);

    gateCombobox = new QComboBox(this);
    gateCombobox->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    gain = new CScanSpinBox(this);
    gain->setDecimals(2);
    gain->setSingleStep(0.5);
    gain->setRange(-12, 12);
    gain->setSuffix(tr(" dB"));
    connect(gain, &QAbstractSpinBox::editingFinished, this, &PanelCScan::onGainEditingFinished);
    toolbar->addWidget(gain);

    int numGate = dataHeader().GetNumGates();
    for (int index = 0; index < numGate; ++index) {
        gateCombobox->addItem( tr("闸门 %1").arg(index + 1) );
    }

    featureCombobox = new QComboBox(this);
    featureCombobox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    connect(gateCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PanelCScan::OnGateIndexChanged);
    connect(featureCombobox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PanelCScan::OnCDataIndexChanged);
    gateCombobox->setCurrentIndex(-1);
    featureCombobox->setCurrentText({});

    QAction *firstAction = toolbar->actions().constFirst();
    toolbar->insertWidget(firstAction, gateCombobox);
    toolbar->insertWidget(firstAction, featureCombobox);
    ui->actionCDataStatistic->setChecked(false);
    ui->actionRecalcFeatures->setChecked(false);

    renderer.moveToThread(&renderThread);
    renderThread.start();
    connect(&renderer, &CScanRenderer::PixmapUpdated, this, &PanelCScan::onPixmapUpdated);
    QAction *busy = toolbar->addWidget(new QLabel(tr("正在渲染")));
    busy->setVisible(false);
#ifndef ARS_FINAL_RELEASE
    connect(&renderer, &CScanRenderer::BusyStatusChanged, busy, &QAction::setVisible);
#endif
}

/*! 初始化B扫界面 */
void PanelCScan::InitBScan()
{
    ui->waveform->setCursorEnabled(true);
    ui->sliceX->axis(Qt::BottomEdge)->setResizePolicy(GraphicsAxis::ResizeKeepSelection);
    ui->sliceX->axis(Qt::LeftEdge)->setResizePolicy(GraphicsAxis::ResizeKeepSelection);
    ui->sliceY->axis(Qt::BottomEdge)->setResizePolicy(GraphicsAxis::ResizeKeepSelection);
    ui->sliceY->axis(Qt::LeftEdge)->setResizePolicy(GraphicsAxis::ResizeKeepSelection);

    connect(ui->view->axis(Qt::BottomEdge), &GraphicsAxis::centerChanged, this, &PanelCScan::SyncPanels, Qt::QueuedConnection);
    connect(ui->view->axis(Qt::LeftEdge), &GraphicsAxis::centerChanged, this, &PanelCScan::SyncPanels, Qt::QueuedConnection);
    connect(ui->view->axis(Qt::BottomEdge), &GraphicsAxis::scaleChanged, this, &PanelCScan::SyncPanels, Qt::QueuedConnection);
    connect(ui->view->axis(Qt::LeftEdge), &GraphicsAxis::scaleChanged, this, &PanelCScan::SyncPanels, Qt::QueuedConnection);
    connect(ui->view, &AbstractGraphicsView::selectionChanged, this, &PanelCScan::OnViewSelectionChanged);
    connect(ui->view, &AbstractGraphicsView::rangeChanged, this, &PanelCScan::OnViewRangeChanged);

    ui->sliceX->view()->setDragMode(QGraphicsView::NoDrag);
    ui->sliceX->setRulerPos(Qt::RightEdge);
    ui->sliceX->setRulerUnitName(Qt::LeftEdge | Qt::BottomEdge, "");

    ui->sliceY->view()->setDragMode(QGraphicsView::NoDrag);
    ui->sliceY->setRulerPos(Qt::BottomEdge);
    ui->sliceY->setRulerUnitName(Qt::LeftEdge | Qt::BottomEdge, "");

    ui->view->setRulerPos(Qt::BottomEdge | Qt::RightEdge);
    ui->waveform->setRulerPos(Qt::Edges());

    ui->splitterH1->setCollapsible(0, false);
    ui->splitterH1->setCollapsible(1, false);
    ui->splitterH2->setCollapsible(0, false);
    ui->splitterH2->setCollapsible(1, false);
    ui->splitterV->setCollapsible(0, false);
    ui->splitterV->setCollapsible(1, false);
    ui->splitterH1->setStretchFactor(1, 255);
    ui->splitterH1->setStretchFactor(0, 0);
    ui->splitterH2->setStretchFactor(1, 255);
    ui->splitterH2->setStretchFactor(0, 0);
    ui->splitterV->setStretchFactor(0, 1);
    ui->splitterV->setStretchFactor(1, 255);
    ui->splitterH1->setSizes({0, 1});
    ui->splitterH2->setSizes({0, 1});

    QPen pen;
    pen.setCosmetic(true);
    featureSliceX->setPen(pen);
    featureSliceY->setPen(pen);
    ui->sliceX->view()->setRenderHint(QPainter::Antialiasing, false);
    ui->sliceY->view()->setRenderHint(QPainter::Antialiasing, false);
}

/*! 闸门改变 */
void PanelCScan::OnGateIndexChanged(int index)
{
    QString currentData = featureCombobox->currentText();
    featureCombobox->clear();
    QVariantMap map = dataHeader().GetGateDescription(index);
    int row = 0;
    for (auto iter = map.cbegin(); iter != map.cend(); ++iter) {
        featureCombobox->insertItem(row, iter.key());
        QJsonObject obj = iter.value().toJsonObject();
        featureCombobox->setItemData(row, obj["GateType"].toInt(), Qt::UserRole);
        featureCombobox->setItemData(row, obj["DataType"].toInt(), Qt::UserRole + 1);
        row++;
    }
    featureCombobox->setCurrentText(currentData);
}

/*! 成像参数改变 */
void PanelCScan::OnImageOptionsChanged(const ImageOptions &options)
{
    transform = options;
    QRectF rect = options.sourceRect();

    ui->view->setRulerSelection(Qt::Horizontal, Range(rect.left(), rect.right()));
    ui->view->setRulerSelection(Qt::Vertical, Range(rect.top(), rect.bottom()));

    ui->view->axis(Qt::BottomEdge)->setUserScale(transform.scale().width());
    ui->view->axis(Qt::LeftEdge)->setUserScale(transform.scale().height());

    QSizeF gridSize = transform.gridSize();
    QRectF sourceRect = transform.sourceRect();
    sourceRect.moveTo(sourceRect.left() - gridSize.width() / 2, sourceRect.top() - gridSize.height() / 2);
    ImageOptions movedTransform(sourceRect, gridSize);
    imageRoot->setTransform(movedTransform.InverseTransform());

    auto pathItemTransform = QTransform::fromScale(transform.scale().width(), transform.scale().height());
    pathItem->setTransform(pathItemTransform);

    ui->actionViewImage->setChecked(true);
    on_actionViewImage_toggled(true);
    // 移动视口
    ResetView();
    updateTransform();
}

void PanelCScan::SetLayerID(int id)
{
    if (id == m_layerId) {
        return;
    }
    UTLayer *layer = document()->GetLayerByID(m_layerId);
    if (layer != nullptr) {
        disconnect(layer, nullptr, this, nullptr);
        disconnect(this, nullptr, layer, nullptr);
    }
    layer = document()->GetLayerByID(id);
    if (layer != nullptr) {
        m_layerId = id;
        connect(layer, &UTLayer::ImagesUpdated, this, &PanelCScan::OnImagesUpdate);
        connect(layer, &UTLayer::MatrixDataUpdated, this, &PanelCScan::refreshImages);
        connect(layer, &UTLayer::ImageOptionsChanged, this, &PanelCScan::OnImageOptionsChanged);
        OnImageOptionsChanged(layer->GetImageOptions());
        OnImagesUpdate(layer->imageList());
        defectManager->SetLayerID(m_layerId);
        probeIndicatorManager->SetLayerID(m_layerId);
        // 更新缺陷列表model
        ui->treeView->SetModel(m_layerId);
    } else {
        qWarning() << "PanelCScan::SetLayerID the new layerid is not valid";
    }
}

void PanelCScan::OnImagesUpdate(UTImageList list)
{
    imageList = list;
    if (currentImage().IsValid() && !qFuzzyIsNull(gain->value()) && gain->isEnabled()) {
        UTImage image = currentImage().Copy().Multiply(UltrasonicManager::decibelToAmp(gain->value(), 1));
        int featureIndex = currentFeatureIndex();
        if (uint(featureIndex) < uint(imageList.count())) {
            imageList.replace(featureIndex, image);
        }
    }
    RefreshUTImages();
}

void PanelCScan::onPixmapUpdated(const QPixmap &pixmap, qreal min, qreal max)
{
    pixmapItem->setPixmap(pixmap);
    ui->palette->setImageRange(min, max);
}

/*! 设置原点 */
void PanelCScan::OnSetOrigin()
{
    QPointF scenePos = ui->view->mapScenePosFromGlobal(menuGlobalPos);
    QVector3D vector = document()->origin();
    vector.setX(scenePos.x());
    vector.setY(scenePos.y());
    document()->setOrigin(vector);
}

/*! 取消原点 */
void PanelCScan::OnCancelOrigin()
{
    QVector3D vector = document()->origin();
    vector.setX(0);
    vector.setY(0);
    document()->setOrigin(vector);
}

/*! 保存C扫原始图像(携带标尺，调色板) */
void PanelCScan::OnCScanPixmapSave()
{
    QImage image = ExportFullsizeImage();
    QString defaultName(QDateTime::currentDateTime().toString("yyMMdd-hhmmss"));
    QString fileName = QFileDialog::getSaveFileName(this, tr("C扫图像保存"), defaultName, tr("PNG File(*.png);;JPG File(*.jpg);;BMP File(*.bmp)"));
    if (fileName.isEmpty()) {
        return;
    }
    image.save(fileName, nullptr, 100);
}

/*! 复制C扫图像到剪切板 */
void PanelCScan::CopyCScanPixmap()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setImage(ExportFullsizeImage());
}

void PanelCScan::OnSaveAllPixmap()
{
    FullImageOption option;
    DialogImageABCOptions dialog;
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    option.widthImageA = dialog.GetWidthImageA();
    option.widBlank = dialog.isBlank() ? 5 : 0;
    option.timeStampFlag = dialog.isTimeStamp() ? true : false;

    QImage image = ExportABCFullsizeImage(option);

    QString defaultName(QDateTime::currentDateTime().toString("yyMMdd-hhmmss"));
    QString fileName = QFileDialog::getSaveFileName(this, tr("ABC扫图像保存"),
                        defaultName, tr("PNG File(*.png);;JPG File(*.jpg);;BMP File(*.bmp)"));
    if (fileName.isEmpty()) {
        return;
    }
    image.save(fileName, nullptr, 100);
//    QDesktopServices::openUrl(QUrl(fileName));
}

QImage PanelCScan::ExportABCFullsizeImage(FullImageOption fio)
{
    QImage CScanImage = renderCurrentImage();

    static const qreal ratio = 0.015;

    if (fio.autoRulerSize) {
        int maxDimension = qMax(CScanImage.height(), CScanImage.width());
        fio.widthRuler = qMax<int>(fio.widthRuler, maxDimension * ratio);
        fio.widthPalette = qMax<int>(fio.widthPalette, maxDimension * ratio);
        fio.widthPaletteRuler = qMax<int>(fio.widthPaletteRuler, maxDimension * ratio);
    }

    QRect rectA(QPoint(0, 0), QSize(fio.widthImageA, fio.widthImageA));
    QPoint blank(fio.widBlank, fio.widBlank);
    QRect rectC(rectA.bottomRight() + QPoint(blank.x(), blank.y()), CScanImage.size());
    QRect rectHorizontalB(rectA.topRight() + QPoint(blank.x(), 0), rectC.topRight() + QPoint(0, -blank.y()));
    QRect rectVerticalB(rectA.bottomLeft() + QPoint(0, blank.y()), rectC.bottomLeft() + QPoint(-blank.x(), 0));

    QRect rectHorizontalRulerB(rectVerticalB.bottomLeft(), QSize(fio.widthImageA, fio.widthRuler));
    QRect rectVerticalRulerB(rectHorizontalB.topRight(), QSize(fio.widthRuler, fio.widthImageA));
    QRect rectVerticalPaletteB(rectVerticalRulerB.topRight(), QSize(fio.widthPalette, fio.widthImageA));
    QRect rectVerticalPaletteRulerB(rectVerticalPaletteB.topRight(), QSize(fio.widthPaletteRuler, fio.widthImageA));

    QRect rectHorizontalRulerC(rectC.bottomLeft(), QSize(CScanImage.width(), fio.widthRuler));
    QRect rectVerticalRulerC(rectC.topRight(), QSize(fio.widthRuler, CScanImage.height()));
    QRect rectVerticalPaletteC(rectVerticalRulerC.topRight(), QSize(fio.widthPalette, CScanImage.height()));
    QRect rectVerticalPaletteRulerC(rectVerticalPaletteC.topRight(), QSize(fio.widthPaletteRuler, CScanImage.height()));

    QRect imageRect;
    QList<QRect> parts = QList<QRect>() << rectA << rectC << rectHorizontalB << rectVerticalB
                    << rectHorizontalRulerB << rectVerticalRulerB << rectVerticalPaletteB
                    << rectVerticalPaletteRulerB << rectHorizontalRulerC << rectVerticalRulerC
                    << rectVerticalPaletteC << rectVerticalPaletteRulerC;
    for (const auto rect: parts) {
        imageRect = rect.united(imageRect);
    }

    QImage image;
    if (fio.timeStampFlag) {                                                         //是否需要插入时间戳
        image = QImage(imageRect.width(), imageRect.height() + fio.widTimeStamp, QImage::Format_ARGB32_Premultiplied);
    } else {
        image = QImage(imageRect.size(), QImage::Format_ARGB32_Premultiplied);
    }

    image.fill(Qt::white);
    QPainter painter(&image);
    // 获取视块的实际Rect
    QRectF sourceRect = transform.sourceRect();                                 //获取原始坐标系
    sourceRect.translate(-document()->origin().toPointF());
    // 绘制C扫横向/下侧标尺
    PlotterRuler plotter;
    plotter.setUnitName(tr("mm"));
    painter.resetTransform();
    plotter.drawRuler(&painter, Range(sourceRect, Qt::Horizontal), rectHorizontalRulerC,
                      rectHorizontalRulerC, Qt::TopEdge, false);
    // 绘制C扫纵向/右侧标尺
    painter.resetTransform();
    plotter.drawRuler(&painter, Range(sourceRect, Qt::Vertical), rectVerticalRulerC,
                      rectVerticalRulerC, Qt::LeftEdge, true);
    // 绘制C扫调色板
    painter.resetTransform();
    ColorPalette palette = ui->palette->colorPalette();
    palette.paint(&painter, rectVerticalPaletteC, Qt::Vertical, true);
    // 绘制C扫调色板标尺
    Range paletteRange(palette.minValue(), palette.maxValue());
    plotter.setUnitName("");
    painter.resetTransform();
    plotter.drawRuler(&painter, paletteRange, rectVerticalPaletteRulerC,
                      rectVerticalPaletteRulerC, Qt::LeftEdge, true);

    // 绘制C扫图像
    painter.resetTransform();
    painter.translate(rectC.x(), rectC.y());
    painter.translate(rectC.width() * ui->view->isReversed(Qt::Horizontal),
                      rectC.height() * ui->view->isReversed(Qt::Vertical));
    painter.scale(ui->view->isReversed(Qt::Horizontal) ? -1 : 1, ui->view->isReversed(Qt::Vertical) ? -1 : 1);
    painter.drawImage(0, 0, CScanImage);

    // 绘制B扫水平图像
    {
        painter.resetTransform();
        QImage imageBX = sliceImageRendered(Qt::Horizontal).scaled(rectHorizontalB.width(), rectHorizontalB.height());
        painter.drawImage(rectHorizontalB, imageBX);

        // 绘制B扫纵向/右侧标尺
        plotter.setUnitName(tr(""));
        painter.resetTransform();
        plotter.drawRuler(&painter, ui->sliceY->axis(Qt::BottomEdge)->range(),
                          rectVerticalRulerB, rectVerticalRulerB, Qt::LeftEdge, false);
        // 绘制B扫纵向/右侧调色板
        painter.resetTransform();
        ColorPalette paletteBY = ui->paletteSlice->colorPalette();
        paletteBY.paint(&painter, rectVerticalPaletteB, Qt::Vertical, true);
        // 绘制B扫纵向/右侧调色板标尺
        Range paletteRangeBY(paletteBY.minValue(), paletteBY.maxValue());
        painter.resetTransform();
        plotter.drawRuler(&painter, paletteRangeBY, rectVerticalPaletteRulerB,
                          rectVerticalPaletteRulerB, Qt::LeftEdge, true);
    }

    // 绘制B扫垂直图像
    {
        painter.resetTransform();
        painter.translate(rectVerticalB.x(), rectVerticalB.y());
        painter.translate(rectVerticalB.width() * ui->view->isReversed(Qt::Horizontal),
                          rectVerticalB.height() * ui->view->isReversed(Qt::Vertical));
        painter.scale(ui->view->isReversed(Qt::Horizontal) ? -1 : 1, ui->view->isReversed(Qt::Vertical) ? -1 : 1);
        QImage imageBY = sliceImageRendered(Qt::Vertical).scaled(rectVerticalB.width(), rectVerticalB.height());
        painter.drawImage(0, 0, imageBY);

        // 绘制B扫横向/下侧标尺
        plotter.setUnitName(tr(""));
        painter.resetTransform();
        plotter.drawRuler(&painter, ui->sliceY->axis(Qt::BottomEdge)->range(),
                          rectHorizontalRulerB, rectHorizontalRulerB, Qt::TopEdge, false);
    }

    // 绘制A扫图像
    if (rectA.isValid()) {
        painter.resetTransform();
        Range timeRange = ui->waveform->axis(Qt::BottomEdge)->selection();
        QRectF sceneRect = Range::toRect(timeRange, Range(-100, 0));
        painter.save();
        painter.scale(rectA.width() / sceneRect.width(), rectA.height() / sceneRect.height());
        painter.translate(-sceneRect.topLeft());

        ui->waveform->view()->drawBackground(&painter, sceneRect);
        painter.restore();
        ui->waveform->view()->scene()->render(&painter, rectA, sceneRect, Qt::IgnoreAspectRatio);
//        qCDebug(debug) << "ExportFullSize: rendering AScan at " << rectA;
    }

    // 绘制截图时间
    if (fio.timeStampFlag) {
        QString text = QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate);
        QFont font;
        font.setPixelSize(fio.widTimeStamp);
        painter.setFont(font);
        painter.setPen(Qt::black);
        int length = painter.fontMetrics().width(text);
        QRect rect(imageRect.width() - length, imageRect.height(), length, fio.widTimeStamp);
        painter.resetTransform();
        painter.drawText(rect, text);
    }
    painter.end();

    image.setDotsPerMeterX( 1 / transform.gridSize().width() );
    image.setDotsPerMeterY( 1 / transform.gridSize().height() );

    return image;
}

/*! 导出携带标尺、调色板与原尺寸图像的image */
QImage PanelCScan::ExportFullsizeImage(bool timeFlag)
{
    FullImageOption options;
    options.widthImageA = 0;
    options.timeStampFlag = timeFlag;
    return ExportABCFullsizeImage(options);
}

UTLayer *PanelCScan::GetLayer() const
{
    return document()->GetLayerByID(m_layerId);
}

bool PanelCScan::HasDefectTreeView(DefectTreeView *view)
{
    return ui->treeView == view;
}

UTImage PanelCScan::sliceImage(Qt::Orientation orientation, int index) const
{
    UTDataSegment segment = dataSegment(orientation, index);
    if (segment.isEmpty()) {
        return UTImage();
    }
    bool sync = ui->actionSliceSync->isChecked();
    int offset = sync ? sliceOffset : 0;
    return UTImage::FromAData(segment, orientation, offset, sync);
}

QImage PanelCScan::sliceImageRendered(Qt::Orientation orientation, int index) const
{
    UTImage image = sliceImage(orientation, index);
    if (!image.IsValid()) {
        return QImage();
    }
    UTImagePalette palette = ui->paletteSlice->colorPalette().GetColorPalette();
    palette.min *= 0.0f;
    palette.max *= 10.24f;
    return image.ToImage(palette);
}

void PanelCScan::sliceFeatureRendered(Qt::Orientation orientation)
{
    UTDataSegment segment = dataSegment(orientation, -1);
    if (segment.isEmpty()) {
        return;
    }
    if (orientation & Qt::Horizontal) {
        featureSliceX->setPath(sliceFeaturePath(segment));
        updateSliceBrush();
    } else {
        featureSliceY->setPath(sliceFeaturePath(segment));
        updateSliceBrush();
    }
}

QPainterPath PanelCScan::sliceFeaturePath(const UTDataSegment &segment)
{
    QPainterPath path;
    const int featureIndex = currentFeatureIndex();
    for (int index = 0; index < segment.count(); ++index) {
        const UTDataFrame &frame = segment[index];
        path.lineTo(index, frame.FeatureValue(featureIndex));
    }
    path.lineTo(segment.count() - 1, 0);
    path.lineTo(0, 0);
    return path;
}

QBrush PanelCScan::sliceFeatureBrush(const UTDataSegment &segment)
{
    const int featureIndex = currentFeatureIndex();
    const auto palette = ui->paletteSlice->colorPalette();
    QImage image(segment.count(), 1, QImage::Format_ARGB32_Premultiplied);
    for (int index = 0; index < segment.count(); ++index) {
        const UTDataFrame &frame = segment[index];
        QColor color = palette.colorAt(palette.mapFromRange(frame.FeatureValue(featureIndex)));
        image.setPixelColor(index, 0, color);
    }
    return image;
}

void PanelCScan::updateSliceBrush()
{
    if (ui->actionSliceBrush->isChecked()) {
        UTDataSegment segmentH = dataSegment(Qt::Horizontal, -1);
        UTDataSegment segmentV = dataSegment(Qt::Vertical, -1);
        if (segmentH.isEmpty() || segmentV.isEmpty()) {
            return;
        }
        featureSliceX->setBrush(sliceFeatureBrush(segmentH));
        featureSliceY->setBrush(sliceFeatureBrush(segmentV));
    } else {
        featureSliceX->setBrush({});
        featureSliceY->setBrush({});
    }
}

QJsonValue PanelCScan::saveSettings() const
{
    QJsonObject json;
    json["splitterMain"] = convertToJson(ui->splitterMain->sizes());
    json["splitterV"] = convertToJson(ui->splitterV->sizes());
    json["splitterH"] = convertToJson(ui->splitterH1->sizes());

    QJsonObject actions;
    for (auto *action: savedActions()) {
        actions[action->objectName()] = action->isChecked();
    }
    json["actions"] = actions;

    json["palette"] = ui->palette->colorPalette().toJson();
    json["paletteSlice"] = ui->paletteSlice->colorPalette().toJson();
    json["layerId"] = convertToJson<int>(m_layerId);
    json["featureIndex"] = convertToJson<int>(currentFeatureIndex());
    return json;
}

void PanelCScan::loadSettings(const QJsonValue &value)
{
    const QJsonObject json = value.toObject();
    ui->splitterMain->setSizes(convertFromJson<QList<int> >(json["splitterMain"]));
    ui->splitterV->setSizes(convertFromJson<QList<int> >(json["splitterV"]));
    ui->splitterH1->setSizes(convertFromJson<QList<int> >(json["splitterH"]));

    QJsonObject actions = json["actions"].toObject();
    for (auto *action: savedActions()) {
        if (actions.contains(action->objectName())) {
            action->setChecked(actions[action->objectName()].toBool());
        }
    }

    ui->palette->setColorPalette(ColorPalette::fromJson(json["palette"]));
    ui->paletteSlice->setColorPalette(ColorPalette::fromJson(json["paletteSlice"]));
    if (convertFromJson<int>(json["layerId"]) > 0) {
        SetLayerID(convertFromJson<int>(json["layerId"]));
    }
    setCurrentFeatureIndex(convertFromJson<int>(json["featureIndex"]));
}

QList<QAction *> PanelCScan::savedActions() const
{
    return {
        ui->actionSliceDisable,
        ui->actionSliceImage,
        ui->actionSliceFeature,
        ui->actionCursorFollow,
        ui->actionColorPalette,
        ui->actionZoomFill,
        ui->actionViewProbe,
        ui->actionViewDefect
    };
}

void PanelCScan::setDataFrame(const UTDataFrame &frame)
{
    ui->waveform->setMonitorData(frame);
    ui->waveform->setGatesVisible(!frame.isEmpty());
    if (ascanDataPlotWidget->isVisible() && this == activeCScan) {              // 查看A扫波形界面只显示活动窗口的数据
        info->setDataFrame(frame);
        ascanPlot->setMonitorData(frame);
    }
    updateRulerTimebase();
}

/*! 将机械轴移动到指定的rootItem场景中的位置 */
void PanelCScan::locateTo(const QPointF &rootItemPos)
{
    UTLayer *layer = GetLayer();
    if (!layer) {
        return;
    }
    QPoint gridPos = transform.MapToGrid(rootItemPos);
    UTDataFrame frame = layer->dataFrameAt(gridPos);
    if (this != activeCScan) {
        return;
    }
    UTPose destPose = frame.Pose;
    MotionAxisList axes = document()->scanPlan()->associatedAxes();
    if (!destPose.isEmpty()) {
        PanelAxisControl::setPose(destPose, axes);
    } else {
        MotionPose localPos = MotionManager::endPoint(0)->currentPos();
        int imageAxisX = layer->GetImageOptions().axisX();
        int imageAxisY = layer->GetImageOptions().axisY();
        if (imageAxisX < 0 || imageAxisY < 0) {
            return;
        }
        localPos[imageAxisX] = rootItemPos.x();
        localPos[imageAxisY] = rootItemPos.y();
        PanelAxisControl::setPose(localPos, axes);
    }
    if (ui->actionMoveModel->isChecked()) {
        PanelAxisControl::instance()->startMove();
    }
}

/*! 将光标位置设置为场景坐标 scenePos ，将一个方向的光标位置设置为 inf 可以隐藏该方向的光标。
 * 光标会被同步设置到所有C扫描窗口上 */
void PanelCScan::SetCursorPosition(const QPointF &scenePos)
{
    if (cursorPos.x() == scenePos.x() && cursorPos.y() == scenePos.y()) {       /* 由于使用了inf作为坐标值，使用QPointF::operator==将导致判断错误，从而栈溢出 */
        return;
    }
    // 更新标尺上上显示位置的label
    bool validX = qIsFinite(scenePos.x());
    bool validY = qIsFinite(scenePos.y());
    Qt::Orientations updateSlice;

    UTImage image = currentImage();
    UTLayer *layer = GetLayer();
    if (layer && image.IsValid()) {
        QPoint requestedPos = transform.MapToGrid(ui->view->mapFromScene(cursorPos));
        QPoint pos = transform.MapToGrid(ui->view->mapFromScene(scenePos));
        if (requestedPos != pos && validX && validY && isVisible()) {
            setDataFrame(layer->dataFrameAt(pos));
        }
        if (!ui->actionSliceDisable->isChecked()) {
            if (requestedPos.x() != pos.x() && validX) {
                updateSlice |= Qt::Vertical;
            }
            if (requestedPos.y() != pos.y() && validY) {
                updateSlice |= Qt::Horizontal;
            }
        }
    }

    cursorPos = scenePos;
    m_cursorManager->setCursorPosition(scenePos);

    updateSliceData(updateSlice);

    ui->view->axis(Qt::BottomEdge)->setCursorPos(scenePos.x());
    ui->view->axis(Qt::LeftEdge)->setCursorPos(scenePos.y());
    ui->view->update();

    foreach (PanelCScan *panel, cscanList) {
        if (panel != this) {
            panel->SetCursorPosition(scenePos);
        }
    }

    refreshStatistics();
}

void PanelCScan::refreshImages(const QHash<QPoint, UTDataFrame> &segment)
{
    if (!isVisible() || !ui->actionSliceImage->isChecked() || !ui->actionSliceFeature->isChecked()) {
        return;
    }

    Qt::Orientations orientation;
    const QPoint cursor = transform.MapToGrid(cursorPos.x(), -cursorPos.y());
    for (auto it = segment.begin(); it != segment.end(); ++it) {
        const QPoint &point = it.key();
        if (cursor.y() == point.y()) {
            orientation.setFlag(Qt::Horizontal);
        }
        if (cursor.x() == point.x()) {
            orientation.setFlag(Qt::Vertical);
        }
        if (cursor == point) {
            setDataFrame(segment[cursor]);
            break;                                                              // 刷新A扫并跳出，此时必定满足 orientation == Qt::Horizontal | Qt::Vertical
        }
    }

    updateSliceData(orientation);
}

/*! 渲染全尺寸的当前C扫图像 */
QImage PanelCScan::renderCurrentImage()
{
    UTImage utimage = currentImage();
    QImage image = utimage.ToImage(ui->palette->colorPalette().GetColorPalette(), &utimage);
    image.setDotsPerMeterX( 1 / transform.gridSize().width() );
    image.setDotsPerMeterY( 1 / transform.gridSize().height() );
    return image;
}

UTDataSegment PanelCScan::dataSegment(Qt::Orientation orientation, int index) const
{
    UTLayer *layer = GetLayer();
    if (!layer) {
        return {};
    }
    if (index < 0) {
        QPoint gridPos = transform.MapToGrid(ui->view->mapFromScene(cursorPos));
        index = (orientation == Qt::Vertical) ? gridPos.x() : gridPos.y();
    }
    UTDataSegment segment = layer->sliceAt(orientation, index);
    return segment;
}

void PanelCScan::updateSliceData(Qt::Orientations orientations)
{
    bool isImage = ui->actionSliceImage->isChecked();
    bool isFeature = ui->actionSliceFeature->isChecked();
    if (orientations & Qt::Horizontal) {
        if (isImage) {
            QImage image = sliceImageRendered(Qt::Horizontal);
            pixmapSliceX->setPixmap(QPixmap::fromImage(image));
        } else if (isFeature) {
            sliceFeatureRendered(Qt::Horizontal);
        }
    }
    if (orientations & Qt::Vertical) {
        if (isImage) {
            QImage image = sliceImageRendered(Qt::Vertical);
            pixmapSliceY->setPixmap(QPixmap::fromImage(image));
        } else if (isFeature) {
            sliceFeatureRendered(Qt::Vertical);
        }
    }
}

/*! 接收platform的原点改变的信号的槽函数 */
void PanelCScan::setOrigin(const QVector3D &origin3D)
{
//    qDebug() << "PanelCScan::OnOriginChanged" << origin3D;
    ui->view->setOrigin(QPointF(origin3D.x(), origin3D.y()));
    ui->sliceX->setOrigin(QPointF(origin3D.x(), 0));
    ui->sliceY->setOrigin(QPointF(0, origin3D.y()));
    refreshStatistics();

    UTLayer *layer = GetLayer();
    if (layer && layer->GetModel()) {
        layer->GetModel()->setOrigin(origin3D.toVector2D());
    }
}

/*! 重置视图 */
void PanelCScan::ResetView()
{
    QRectF source = transform.sourceRect();
//    QRectF rect = source.marginsAdded( QMarginsF(source.width() * 0.1, source.height() * 0.1,
//        source.width() * 0.1, source.height() * 0.1) );
    ui->view->fitInViewAnimated(source, Qt::KeepAspectRatio);
}

void PanelCScan::OnLocate()
{
    if (!qIsFinite(cursorPos) || QLineF(ui->view->mapToGlobalFromScene(cursorPos), menuGlobalPos).length() > 10) {
        locateTo(ui->view->mapScenePosFromGlobal(menuGlobalPos));   //global -> item
    } else {
        locateTo(ui->view->rootItem()->mapFromScene(cursorPos));    //scene -> item
    }
}

/*! C扫数据统计 */
void PanelCScan::OnCScanDataStatistic()
{
    refreshStatistics();
    ui->actionCDataStatistic->setChecked(true);
}

/*! 缺陷识别 */
void PanelCScan::OnSelectRectDefect()
{
    UTImage image = currentImage();
    if (!image.IsValid()) {
        QMessageBox::warning(this, QString(), tr("当前C扫图片无效"));
        return;
    }

    // 找出通道过滤中第一个使用的通道，如果通道都没被选中，使用第一个通道
    int channel = qMax(transform.channels().first(), 0);

    CScanDefectParameter defectParameter(channel);
    defectParameter.SetInfo(UTDefectProcessorOptions::FromJson(platform()->Get("DefectParameter")));

    if (defectParameter.exec() == QDialog::Accepted) {
        UTDefectProcessorOptions options = defectParameter.GetOptions();
        platform()->Set("DefectParameter", options.ToJson());
        UpdateDefectList(image, options);
    }
}

/*! 查看A扫波形 */
void PanelCScan::OnLookAtAData()
{
    UTLayer *layer = GetLayer();
    if (!layer) {
        return;
    }
    ascanDataPlotWidget->show();
    QPointF scenePos = ui->view->mapScenePosFromGlobal(menuGlobalPos);
    QPoint gridPos = transform.MapToGrid(scenePos);
    setDataFrame(layer->dataFrameAt(gridPos));
}

/*! 放大到这里 */
void PanelCScan::OnZoomIn()
{
    ui->view->fitInViewAnimated(selectRect, Qt::KeepAspectRatio);
}

/*! 对选中的区间进行采样仿形 */
void PanelCScan::OnSampleAndShape()
{
    UTStreamFile file(document()->GetCurrentFileName());
    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(debug) << "Error opening file" << file.fileName() << ":" << file.errorString();
        return;
    }
    QFile csvFile(QString("SampleAndShape.csv"));
    bool ok;                                                                    /* 处理取消指令　*/
    int col = QInputDialog::getInt(nullptr, QString(), "采样点列数：", 0, 0, 10000, 1, &ok);
    if (col <= 3 || !ok) {                                                      /* 未采点或者取消 */
        qCDebug(debug) << "Sample and shape: cannot sample" << col << "points";
        return;
    }
    if (!(csvFile.open(QIODevice::WriteOnly))) {
        return qWarning("Sample and shaple: failed openning csv output file");
    }
    QProgressDialog progress(this);
    progress.setWindowModality(Qt::ApplicationModal);
    progress.setMinimumDuration(0);

    int scanAxis = transform.axisX();
    qreal prevPos = Q_SNAN;
    qreal sizeBase = 100 / qreal(file.size());
    QList<UTDataSegment> segments = QList<UTDataSegment>() << UTDataSegment();
    while (!progress.wasCanceled() && !file.atEnd()) {
        UTDataSegment segment = file.ReadSegment(1024);
        progress.setValue(qreal(file.pos()) * sizeBase);
        for (const auto &frame: segment) {
            if (!frame.IsIGateTriggered() || !frame.Context.IsReverse(scanAxis) || frame.Context.GetChannelID() != 0) {
                continue;
            }
            qreal pos = frame.GetPose(scanAxis);
            if (qAbs(pos - prevPos) > 50 && !qIsNaN(prevPos)) {
//                qCDebug(debug) << "Line" << segments.count() << ": length" << segments.last().count();
                segments << UTDataSegment();
            }
            prevPos = pos;
            segments.last() << frame;
        }
    }

    QTextStream csv(&csvFile);
    csv << col << endl;

    qCDebug(debug) << "Finished reading" << segments.size() << "lines";

    const qreal threshold = qCos(qDegreesToRadians(60.0));
    for (const auto &segment: segments) {
        QVector3D previous = segment.first().UpdatePosition().first;
        QVector<QVector3D> points;
        points.reserve(segments.count());
        for (int index = 1; index < segment.count(); ++index) {
            QVector3D pos = segment[index].UpdatePosition().first;
            if (!pos.isNull() && qAbs((pos - previous).normalized()[scanAxis]) > threshold) {
                if (pos.distanceToPoint(QVector3D(0, 0, 0)) > 1e10) {
                    qCDebug(debug) << "Bad frame" << segment[index].Pose;
                } else {
                    previous = pos;
                    points << pos;
                }
            }
        }

        QMap<qreal, QVector3D> p;
        qreal total = 0;
        previous = points.value(0);
        for (const QVector3D &pos: points) {
            total += pos.distanceToPoint(previous);
            p[total] = pos;
            previous = pos;
        }

        qCDebug(debug) << "Dumping segment" << segment.size() << ", points" << points.size();
        if (p.size() < col * 1) {
            qCWarning(debug) << "Skipping line with" << p.size() << "frames";
            continue;
        }
        qreal interval = p.lastKey() / (col - 1);                               //区间间隔数应该为采样点个数 - 1
//        qreal seperation = qreal(p.count() - 1) / (col);
        for (int index = 0; index < col; ++index) {
//            const auto &pos = points[qFloor(index * seperation)];
            const auto it = p.upperBound(interval * index);
            const auto &pos = (it == p.end()) ? p.last() : it.value();
            csv << pos[0] << "," << pos[1] << "," << pos[2] << endl;
        }
    }
    qCDebug(debug) << "Finished writing resampled csv";
}

/*! 标记为选区 */
void PanelCScan::OnRecordRect()
{
    polygonSelection->setVisible(true);
    polygonSelection->setPolygon(QPolygonF(selectRect));
}

/*! 隐藏光标 */
void PanelCScan::OnCursorHide()
{
    SetCursorPosition(QPointF(Q_INFINITY, Q_INFINITY));
}

void PanelCScan::OnMouseMoved(const QPointF &point)
{
    if (m_magnifier->isVisible()) {
        QPoint p = point.toPoint() - m_magnifier->rect().center();
        m_magnifier->move(p);
    }
    if (annotationManager->mouseMoveHandler(point)) {
        return;
    }
    if (ui->actionCursorFollow->isChecked()) {
        SetCursorPosition(point);
    }
}

void PanelCScan::UpdateDefectList(const UTImage &image, UTDefectProcessorOptions options)
{
    options.roi = GetRecordPolygon();

    QProgressDialog progress(tr("正在评定缺陷"), tr("取消"), 0, 0, this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);

    UTDefectList defects = image.FindDefects(imageList, options);
    const ImageStatistics statistics = calculateStatistics();

    for (auto &defect: defects) {
        defect.area = defect.pixelCount * transform.gridArea();
        defect.minRect = transform.MapFromGrid(defect.markRect);
        defect.percentage = qreal(defect.pixelCount) / statistics.pixelCount;
    }

//    qDebug() << "Total pixel count before remove: " << defects.TotalPixelCount();
    for (auto it = defects.begin(); it != defects.end(); ) {
        if (it->area < options.minOuterArea) {
            it = defects.erase(it);
        } else {
            ++it;
        }
    }
    progress.hide();
    UTLayer *layer = GetLayer();
    if (layer) {
        DefectItemModel *model = layer->GetModel();
        model->appendDefectList(defects);
    }
}

void PanelCScan::OnCDataIndexChanged(int index)
{
    Q_UNUSED(index);
    int CDataIndex = currentFeatureIndex();
    bool isDataTime = dataHeader().GetDataType(CDataIndex) == UTDataType::DataTime;
    gain->setEnabled(!isDataTime);
    axisVisual->setTitleText(isDataTime == true ? tr("μs") : tr("%"));
    if (imageList.count() > CDataIndex && CDataIndex >= 0) {
        RefreshUTImages();
        refreshSliceData();
    }
}

/*! 接受View的右键菜单 */
void PanelCScan::OnViewContextMenuRequest(const QPoint &pos)
{
    QMenu menu;
    // 控制部分菜单的显示使能
    UTImage image = currentImage();
    bool enable = image.IsValid();

    menu.addSection(tr("视图"));
    menu.addAction(ui->actionZoomIn);
    menu.addAction(ui->actionZoomOut);
    menu.addAction(ui->actionZoomInput);
    menu.addAction(ui->actionZoomFit);
    menu.addAction(ui->actionZoomFill);
    menu.addSeparator();
    menu.addAction(ui->actionCursorFollow);
    menu.addAction(ui->actionCursorHide);
    menu.addSeparator();
    menu.addAction(tr("添加参考光标"), this, &PanelCScan::onAddReferCursor);
    menu.addSeparator();
    menu.addAction(tr("设置原点"), this, &PanelCScan::OnSetOrigin);
    menu.addAction(tr("取消原点"), this, &PanelCScan::OnCancelOrigin);
    menu.addSeparator();
    if (enable) {
        menu.addAction(tr("保存图像"), this, &PanelCScan::OnCScanPixmapSave);
        menu.addAction(tr("复制图像"), this, &PanelCScan::CopyCScanPixmap);
        menu.addAction(tr("保存ABC扫图像"), this, &PanelCScan::OnSaveAllPixmap);
        menu.addAction(tr("添加选区"), polygonSelection, &GraphicsEditablePolygon::startRecord)->setDisabled(!polygonSelection->polygon().isEmpty());
        menu.addSeparator();
        menu.addAction(tr("清除选区"), polygonSelection, &GraphicsEditablePolygon::reset);
        menu.addSeparator();
        menu.addAction(tr("查看A扫波形"), this, &PanelCScan::OnLookAtAData);
        menu.addAction(tr("移到当前位置"), this, &PanelCScan::OnLocate);
        menu.addSeparator();
    }
    menu.addAction(tr("统计/判伤"), this, &PanelCScan::OnCScanDataStatistic);

    // 添加各个managaer的actions
    menu.addSeparator();
    menuGlobalPos = pos;
    menu.exec(pos);
}

/*! 文件打开的接口 */
void PanelCScan::OnDocumentOpen()
{
    polygonSelection->setPolygon(QPolygonF());
    OnCancelOrigin();
}

/*! 获取当前的UTImage */
UTImage PanelCScan::currentImage()
{
    // 首先需要计算出当前的特征值序号
    int CDataIndex = currentFeatureIndex();
    if (imageList.count() > CDataIndex && CDataIndex >= 0) {
        return imageList.value(CDataIndex);
    }
    return UTImage();
}

/*! 框选区域改变的槽函数 */
void PanelCScan::onRubberBandChanged(QRect rubberBandRect, QPointF fromScenePoint, QPointF toScenePoint)
{
    Q_UNUSED(rubberBandRect);
    if (!toScenePoint.isNull()) {
        rubberBandPos = ui->view->mapToGlobalFromScene(toScenePoint);
        selectRect = ui->view->mapFromScene(QRectF(fromScenePoint, toScenePoint));
    } else {
        QMenu menu(this);
        menu.addAction(tr("放大到这里"), this, &PanelCScan::OnZoomIn);
        menu.addAction(tr("采样仿形"), this, &PanelCScan::OnSampleAndShape)->setVisible(Core::ICore::isInternalVersion());
        menu.addAction(tr("标记为选区"), this, &PanelCScan::OnRecordRect);
        menu.addSeparator();
        menu.addAction(tr("统计/判伤"), this, &PanelCScan::OnCScanDataStatistic);
        menu.exec(rubberBandPos);
    }
}

/*! 初始化查看A扫数据面板 */
void PanelCScan::InitAScanDataPlot()
{
    if (!ascanDataPlotWidget.isNull()) {
        return;
    }
    ascanDataPlotWidget = new QDockWidget(tr("A扫还原"), Core::ICore::mainWindow());
    ascanDataPlotWidget->setAllowedAreas(Qt::NoDockWidgetArea);
    ascanDataPlotWidget->setObjectName("AScanStatistics");
    QWidget *widget = new QWidget(ascanDataPlotWidget);

    auto *layout = new QVBoxLayout(widget);
    ascanPlot = new AScanGraphicsView(widget);
    info = new DataInfoWidget(widget);
    layout->addWidget(ascanPlot);
    layout->addWidget(info);

    ascanDataPlotWidget->setWidget(widget);
    Core::ICore::mainWindow()->addDockWidget(Qt::LeftDockWidgetArea, ascanDataPlotWidget);
    ascanDataPlotWidget->setFloating(true);
    ascanDataPlotWidget->hide();
}

void PanelCScan::RefreshUTImages()
{
    //            emit PixmapUpdated(image.ToPixmap(palette, &image));
    QMetaObject::invokeMethod(&renderer, "OnImageUpdated", Q_ARG(UTImageList, imageList));
    QMetaObject::invokeMethod(&renderer, "OnImagePaletteUpdated", Q_ARG(ColorPalette, ui->palette->colorPalette()));
    QMetaObject::invokeMethod(&renderer, "OnImageIndexChanged", Q_ARG(int, currentFeatureIndex()));
    refreshStatistics();
}

ImageStatistics PanelCScan::calculateStatistics()
{
    prevUpdate = QDateTime::currentDateTime();

    UTImage image = currentImage();
    QJsonValue area;
    if (polygonSelection->polygon().isEmpty()) {
        QRectF rect = transform.MapFromGrid(image.GetRect());
        QPolygonF polygon(rect);
        area = PolygonFToJson(polygon);
    } else {
        area = PolygonFToJson(polygonSelection->polygon());
    }

    ImageStatistics s;
    s.area = JsonToPolygonF(area);
    UTImage mask = image.CreateSameSize();

    UTLayer *layer = document()->GetLayerByID(m_layerId);
    QPolygon imagePolygon = layer ? layer->GetImageOptions().MapToGrid(s.area) : s.area.toPolygon();
    mask.DrawPolygon(imagePolygon);
    mask = mask.And(image);

    image.GetMinMax(&s.min, &s.max, &s.minLocation, &s.maxLocation, &mask);

    QPointF minLocation;
    QPointF maxLocation;
    QPointF origin = document()->origin().toPointF();
    s.mean = 0;
    s.stddev = 0;

    disconnect(ui->minCursor, nullptr, nullptr, nullptr);
    disconnect(ui->maxCursor, nullptr, nullptr, nullptr);
    if ((s.minLocation.x() >= 0) && (s.minLocation.y() >= 0) && (s.maxLocation.x() >= 0) && (s.maxLocation.y() >= 0)) {
        image.meanStdDev(&s.mean, &s.stddev, &mask);
        minLocation = transform.MapFromGrid(s.minLocation) - origin;
        maxLocation = transform.MapFromGrid(s.maxLocation) - origin;
        connect(ui->minCursor, &QToolButton::clicked, this, [=] () {
            QPointF pos = {minLocation.x(), -minLocation.y()};
            ui->view->ensureVisible(pos);
            SetCursorPosition(pos);
        });
        connect(ui->maxCursor, &QToolButton::clicked, this, [=] () {
            QPointF pos = {maxLocation.x(), -maxLocation.y()};
            ui->view->ensureVisible(pos);
            SetCursorPosition(pos);
        });
    }

    ui->average->setText(tr("%1").arg(s.mean));
    ui->stdev->setText(tr("%1").arg(s.stddev));
    ui->max->setText(tr("%1 @(%2, %3)").arg(s.max).arg(maxLocation.x()).arg(maxLocation.y()));
    ui->min->setText(tr("%1 @(%2, %3)").arg(s.min).arg(minLocation.x()).arg(minLocation.y()));

    if (qIsFinite(cursorPos.x()) && qIsFinite(cursorPos.y())) {
        QPoint pos = transform.MapToGrid(ui->view->mapFromScene(cursorPos));
        qreal cursorValue = Q_SNAN;
        if (image.Contains(pos)) {
            cursorValue = image(pos);
        }
        s.snr = UltrasonicManager::ampToDecibel(cursorValue / s.mean, 1);
        ui->snr->setText(tr("%1 dB").arg(s.snr));
    } else {
        ui->snr->setText(tr("请选择一个有效点"));
    }

    // 刷新焊合率统计
    s.threshold = ui->threshold->value();
    s.pixelCount = mask.CountNonZero();
    auto compareMode = ui->ratioMode->currentIndex() == 0 ? UTImage::CMP_GE : UTImage::CMP_LE;
    UTImage copyImage = image.Compare(s.threshold, compareMode).And(mask);
    s.thresholdPixelCount = copyImage.CountNonZero();
    s.thresholdRatio = 100 - s.thresholdPixelCount / qreal(s.pixelCount) * 100;
    ui->pixelCount->setText(tr("%1 / %2").arg(s.thresholdPixelCount).arg(s.pixelCount));
    ui->areaCount->setText(tr("%1 / %2").arg(s.thresholdPixelCount * transform.gridArea()).arg(s.pixelCount * transform.gridArea()));
    ui->ratio->setText(tr("%1%").arg(s.thresholdRatio));

    // 刷新柱状图
    if (!series) {
        return s;
    }
    series->clear();
    QVector<int> list;
    ColorPalette palette = ui->palette->colorPalette();
    int resolution = ui->histSize->value();
    list = image.CalcHistogram(resolution, palette.minValue(), palette.maxValue(), QRect(), &mask);
    if (list.isEmpty()) {
        qCWarning(debug) << "Statistic: got empty histogram";
        return s;
    }
    int maxValue = list.value(0);
    for (int index = 1; index < list.count(); ++index) {
        maxValue = qMax(maxValue, list[index]);
    }
    if (maxValue == 0) {
        return s;
    }
    auto *bar = new QBarSet(tr("频度"), series);
    for (int i = 0; i < resolution; ++i) {
        bar->append(list.value(i));
    }
    series->chart()->axisY()->setRange(0, maxValue);
    axisVisual->setRange(palette.minValue(), palette.maxValue());
    axisBar->setRange(0 - 0.5, list.size() - 0.5);
    series->setBarWidth(1);
    series->append(bar);

    return s;
}

void PanelCScan::refreshStatistics()
{
    refreshStatisticsInternal();
    prevUpdateRequest = QDateTime::currentDateTime();
}

void PanelCScan::refreshStatisticsInternal()
{
    QDateTime now = QDateTime::currentDateTime();
    if (!prevUpdate.isValid() || !prevUpdateRequest.isValid() ||
        (ui->actionCDataStatistic->isChecked() && ui->statisticsAutoRefresh->isChecked() && prevUpdateRequest.msecsTo(now) > 100 && prevUpdate.msecsTo(now) > 1000)) {
        calculateStatistics();
    } else {
        ThrottleUpdate::call(this, &PanelCScan::refreshStatisticsInternal);
    }
}

void PanelCScan::refreshSliceData()
{
    updateSliceData(Qt::Horizontal | Qt::Vertical);
}

void PanelCScan::updateRulerTimebase()
{
    UTSetup setup = currentSetup();
    if (!ui->actionRecalcFeatures->isChecked()) {
        ui->waveform->setSetup(setup);
    }
    qreal scale = 1.0;
    qreal delay = setup.delay();
    qreal range = setup.range();
    if (ascanDataPlotWidget->isVisible() && this == activeCScan) {              // 查看A扫波形界面只显示活动窗口的数据
        ascanPlot->setSetup(setup);
        ascanPlot->setRulerSelection(Qt::Horizontal, Range(delay, delay + range));
    }

    if (ui->actionSliceImage->isChecked()) {
        if (setup.timebaseUnit() == UTSetupTimebase::Distance) {
            scale = 2e3 / setup.materialSpeed();
        }
        ui->sliceX->setRulerUnitName(Qt::LeftEdge | Qt::BottomEdge, setup.timebase().timebaseSuffix());
        ui->sliceY->setRulerUnitName(Qt::LeftEdge | Qt::BottomEdge, setup.timebase().timebaseSuffix());
        ui->waveform->setRulerSelection(Qt::Horizontal, Range(delay, delay + range));
        ui->sliceX->setRulerSelection(Qt::Vertical, Range(delay, delay + range));
        ui->sliceX->setRulerScale(Qt::RightEdge, scale);
        ui->sliceY->setRulerSelection(Qt::Horizontal, Range(delay, delay + range));
        ui->sliceY->setRulerScale(Qt::BottomEdge, scale);
    } else if (ui->actionSliceFeature->isChecked()) {
        ui->sliceX->setRulerUnitName(Qt::LeftEdge | Qt::BottomEdge, "%");
        ui->sliceY->setRulerUnitName(Qt::LeftEdge | Qt::BottomEdge, "%");
        ui->sliceY->setRulerSelection(Qt::Horizontal, Range(0, 100));
        ui->sliceX->setRulerSelection(Qt::Vertical, Range(0, 100));
        ui->sliceX->setRulerScale(Qt::RightEdge, scale);
        ui->sliceY->setRulerScale(Qt::BottomEdge, scale);
    }
    updateTransform();
}

int PanelCScan::currentFeatureIndex() const
{
    int gate = gateCombobox->currentIndex();
    int gateType = featureCombobox->currentData(Qt::UserRole).toInt();
    int dataType = featureCombobox->currentData(Qt::UserRole + 1).toInt();
    return dataHeader().dataIndex(gate, gateType, dataType);
}

void PanelCScan::setCurrentFeatureIndex(int index)
{
    int gateIndex = dataHeader().gateId(index);
    QString featureName = dataHeader().GetName(index);
    gateCombobox->setCurrentIndex(gateIndex);
    featureCombobox->setCurrentText(featureName);
}

void PanelCScan::on_splitterH1_splitterMoved()
{
    ui->splitterH2->setSizes(ui->splitterH1->sizes());
}

void PanelCScan::on_splitterH2_splitterMoved()
{
    ui->splitterH1->setSizes(ui->splitterH2->sizes());
}

ReportWriter::ReportData MakeReport(DefectItemModel *model, const QMap<QString, QString> &info, ImageStatistics statistics)
{
    ReportWriter::ReportData data;
    ReportWriter::ReportTable &globals = data[QString()];
    ReportWriter::ReportRow global;
    for (auto it = info.constBegin(); it != info.constEnd(); ++it) {
        global.insert(it.key(), it.value());
    }
    const QPointF origin = document()->origin().toPointF();
    ReportWriter::ReportTable &defects = data["Defects"];
    ReportWriter::ReportTable &defectImages = data["DefectImages"];
    UTDefect maxDefect;
    int defectIndex = 1;
    qreal totalArea = 0;
    qreal maxLength = 0;
    qreal defectsPixels = 0;
    if (model != nullptr) {
        model->forItemsAtLevel<1>([&] (DefectItem *defectItem) {
            UTDefect defect = defectItem->defect();
            qreal area = defect.GetArea();
            if (maxDefect.GetArea() < area) {
                maxDefect = defect;
            }

            defectsPixels += defect.pixelCount;
            maxLength = qMax(maxLength, defect.GetCharacteristicSize());
            totalArea += area;

            QVariantMap row = defect.ToVariantMap(origin);
            row["Index"] = defectIndex;
            defects << row;
            int imageIndex = 1;
            foreach (const QImage &image, defect.screenshots()) {
                QVariantMap imageItem;
                imageItem["Image"] = image;
                imageItem["DefectIndex"] = defectIndex;
                imageItem["Index"] = imageIndex;
                defectImages << imageItem;
                imageIndex++;
            }
            defectIndex++;
        });
    }

    QString fillInRatio = QString("%1%").arg(defectsPixels / statistics.pixelCount * 100);
    QString maxDefectInRatio = QString("%1%").arg(maxDefect.percentage * 100);
    global["TotalDefectArea"] = totalArea;                                      /*!< 总缺陷面积 */
    global["MaxDefectArea"] = maxDefect.GetArea();                              /*!< 最大缺陷面积 */
    global["MaxDefectLength"] = maxLength;                                      /*!< 最大缺陷长度 */
    global["DefectCount"] = defectIndex - 1;                                    /*!< 缺陷总数 */
    global["FillInRatio"] = fillInRatio;                                        /*!< 缺陷面积占比 */
    global["MaxDefectInRatio"] = maxDefectInRatio;                              /*!< 最大缺陷面积占比 */
    global["Origin"] = QString("(%1, %2)").arg(origin.x()).arg(origin.y());     /*!< 原点位置 */
    global["Threshold"] = statistics.threshold;                                 /*!< 焊合率阈值 */
    global["ThresholdPixelCount"] = statistics.thresholdPixelCount;             /*!< 选区过阈值像素数(焊合) */
    global["PixelCount"] = statistics.pixelCount;                               /*!< 选区总像素数 */
    global["ThresholdRatio"] = statistics.thresholdRatio;                       /*!< 焊合率 */
    global["SNR"] = statistics.snr;                                             /*!< 信噪比 */
    global["AreaWidth"] = statistics.area.boundingRect().width();               /*!< 选区宽度 */
    global["AreaHeight"] = statistics.area.boundingRect().height();             /*!< 选区高度 */

    foreach (PanelCScan *panel, Core::ICore::mainWindow()->findChildren<PanelCScan *>()) {
        static const QRegularExpression numberPattern("\\d+");                  /* 从面板名称中抽取ID，加入报表 */
        QString imageIndex = numberPattern.match(panel->objectName()).captured(0);
        global[QString("picture%1").arg(imageIndex)] = panel->ExportFullsizeImage(true);
    }

    QPixmap pixmap("./BScanPicture.png");
    global["BScanPicture"] = pixmap.toImage();
    global["DataTime"] = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm");

    globals << global;

    return data;
}

void PanelCScan::on_generateReport_clicked()
{
    DefectItemModel *model = nullptr;
    UTLayer *layer = GetLayer();
    if (layer != nullptr) {
        model = layer->GetModel();
    }

    QString templateFile = platform()->Get("ReportTemplate").toString();
    if ( !QFile(templateFile).exists() ){
        QMessageBox::warning(this, QString(), tr("导出报表失败 - 模板文件不存在"));
        return;
    }

    QMap<QString, QString> customData = document()->scanTask()->data().customData;
    /* 获取文件名 */
    QString outputFile = ReportDlg::askFileName(customData, tr("Excel报表文件 (*.xlsx)"));
    if (outputFile.isEmpty()) {
        return;
    }

    QFileInfo outputFileInfo(outputFile);
    QString outputFilePath = outputFileInfo.absoluteFilePath();
    if (outputFileInfo.suffix().toLower() != "xlsx") {
        outputFilePath += ".xlsx";
    }

    QDir dir;
    if (!dir.mkpath(outputFileInfo.absolutePath())) {
        QMessageBox::warning(this, QString(), tr("导出报表失败 - 无法为下列文件创建文件夹：\n%1").arg(outputFilePath));
        return;
    }

    ReportWriterQtExcel report(this);
    bool success = report.Generate(templateFile, outputFilePath, MakeReport(model, customData, calculateStatistics()));
    if (!success) {
        QMessageBox::warning(this, QString(), tr("导出报表失败 - %1\n%2").arg(report.GetLastError(), outputFilePath));
        return;
    }
    if (QMessageBox::question(this, QString(), tr("导出报表成功，现在打开报表吗?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(outputFilePath));
    }
}

/*! B扫视口范围更新 */
void PanelCScan::OnViewSelectionChanged(Qt::Orientation ori, const Range &range)
{
    GraphicsAxis *axis = ori == Qt::Horizontal ? ui->sliceX->axis(Qt::BottomEdge) : ui->sliceY->axis(Qt::LeftEdge);
    axis->setSelection(range);
}

void PanelCScan::OnViewRangeChanged(Qt::Orientation ori, const Range &range)
{
    GraphicsAxis *axis = ori == Qt::Horizontal ? ui->sliceX->axis(Qt::BottomEdge) : ui->sliceY->axis(Qt::LeftEdge);
    axis->setRange(range);
}

/*! 接受双击事件 */
void PanelCScan::OnViewDoubleClicked(QPointF pos)
{
    if (annotationManager->doubleClickHandler(pos)) {
        return;
    }
    if (ui->actionCursorFollow->isChecked()) {
        return;
    }
    auto *view = qobject_cast<AbstractGraphicsView *>(sender());
    QPointF scenePos = cursorPos;
    if (view == ui->view) {                                                     // C扫界面双击
        scenePos = pos;
    } else if (view == ui->sliceX) {                                            // 横向B扫界面双击
        scenePos.setX(pos.x());
    } else if (view == ui->sliceY) {                                            // 纵向B扫界面双击
        scenePos.setY(pos.y());
    }

    SetCursorPosition(scenePos);
    if (isActiveWindow()) {
        emit ViewPosChanged(m_layerId, scenePos);
    }
}

void PanelCScan::SyncPanels()
{
    if (activeCScan == this && this->isVisible()) {
        const qreal scale = ui->view->view()->scale();
        const QPointF center = ui->view->view()->center();
        foreach (PanelCScan *panel, cscanList) {
            panel->ui->view->view()->setScale(scale);
            panel->ui->view->view()->setCenter(center);
        }
    }
}

/*! 切换新的活动窗口 */
void PanelCScan::UpdateActivePanelCScan(PanelCScan *debug)
{
    if (activeCScan == debug) {
        return;
    }
    foreach (QPointer<PanelCScan> panel, cscanList) {
        if (!activeCScan.isNull()) {
            if (panel != activeCScan) {
                disconnect(activeCScan, nullptr, panel, nullptr);                               // 断开旧的活动窗口与其它窗口的同步信号
            }
        }
    }
    activeCScan = debug;
}

QPolygon PanelCScan::GetRecordPolygon()
{
    QPolygonF polygon;
    UTImage image = currentImage();
    if (!image.IsValid()) {
        return polygon.toPolygon();
    }
    if (polygonSelection->polygon().isEmpty()) {
        QRectF rect = transform.MapFromGrid(image.GetRect());
        polygon.append(rect.topLeft());
        polygon.append(rect.bottomLeft());
        polygon.append(rect.bottomRight());
        polygon.append(rect.topRight());
    } else {
        polygon = polygonSelection->polygon();
    }
    return transform.MapToGrid( polygon );
}

void PanelCScan::focusInEvent(QFocusEvent *event)
{
    QFrame::focusInEvent(event);
    UpdateActivePanelCScan(this);
}

void PanelCScan::keyReleaseEvent(QKeyEvent *event)
{
    QFrame::keyReleaseEvent(event);
    ui->view->scene()->sendEvent(polygonSelection, event);
}

void PanelCScan::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    on_splitterH1_splitterMoved();
}

void PanelCScan::onLicenseUpdated()
{
    bool develop = LicenseManager::instance()->hasDevelopLicense();
    ui->actionSurfacePlot->setVisible(develop);
    ui->actionSliceSync->setVisible(develop);
    ui->actionSliceSyncOffset->setVisible(develop);
}

void PanelCScan::on_actionImageOptions_triggered()
{
    DialogImageOptions dialog;
    dialog.SetOptions(transform);
    if (MotionManager::driver()->queue()->GetState() == Motion::TrioQueue::STATE_RUNNING) {
        dialog.setEnabled(false);
    }
    if (dialog.exec() != QDialog::Accepted) {
        return;
    }
    ImageOptions newTransform = dialog.GetOptions();
    UTLayer *layer = document()->GetLayerByID(m_layerId);
    if (layer && layer->GetImageOptions() != newTransform) {
        layer->SetImageOptions(newTransform);
        if (QMessageBox::warning(this, QString(), tr("您更改了成像参数，需要重载文件才能生效\n现在重载吗?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            document()->Reload();
        }
    }
}

void PanelCScan::on_actionViewProbe_toggled(bool checked)
{
    probeIndicatorManager->setVisible(checked);
}

void PanelCScan::on_actionViewDefect_toggled(bool checked)
{
    defectManager->setVisible(checked);
}

void PanelCScan::on_actionViewImage_toggled(bool checked)
{
    pixmapItem->setVisible(checked);
    if (polygonSelection->polygon().isEmpty()) {
        polygonSelection->setVisible(checked);
    }
}

void PanelCScan::on_actionWaveformRuler_triggered(bool checked)
{
    if (checked) {
        ui->waveform->setRulerPos(Qt::LeftEdge | Qt::BottomEdge);
    } else {
        ui->waveform->setRulerPos(Qt::Edges());
    }
}

void PanelCScan::on_actionHideSliceCursor_triggered(bool checked)
{
    m_cursorManager->setCursorVisible(!checked);
}

void PanelCScan::on_actionSliceBrush_triggered()
{
    updateSliceBrush();
}

void PanelCScan::on_actionSurfacePlot_triggered()
{
    MatlabEngine matlab;
    UTLayer *layer = GetLayer();
    if (!layer || !matlab.IsValid()) {
        qCDebug(debug) << "Matlab unavailable";
//        return;
    }
    QProgressDialog progress;
    progress.setLabelText(tr("正在读取文件..."));

    QString command;
    QTextStream s(&command, QIODevice::WriteOnly);
    progress.setWindowModality(Qt::ApplicationModal);
    UTStreamFile file(document()->GetCurrentFileName());
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, QString(), tr("无法读取数据: %1").arg(file.errorString()));
        return;
    }

    s << "data = [" << endl;
    const int featureIndex = currentFeatureIndex();
    while (!file.atEnd() && !progress.wasCanceled()) {
        UTDataSegment segment = file.ReadSegment(1024);
        for (const auto &frame: segment) {
            if (!transform.isChannelEnabled(frame.Context.GetChannelID())
                || !frame.IsIGateTriggered()
                || frame.Context.IsReverse(0)) {
                continue;
            }
            auto pos = frame.UpdatePosition();
            //if (pos.first[0] > 1100 && pos.first[1] < 819) {
                s << pos.first[0] << " " << pos.first[1] << " " << pos.first[2] << " "
                  << (frame.CData.value(featureIndex) + frame.Context.GetDelay()) * 0.75 << ";" << endl;
            //}
        }
        progress.setValue(qreal(file.pos()) / qreal(file.size()) * 100);
    }
    s << "];" << endl;
    s << "x = data(:, 1);" << endl;
    s << "y = data(:, 2);" << endl;
    s << "gridSize = 0.2;" << endl;
    s << "x_range = min(x): gridSize: max(x);" << endl;
    s << "y_range = min(y): gridSize: max(y);" << endl;
    s << "[xi, yi] = meshgrid(x_range, y_range);" << endl;
    s << "zi = griddata(x, y, data(:, 3), xi, yi);" << endl;
    s << "ci = griddata(x, y, data(:, 4), xi, yi);" << endl;
    s << "surf(xi, yi, zi, ci, 'EdgeColor','none');" << endl;
    s << "axis equal;" << endl;
    s << "colorbar" << endl;
    s << "maximum = max(data(:, 4))" << endl;
    s << "minimum = min(data(:, 4))" << endl;

    WriteFileAsString("image3d.m", command);
    matlab.DoString(command);
    qCDebug(debug) << "Done.";
}

void PanelCScan::on_actionSliceSyncOffset_triggered()
{
    bool ok = false;
    int offset = QInputDialog::getInt(this, QString(), tr("B扫描表面波同步偏移"), sliceOffset, 0, 512, 1, &ok);
    if (ok) {
        sliceOffset = offset;
    }
}

void PanelCScan::on_actionSliceImage_toggled(bool checked)
{
    pixmapSliceX->setVisible(checked);
    pixmapSliceY->setVisible(checked);
    updateRulerRange();
    updateRulerTimebase();
}

void PanelCScan::on_actionSliceFeature_toggled(bool checked)
{
    featureSliceX->setVisible(checked);
    featureSliceY->setVisible(checked);
    updateRulerRange();
    updateRulerTimebase();
}

void PanelCScan::on_actionColorPalette_triggered(bool checked)
{
    ui->palette->setVisible(checked);
    ui->paletteSlice->setVisible(checked);
}

void PanelCScan::on_actionZoomFill_toggled(bool enabled)
{
    ui->view->setZoomEnable(!enabled);
    auto resizePolicy = enabled ? GraphicsAxis::ResizeKeepSelection : GraphicsAxis::ResizeKeepScale;
    auto *leftAxis = ui->view->axis(Qt::LeftEdge);
    auto *bottomAxis = ui->view->axis(Qt::BottomEdge);
    leftAxis->setResizePolicy(resizePolicy);
    bottomAxis->setResizePolicy(resizePolicy);
    const QRectF &source = transform.sourceRect();
    if (enabled) {
        if (source.isValid()) {
            leftAxis->setSelection(Range(source.top(), source.bottom()));
            bottomAxis->setSelection(Range(source.left(), source.right()));
        }
    } else {
        ResetView();
    }
}

//!< 点对点测距
void PanelCScan::on_actionReferenceLine_toggled(bool checked)
{
    Q_UNUSED(checked);
    modeChange(ui->actionReferenceLine, AnnotationManager::Line);
}

void PanelCScan::on_actionReferenceCircle_toggled(bool checked)
{
    Q_UNUSED(checked);
    modeChange(ui->actionReferenceCircle, AnnotationManager::Circle);
}

void PanelCScan::on_actionCustomCircle_toggled(bool checked)
{
    Q_UNUSED(checked);
    modeChange(ui->actionCustomCircle, AnnotationManager::CustomCircle, tr("直径"));
}

void PanelCScan::on_actionReferenceRect_toggled(bool checked)
{
    Q_UNUSED(checked);
    modeChange(ui->actionReferenceRect, AnnotationManager::Rect, tr("边长"));
}

void PanelCScan::modeChange(QAction *action, AnnotationManager::Mode mode, const QString &title)
{
    if (action->isChecked()) {
        if (!title.isEmpty()) {
            bool ok = false;
            qreal diameter = QInputDialog::getDouble(this, QString(), title , 0, 0, std::numeric_limits<double>::max(), DECIMAL, &ok);
            if (!ok) {
                action->setChecked(false);
                return;
            }
            annotationManager->setDiameter(diameter);
        }
        for (auto *a: {ui->actionReferenceLine, ui->actionReferenceCircle, ui->actionCustomCircle, ui->actionReferenceRect}) {
            if (a != action) {
                a->setChecked(false);
            }
        }
        ui->view->view()->viewport()->setCursor(Qt::CrossCursor);
        annotationManager->setMode(mode);
    } else {
        ui->view->view()->viewport()->unsetCursor();
        annotationManager->setMode(AnnotationManager::None);
    }
}

void PanelCScan::recalculateStart()
{
    ui->waveform->setSetupLocked(false);
    ui->waveform->setGatesVisible(true);
    cloneSetup = ui->waveform->setup();
    updateRulerTimebase();
    ui->waveform->setSetup(cloneSetup);
}

void PanelCScan::recalculateFinish()
{
    UTLayer *layer = GetLayer();
    if (!layer) {
        qCCritical(debug) << "Can't recalculate features: invalid layer";
        return;
    }
    UTDataSegment segment = layer->dataAll();
    for (UTDataFrame &frame: segment) {
        CDataFrame features = cloneSetup.gateFeatures(frame.waveform());
        frame.CData = features;
    }
    layer->Reset();
    layer->OnDataArrived(segment);
    setDataFrame(ui->waveform->lastFrame());
    ui->waveform->setGatesVisible(true);
    ui->waveform->setSetupLocked(true);
}

UTSetup PanelCScan::currentSetup() const
{
    if (ui->actionRecalcFeatures->isChecked()) {
        return cloneSetup;
    }
    return document()->setupForChannel(ui->waveform->lastFrame().Context.GetChannelID());
}

void PanelCScan::updateTransform()
{
    if (ui->actionSliceImage->isChecked()) {
        qreal scale = currentSetup().range() / 512;
        qreal offset = currentSetup().delay();

        QTransform transformX;
        transformX.translate(transform.sourceRect().left(), offset);
        transformX.scale(transform.gridSize().width(), scale);
        pixmapSliceX->setTransform(transformX);

        QTransform transformY;
        transformY.translate(offset, transform.sourceRect().top());
        transformY.scale(scale, transform.gridSize().height());
        pixmapSliceY->setTransform(transformY);
    } else if (ui->actionSliceFeature->isChecked()) {
        qreal scale = 100 / 100;
        qreal offset = 0;

        QTransform transformX;
        transformX.translate(transform.sourceRect().left(), offset);
        transformX.scale(transform.gridSize().width(), scale);
        featureSliceX->setTransform(transformX);

        QTransform transformY;
        transformY.translate(offset, transform.sourceRect().top());
        transformY.scale(scale, transform.gridSize().height());
        transformY.rotate(90, Qt::ZAxis);
        transformY.scale(1, -1);
        featureSliceY->setTransform(transformY);
    }
}

void PanelCScan::updateRulerRange()
{
    Range waveformRange;
    bool isViewRevesedH = ui->view->isReversed(Qt::Horizontal);
    bool isViewRevesedV = ui->view->isReversed(Qt::Vertical);
    qCDebug(debug) << "isViewRevesedH =" << isViewRevesedH << "isViewRevesedV =" << isViewRevesedV;
    if (ui->actionSliceImage->isChecked()) {
        waveformRange.setRange(0, 512);
        ui->sliceX->setReversed(Qt::Vertical, !isViewRevesedV);
    } else if (ui->actionSliceFeature->isChecked()) {
        waveformRange.setRange(0, 100);
        ui->sliceX->setReversed(Qt::Vertical, isViewRevesedV);
    }
    ui->sliceY->setReversed(Qt::Horizontal, isViewRevesedH);
    ui->sliceX->setReversed(Qt::Horizontal, isViewRevesedH);
    ui->sliceY->setReversed(Qt::Vertical, isViewRevesedV);

    ui->waveform->setRulerSelection(Qt::Vertical, waveformRange);
}

void PanelCScan::on_actionRecalcFeatures_toggled(bool arg1)
{
    if (arg1) {
        recalculateStart();
    } else {
        recalculateFinish();
    }
}

void PanelCScan::onGainEditingFinished()
{
    UTLayer *layer = GetLayer();
    if (!layer) {
        return;
    }
    OnImagesUpdate(layer->imageList());
}

void PanelCScan::on_actionMagnifier_toggled(bool checked)
{
    if (checked) {
        int size = qMin(ui->view->size().width() / 2, ui->view->size().height() / 2);
        m_magnifier->setFixedSize(size, size);
        m_magnifier->setCurrentView(ui->view->view());
        m_magnifier->setCurrentScene(ui->view->view()->scene());
        QTransform transform = ui->view->view()->transform() * QTransform::fromScale(3, 3);
        m_magnifier->setTransform(transform);
        QPoint p = mapFromGlobal(QCursor::pos());
        m_magnifier->move(p - m_magnifier->rect().center());
    }
    m_magnifier->setVisible(checked);
}

void PanelCScan::onMagnifierClicked(double zoomLevel, QPointF position)
{
    Q_UNUSED(zoomLevel);
    Q_UNUSED(position);
    ui->actionMagnifier->setChecked(false);
}

void PanelCScan::onAddReferCursor()
{
    auto *cursor = new GraphicsCursorManager(this);
    connect(cursor, &GraphicsCursorManager::referCursorChanged, this, [=]() {
        qCDebug(debug) << "remove referCursor";
        m_referCursorList.removeOne(cursor);
        cursor->deleteLater();
    });
    QPointF scenePos = ui->view->mapScenePosFromGlobal(menuGlobalPos);
    cursor->createItems(ui->view->rootItem(), ui->sliceX->rootItem(), ui->sliceY->rootItem());
    cursor->setCursorPosition({scenePos.x(), -scenePos.y()});
    cursor->setCursorColor(cursorColorList.value(m_referCursorList.count() % cursorColorList.count()));
    cursor->setMenuEnable(true);
    cursor->setCursorVisible(true);
    cursor->setMovable(true);
    m_referCursorList.append(cursor);
}

void PanelCScan::on_actionSelectLayer_triggered()
{
    QList<int> availableLayers = {};
    for (UTLayer *layer: document()->GetLayerMap()) {
        availableLayers.append(layer->GetID());
    }
    QStringList availableLayerText = Utils::transform(availableLayers, [] (int layerId) {
        return tr("图层 %1").arg(layerId);
    });
    Q_ASSERT(availableLayerText.removeDuplicates() == 0);
    bool ok = false;
    QString item = QInputDialog::getItem(this, QString(), tr("图层更改为："), availableLayerText, availableLayers.indexOf(m_layerId), false, &ok);
    int index = availableLayerText.indexOf(item);
    if (ok && index >= 0) {
        SetLayerID(index + 1);
    }
    ui->actionSelectLayer->setChecked(false);
}

bool PanelCScan::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->view->view()->viewport() && event->type() == QEvent::MouseButtonPress) {
        auto *mouseEvent = static_cast<QMouseEvent *>(event);
        if (ui->actionMoveModel->isChecked() && currentImage().IsValid()) {
            locateTo(ui->view->rootItem()->mapFromScene( ui->view->view()->mapToScene(mouseEvent->pos()) ));
        }
    }
    return QFrame::eventFilter(watched, event);
}

void PanelCScan::on_actionCopyColorToOthers_triggered()
{
    ColorPalette color = this->ui->palette->colorPalette();
    foreach (PanelCScan *panel, cscanList) {
        panel->ui->palette->setColorPalette(color);
    }
}

void PanelCScan::on_actionZoomInput_triggered()
{
    QWindow *window = this->window()->windowHandle();
    qreal dpi = 0;
    if (window && window->screen()) {                   //安全检查
        dpi = window->screen()->logicalDotsPerInch();   //每英寸的像素数
        if (dpi <= 0) {
            dpi = DPI;
        }
    } else {
        dpi = DPI;
    }
    qreal dotSizeInInch = 1 / dpi;                      //每像素点占英寸大小
    qreal dotSize = InchToMillimetre(dotSizeInInch);    //每像素点占毫米大小

    QList<int> scale = {10, 20, 50, 100, 150, 200, 500};
    QStringList scaleText = Utils::transform(scale, [] (int scale) {
        return tr("%1%").arg(scale);
    });
    Q_ASSERT(scaleText.removeDuplicates() == 0);
    qreal numInput = 0;
    forever {
        bool ok = false;
        QString item = QInputDialog::getItem(this, QString(), tr("当前缩放比例为：%1%\n缩放比例：").arg(ui->view->view()->scale() * dotSize * 100), scaleText, 3, true, &ok);
        if (!ok) {
            return;
        }
        bool isNum = false;
        numInput = item.remove('%').toDouble(&isNum);
        if (isNum && numInput > 0) {
            break;
        }
        QMessageBox::warning(this, QString(), tr("无效的值\"%1\", 请重新输入").arg(item));
    }
    ui->view->view()->setScaleAnimated(numInput / dotSize / 100);
}

void PanelCScan::on_actionAnnotationLineItem_triggered(bool checked)
{
    auto *line = new AnnotationLineItem(ui->view->rootItem());
    line->setLine(QPointF(0, 20), QPointF(100, 20));
//    if (checked) {
//        line->grabKeyboard();
//        line->grabMouse();
//    } else {
//        line->ungrabKeyboard();
//        line->ungrabMouse();
//    }
}

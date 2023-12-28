#pragma once

#include <Imaging/AnnotationManager.h>
#include <Imaging/ColorPalette.h>
#include <Imaging/UTDefect.h>
#include <Imaging/UTImage.h>
#include <QDateTime>
#include <QDockWidget>
#include <QFrame>
#include <QMenu>
#include <QThread>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <Waveform/AScanGraphicsView.h>
#include "GraphicsCursorManager.h"

namespace Ui {
class PanelCScan;
}

class QComboBox;
class QDoubleSpinBox;
class DataInfoWidget;
class GraphicsEditablePolygon;
class DefectManager;
class ProbeIndicatorManager;
class UTLayer;
class AScanGraphicsView;
class DefectTreeView;
class Magnifier;

struct ImageStatistics
{
    CDataValue min = 0;
    CDataValue max = 0;
    QPoint minLocation;
    QPoint maxLocation;
    qreal snr = Q_SNAN;
    CDataValue mean = 0;
    CDataValue stddev = 0;
    qreal threshold = 0;
    int pixelCount = 0;
    int thresholdPixelCount = 0;
    qreal thresholdRatio = 0;
    QPolygonF area;
};

class CScanRenderer : public QObject
{
    Q_OBJECT

public:
    CScanRenderer(QObject *parent = nullptr);

public slots:
    void OnImageUpdated(UTImageList newImage);
    void OnImagePaletteUpdated(ColorPalette newPalette);
    void OnImageIndexChanged(int newIndex);

private:
    void renderImage();
    void UpdateImage();

signals:
    void PixmapUpdated(QPixmap pixmap, qreal min, qreal max);
    void BusyStatusChanged(bool isBusy);

private:
    int index;
    ColorPalette palette;
    UTImageList imageList;
    bool dirty;
};

class PanelCScan : public QFrame
{
    Q_OBJECT

    struct FullImageOption
    {
        int widthImageA = 458;
        int widthRuler = 18;
        int widthPalette = 18;
        int widthPaletteRuler = 18;
        int widTimeStamp = 18;
        int widBlank = 0;
        bool autoRulerSize = true;
        bool isShowImageAB = true;
        bool timeStampFlag = false;
    };

public:
    explicit PanelCScan(QWidget *parent = nullptr);
    ~PanelCScan();

public:
    void SetLayerID(int id);
    UTImage currentImage();
    QImage ExportFullsizeImage(bool timeFlag = false);
    QImage ExportABCFullsizeImage(FullImageOption fio);
    UTLayer *GetLayer() const;
    bool HasDefectTreeView(DefectTreeView *view);

    QJsonValue saveSettings() const;
    void loadSettings(const QJsonValue &value);
    QList<QAction *> savedActions() const;                                      /*!< 状态被保存的 action 列表 */

public slots:
    void setDataFrame(const UTDataFrame &frame);
    void locateTo(const QPointF &scenePos);
    void SetCursorPosition(const QPointF &scenePos);
    void refreshImages(const QHash<QPoint, UTDataFrame> &segment);

signals:
    void ViewPosChanged(const int &id, const QPointF &pos);

protected:
    void focusInEvent(QFocusEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    void onLicenseUpdated();
    void RefreshUTImages();
    ImageStatistics calculateStatistics();
    void refreshStatistics();
    void refreshStatisticsInternal();
    void refreshSliceData();
    void updateRulerTimebase();

private:
    void InitUI();
    void InitManagers();
    void InitToolBar();
    void InitAScanDataPlot();
    void InitBScan();
    int currentFeatureIndex() const;                        /*得到特征值表索引值，内容包括gate + feature*/
    void setCurrentFeatureIndex(int index);           /*特征值表索引值 设置界面gate\featureCombobox*/

    QImage renderCurrentImage();
    UTDataSegment dataSegment(Qt::Orientation orientation, int index = -1) const;
    void updateSliceData(Qt::Orientations orientations);
    void UpdateDefectList(const UTImage &image, UTDefectProcessorOptions options);
    void UpdateActivePanelCScan(PanelCScan *cscan);
    QPolygon GetRecordPolygon();
    void modeChange(QAction *action, AnnotationManager::Mode mode, const QString &title = QString());
    void recalculateStart();
    void recalculateFinish();
    UTSetup currentSetup() const;
    void updateTransform();
    void updateRulerRange();
    UTImage sliceImage(Qt::Orientation orientation, int index = -1) const;
    QImage sliceImageRendered(Qt::Orientation orientation, int index = -1) const;
    void sliceFeatureRendered(Qt::Orientation orientation);
    QPainterPath sliceFeaturePath(const UTDataSegment &segment);
    QBrush sliceFeatureBrush(const UTDataSegment &segment);
    void updateSliceBrush();

private slots:
    void OnGateIndexChanged(int index);
    void OnCDataIndexChanged(int index);
    void OnImageOptionsChanged(const ImageOptions &options);
    void OnImagesUpdate(UTImageList list);
    void onPixmapUpdated(const QPixmap &pixmap, qreal min, qreal max);

    void OnSetOrigin();
    void OnCancelOrigin();
    void OnCScanPixmapSave();
    void CopyCScanPixmap();
    void OnSaveAllPixmap();
    void setOrigin(const QVector3D &origin3D);
    void ResetView();
    void OnLocate();
    void OnCScanDataStatistic();
    void OnSelectRectDefect();
    void OnLookAtAData();
    void onGainEditingFinished();
    void onMagnifierClicked(double zoomLevel, QPointF position);
    void onAddReferCursor();

    void OnZoomIn();
    void OnSampleAndShape();
    void OnRecordRect();
    void OnCursorHide();

    void OnMouseMoved(const QPointF &point);
    void OnViewContextMenuRequest(const QPoint &pos);
    void OnDocumentOpen();
    void onRubberBandChanged(QRect rubberBandRect, QPointF fromScenePoint, QPointF toScenePoint);
    void on_splitterH1_splitterMoved();
    void on_splitterH2_splitterMoved();
    void on_generateReport_clicked();
    void OnViewSelectionChanged(Qt::Orientation ori, const Range &range);
    void OnViewRangeChanged(Qt::Orientation ori, const Range &range);
    void OnViewDoubleClicked(QPointF pos);
    void SyncPanels();
    void on_actionImageOptions_triggered();
    void on_actionViewProbe_toggled(bool checked);
    void on_actionViewDefect_toggled(bool checked);
    void on_actionViewImage_toggled(bool checked);
    void on_actionWaveformRuler_triggered(bool checked);
    void on_actionHideSliceCursor_triggered(bool checked);
    void on_actionSliceBrush_triggered();
    void on_actionSurfacePlot_triggered();
    void on_actionSliceSyncOffset_triggered();
    void on_actionSliceImage_toggled(bool checked);
    void on_actionSliceFeature_toggled(bool checked);
    void on_actionColorPalette_triggered(bool checked);
    void on_actionZoomFill_toggled(bool enabled);
    void on_actionReferenceLine_toggled(bool checked);
    void on_actionCustomCircle_toggled(bool checked);
    void on_actionReferenceRect_toggled(bool checked);
    void on_actionReferenceCircle_toggled(bool checked);
    void on_actionRecalcFeatures_toggled(bool arg1);
    void on_actionMagnifier_toggled(bool checked);
    void on_actionSelectLayer_triggered();
    void on_actionCopyColorToOthers_triggered();
    void on_actionZoomInput_triggered();
    void on_actionAnnotationLineItem_triggered(bool checked);

private:
    Ui::PanelCScan *ui;
    QDateTime prevUpdateRequest;
    QDateTime prevUpdate;
    int m_layerId = 0;

    static QPointer<DataInfoWidget> info;
    static QPointer<QDockWidget> ascanDataPlotWidget;
    static QPointer<AScanGraphicsView> ascanPlot;

    QComboBox *gateCombobox = nullptr;
    QComboBox *featureCombobox = nullptr;
    QDoubleSpinBox *gain = nullptr;

    DefectManager *defectManager = nullptr;
    ProbeIndicatorManager *probeIndicatorManager = nullptr;
    QGraphicsItem *imageRoot = nullptr;
    QGraphicsPixmapItem *pixmapItem = nullptr;
    QGraphicsPixmapItem *pixmapSliceX = nullptr;
    QGraphicsPixmapItem *pixmapSliceY = nullptr;
    QGraphicsPathItem *featureSliceX = nullptr;
    QGraphicsPathItem *featureSliceY = nullptr;
    QGraphicsPathItem *pathItem = nullptr;
    GraphicsEditablePolygon *polygonSelection = nullptr;
    AnnotationManager *annotationManager = nullptr;
    UTImageList imageList;
    Magnifier *m_magnifier = nullptr;
    UTSetup cloneSetup;
    CScanRenderer renderer;
    QThread renderThread;
    QPoint menuGlobalPos;   // pixel/global coordinate
    QRectF selectRect;
    QPoint rubberBandPos;
    QPointF cursorPos;      // scene coordinate
    ImageOptions transform;

    QtCharts::QBarSeries *series = nullptr;
    QtCharts::QValueAxis *axisBar = nullptr;
    QtCharts::QValueAxis *axisVisual = nullptr;
    QtCharts::QValueAxis *axisPixel = nullptr;

    int sliceOffset = 100;

    static QPointer<PanelCScan> activeCScan;
    static QList<QPointer<PanelCScan> > cscanList;

    GraphicsCursorManager *m_cursorManager = nullptr;
    QList<GraphicsCursorManager *> m_referCursorList = {};
};

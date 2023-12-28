#pragma once

#include "AbstractGraphicsView.h"
#include "GraphicsInvertedLine.h"
#include "GraphicsMarkItem.h"
#include <QLabel>
#include <QTimer>
#include <QWidget>
#include <Ultrasonic/UTChannel.h>
#include <Ultrasonic/UTData.h>
#include <Ultrasonic/UTSetup.h>
#include <allrising/ConnectionHelper.h>
#include <allrising/ThrottleUpdate.h>

class GraphicsGateItem;
class GraphicsEditablePolygon;
class UTSetup;
class GraphicsWaveformItem;

struct AScanStyle
{
    QColor waveformOutline = Qt::black;
    QColor waveformFill = Qt::black;
    bool waveformFillEnable = false;
    QColor background = Qt::white;
    QColor gridColor = Qt::gray;
    bool gridEnable = true;
    bool featureEnable = false;
    bool gateEnable = true;
};

class WAVEFORM_EXPORT AScanGraphicsView : public AbstractGraphicsView
{
    Q_OBJECT

    Q_PROPERTY(UTSetup setup READ setup WRITE setSetup NOTIFY setupChanged)
    Q_PROPERTY(bool setupLocked READ isSetupLocked WRITE setSetupLocked)
    Q_PROPERTY(AScanStyle plotStyle READ plotStyle WRITE setPlotStyle)

public:
    AScanGraphicsView(QWidget *parent = nullptr);
    ~AScanGraphicsView();

public:
    virtual bool eventFilter(QObject *watched, QEvent *event);
    UTSetup setup() const;
    AScanStyle plotStyle() const;
    bool isSetupLocked() const;
    UTDataFrame lastFrame() const;
    qreal tcgSyncPos(const int gateIndex = 0);
    void refreshFocalPos();

public slots:
    void setSetup(const UTSetup &setup);
    void setSetupLocked(bool setupLocked);
    void setPlotStyle(AScanStyle plotStyle);
    void setMonitorData(UTDataFrame data);                                // 接收PanelAScan传来的DataFrame
    void setFeatureMarkVisible(bool visible);
    void setWaveformFill(bool enabled = false);
    void setCursorEnabled(bool enabled);
    void setGatesVisible(bool visible);
    void updateTimebaseScale();
    void setChannel(const QPointer<UTChannel> &channel);

signals:
    void onHorizontalRulerMouseDoubleClicked();
    void setupChanged(UTSetup setup);
    void channelChanged(QPointer<UTChannel> channel);

private slots:
    void onMouseMoved(const QPointF &scenePos);
    void updateWaveform();
    void updateControls();
    void updatePlotStyle();
    void updateCursor();
    void onGateItemMenu();
    void onAddDac();
    void onZoomTo();
    void onAutoGain();
    void onAutoCalculateSoundVelocity();
    void onTimeOut();

private:
    void initUi();
    int currentGateIndex() const;

private:
    UTSetup m_setup;
    bool m_setupLocked = false;
    AScanStyle m_plotStyle;
    QList<GraphicsGateItem *> m_gateItems;
    GraphicsInvertedLine *cursorH = nullptr;
    GraphicsInvertedLine *cursorV = nullptr;
    GraphicsWaveformItem *waveItem = nullptr;
    GraphicsEditablePolygon *dacItem = nullptr;
    UTDataFrame m_lastFrame;
    QPointF cursorPos;
    QPoint mouseStartPos;
    bool isBottomRulerPressed = false;
    bool isCursorEnable = false;
    ConnectionHelper<AScanGraphicsView, UTSetup> handler;
    QGraphicsPathItem *m_triangleRoot = nullptr;
    GraphicsMarkItem *m_verticalTriangle = nullptr;
    GraphicsMarkItem *m_horizontalTriangle = nullptr;
    GraphicsMarkItem *m_probeFocalPosCross = nullptr;
    QTimer *m_timer = nullptr;
    int m_gateIndex = 0;
    qreal m_referenceGain = 0;
    int m_timeCount = 0;
    QPointer<UTChannel> m_channel;
};

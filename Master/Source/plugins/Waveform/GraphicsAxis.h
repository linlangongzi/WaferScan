#pragma once

#include "WaveformGlobal.h"
#include <allrising/Range.h>
#include <allrising/Transform1D.h>
#include <QPointer>
#include <QWidget>

class WAVEFORM_EXPORT GraphicsAxis : public QObject
{
    Q_OBJECT

public:
    enum ResizePolicy {
        ResizeKeepSelection,
        ResizeKeepScale,
    };
    Q_ENUM(ResizePolicy)

    Q_PROPERTY(Range range READ range WRITE setRange NOTIFY rangeChanged)
    Q_PROPERTY(Range selection READ selection WRITE setSelection NOTIFY selectionChanged)
    Q_PROPERTY(qreal origin READ rulerOrigin WRITE setRulerOrigin)
    Q_PROPERTY(qreal unitRatio READ userScale WRITE setUserScale)
    Q_PROPERTY(Transform1D sceneTransform READ sceneTransform)
    Q_PROPERTY(Transform1D userTransform READ userTransform WRITE setUserTransform)
    Q_PROPERTY(Transform1D rulerTransform READ rulerTransform WRITE setRulerTransform)
    Q_PROPERTY(qreal center READ center WRITE setCenter)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
    Q_PROPERTY(ResizePolicy resizePolicy READ resizePolicy WRITE setResizePolicy)
    Q_PROPERTY(bool locked READ isLocked WRITE setLocked)
    Q_PROPERTY(QString unitName READ unitName WRITE setUnitName)
    Q_PROPERTY(bool isReversed READ isReversed WRITE setReversed)
    Q_PROPERTY(qreal cursorPos READ cursorPos WRITE setCursorPos)

public:
    explicit GraphicsAxis(QObject *parent = nullptr);

    const Range &range() const;
    const Range &selection() const;
    Range rulerSelection();
    qreal rulerOrigin() const {return m_rulerTransform.offsetAfter();}
    qreal userScale() const {return m_userTransform.scale();}
    qreal rulerScale() const {return m_rulerTransform.scale();}
    qreal center() const {return m_center;}
    qreal scale() const {return m_scale;}
    qreal selectionScale() const;
    qreal cursorPos() const {return m_cursorPos;}
    Qt::Orientation orientation() const;
    ResizePolicy resizePolicy() const {return m_resizePolicy;}
    bool isLocked() const {return m_locked;}
    QString unitName() const {return m_unitName;}
    bool isReversed() const {return m_reversed;}
    int direction() const {return m_reversed ? -1 : 1;}
    bool isDragEnabled() {return m_dragEnable;}

    int pixelLength() const;
    int pixelLength(const QSize &size) const;
    double lengthInPixel() const;
    void drawRuler(QPainter *painter, Qt::Edge edge, const QRect &rulerRect, const QRect &boundingRect);
    Transform1D sceneTransform() const;
    Transform1D userTransform() const;
    Transform1D GetTransform() const;
    Transform1D rulerTransform() const;

signals:
    void rangeChanged(const Range &range);
    void selectionChanged(const Range &range);
    void scaleChanged(qreal value);
    void centerChanged(qreal value);
    void axisUpdated();
    void rulerTransformChanged();

public slots:
    void setWidget(QWidget *widget);
    void setRange(const Range &newRange);
    void setSelection(const Range &newSelection);
    void setRulerOrigin(qreal origin);
    void setUserScale(qreal scale);
    void setRulerScale(qreal scale);
    void setScale(qreal newScale);
    void setCursorPos(qreal pos);
    void setOrientation(Qt::Orientation arg);
    void setResizePolicy(ResizePolicy policy) {m_resizePolicy = policy;}
    void setLocked(bool arg);
    void setUnitName(const QString &name);
    void setReversed(bool enable);
    void setDragEnable(bool enable) {m_dragEnable = enable;}
    void setCenter(qreal newCenter);
    void setUserTransform(const Transform1D &newTransform);
    void setRulerTransform(const Transform1D &newTransform);

protected:
    void updateSelection();
    void updateSceneTransform();
    virtual bool eventFilter(QObject *o, QEvent *e);

private:
    Range m_range;
    Range m_selection;
//    qreal origin;
//    qreal unitRatio;
    qreal m_center = 0;
    qreal m_scale = 1;
    Qt::Orientation m_orientation = Qt::Horizontal;
    ResizePolicy m_resizePolicy = ResizeKeepScale;
    bool m_locked = false;
    QString m_unitName;

    QPointer<QWidget> bindWidget;
    bool m_reversed = false;
    bool m_dragEnable = false;
    qreal m_cursorPos;
    Transform1D m_sceneTransform;
    Transform1D m_userTransform;
    Transform1D m_rulerTransform;
};

#pragma once

#include <QGraphicsView>
#include <QPointer>

QT_FORWARD_DECLARE_CLASS(QMouseEvent)

namespace Ui {
class Magnifier;
}

class GraphicsView;

class Magnifier : public QWidget
{
    Q_OBJECT

public:
    Magnifier(QWidget *parent = nullptr);

    void setCurrentView(QGraphicsView *mainView);
    void setCurrentScene(QGraphicsScene *scene);
    void setTransform(const QTransform &transform);

signals:
    void clicked(double zoomLevel, QPointF position);

protected:
    void resizeEvent(QResizeEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
    void showEvent(QShowEvent *e) override;
    void hideEvent(QHideEvent *e) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void moveEvent(QMoveEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;

private:
    QPointer<QGraphicsView> m_mainView;
    QRadialGradient m_gradientBrush;
    Ui::Magnifier *ui;
};

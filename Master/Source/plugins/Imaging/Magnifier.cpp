#include "Magnifier.h"
#include "ui_Magnifier.h"
#include <QMouseEvent>

Magnifier::Magnifier(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Magnifier)
{
    ui->setupUi(this);
    setMouseTracking(true);
    ui->m_graphicsView->setEnabled(false);
}

void Magnifier::resizeEvent(QResizeEvent *e)
{
    QWidget::resizeEvent(e);

    qreal radius = width() / 2.0;
    m_gradientBrush.setCenter(radius, radius);
    m_gradientBrush.setFocalPoint(radius, radius);
    m_gradientBrush.setRadius(radius);
    m_gradientBrush.setColorAt(1.0, QColor(255, 255, 255, 0));
    m_gradientBrush.setColorAt(0.0, QColor(0, 0, 0, 255));

    int cap = radius * 0.1;
    ui->m_graphicsView->setMask(QRegion(rect().adjusted(cap, cap, -cap, -cap), QRegion::Ellipse));
}

void Magnifier::showEvent(QShowEvent *e)
{
    QWidget::showEvent(e);
    grabMouse();
}

void Magnifier::hideEvent(QHideEvent *e)
{
    QWidget::hideEvent(e);
    releaseMouse();
}

void Magnifier::mousePressEvent(QMouseEvent *e)
{
    QWidget::mousePressEvent(e);

    emit clicked(ui->m_graphicsView->transform().m11(), m_mainView->mapToScene(m_mainView->mapFromGlobal(e->globalPos())));
}

void Magnifier::mouseMoveEvent(QMouseEvent *e)
{
    QWidget::mouseMoveEvent(e);
    move(pos() + (e->pos() - rect().center()));

    if (m_mainView) {
        ui->m_graphicsView->centerOn(m_mainView->mapToScene(m_mainView->mapFromGlobal(e->globalPos())));
    }
}

void Magnifier::wheelEvent(QWheelEvent *e)
{
    QWidget::wheelEvent(e);

    if (e->delta() > 0) {
        ui->m_graphicsView->scale(1.1, 1.1);
    } else if (ui->m_graphicsView->transform().m11() >= 1.1) {
        ui->m_graphicsView->scale(1.0 / 1.1, 1.0 / 1.1);
    }
}

void Magnifier::moveEvent(QMoveEvent *e)
{
    QWidget::moveEvent(e);
}

void Magnifier::setCurrentView(QGraphicsView *view)
{
    m_mainView = view;
}

void Magnifier::setCurrentScene(QGraphicsScene *scene)
{
    ui->m_graphicsView->setScene(scene);
}

void Magnifier::setTransform(const QTransform &transform)
{
    ui->m_graphicsView->setTransform(transform);
}

void Magnifier::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter p(this);

    p.setPen(Qt::NoPen);
    p.setBrush(m_gradientBrush);
    p.drawRect(rect());
}

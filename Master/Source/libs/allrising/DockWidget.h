#pragma once

#include <QDockWidget>
#include <QAbstractButton>
#include "allrising_global.h"

class DockWidgetTitleButton : public QAbstractButton
{
    Q_OBJECT

public:
    DockWidgetTitleButton(QWidget *parent);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
};

class DockWidgetTitle : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double backColorAlpha MEMBER highlight WRITE SetHighlightProgress)

public:
    DockWidgetTitle(QWidget *parent);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void AddButton(QAbstractButton *button);
    void SetOrientation(bool vertical);

public slots:
    void ToggleFloating();
    void ToggleOrientation();
    void SetHighlightProgress(qreal progress);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *e) override;

public:
    qreal highlight = 0;

private:
    bool isVertical = true;
};

/*!
 * \brief DockWidget
 * 封装悬浮窗口控件，实现独立的闪烁功能
 */
class ALLRISING_UTILS_EXPORT DockWidget : public QDockWidget
{
    Q_OBJECT

public:
    DockWidget(QWidget *parent = nullptr, Qt::WindowFlags flags = nullptr);

public:
    virtual QJsonObject SaveLayout() const;
    virtual void LoadLayout(const QJsonObject &layout);
    void Flash(int count = -1);

public slots:
    void OpenMenu();
    void SetTitleBarVertical(bool isVertical);
    void SetAlwaysFloating(bool enabled);
    void fullscreen();
    void DockAt(Qt::DockWidgetArea area);
    void DockLeft();
    void DockRight();
    void DockTop();
    void DockBottom();

protected:
    void leaveEvent(QEvent *event) override;

private:
    DockWidgetTitle *title;
    QAbstractButton *menuButton;
    QAction *actionVerticalTitle;
    QAction *actionAlwaysFloating;
    QAction *actionFullscreen;
};

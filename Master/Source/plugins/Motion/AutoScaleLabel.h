#pragma once

#include <QLabel>

class AutoScaleLabel : public QLabel
{
    Q_OBJECT

    Q_PROPERTY(int minimumFontSize READ minimumFontSize WRITE setMinimumFontSize)
    Q_PROPERTY(int maximumFontSize READ maximumFontSize WRITE setMaximumFontSize)


public:
    AutoScaleLabel(QWidget *parent = nullptr);

    int minimumFontSize() const;
    int maximumFontSize() const;

public slots:
    void setMinimumFontSize(int minimumFontSize);
    void setMaximumFontSize(int maximumFontSize);

private:
    void paintEvent(QPaintEvent *) override;

private:
    int m_minimumFontSize = 12;
    int m_maximumFontSize = 30;
    int m_prevFontSize = -1;
};

#pragma once

#include <QObject>
#include <QKeySequence>
#include "allrising_global.h"

/*!
 * \brief HotkeyListener 是一个在App范围内注册快捷键的类，当设定的快捷键被按下或弹起时抛出
 * 信号。当用户切换到另一个程序时，按键会被弹起。
 *
 * 注意：这个类的实现效率较低，如果不需要捕获 KeyRelease ，请使用 QShortcut 。
 */
class ALLRISING_UTILS_EXPORT HotkeyListener : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QKeySequence key READ key WRITE setKey)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)

public:
    explicit HotkeyListener(QObject *parent = nullptr);
    HotkeyListener(const QKeySequence &hotkey, QObject *parent);

    QKeySequence key() const;
    bool isEnabled() const;

signals:
    void keyPressed();                                                          /*!< 按键被按下的信号 */
    void keyReleased();                                                         /*!< 按键被弹起的信号 */

public slots:
    void setKey(const QKeySequence &arg);                                       /*!< 设置按键 */
    void setKey(const QString &newKey);
    void setKey(const QJsonValue &arg);
    void setEnabled(bool arg);                                                  /*!< 设置快捷键的开关。关闭快捷键时，会发出按键弹起的信号 */

private slots:
    void onApplicationStateChanged(Qt::ApplicationState state);                 /*!< 应用程序状态改变的槽函数，这个槽函数负责在程序失去焦点时确保按键被弹起 */

private:
    bool eventFilter(QObject *obj, QEvent *event);
    void reinstallHotkey();

private:
    QKeySequence m_key;
    bool m_enabled = true;
};

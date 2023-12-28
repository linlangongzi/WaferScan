#include "HotkeyListener.h"
#include <QGuiApplication>
#include <QKeyEvent>
#include <QJsonValue>

HotkeyListener::HotkeyListener(QObject *parent) :
    QObject(parent)
{}

HotkeyListener::HotkeyListener(const QKeySequence &hotkey, QObject *parent) :
    QObject(parent),
    m_key(hotkey)
{
    reinstallHotkey();
}

QKeySequence HotkeyListener::key() const
{
    return m_key;
}

bool HotkeyListener::isEnabled() const
{
    return m_enabled;
}

void HotkeyListener::setKey(const QKeySequence &arg)
{
    if (m_key == arg) {
        return;
    }
    m_key = arg;
    reinstallHotkey();
}

void HotkeyListener::setKey(const QString &newKey)
{
    setKey(QKeySequence(newKey));
}

void HotkeyListener::setKey(const QJsonValue &arg)
{
    m_key = QKeySequence( arg.toString() );
    if (m_key.isEmpty()) {
        m_key = QKeySequence( qRound( arg.toDouble() ) );
    }
    reinstallHotkey();
}

void HotkeyListener::setEnabled(bool arg)
{
    if (m_enabled && !arg) {                                                      /* 禁用快捷键时，发送按键被弹起的消息 */
        emit keyReleased();
    }
    m_enabled = arg;
    reinstallHotkey();
}

void HotkeyListener::onApplicationStateChanged(Qt::ApplicationState state)
{
    if (m_enabled && (state != Qt::ApplicationActive)) {
        emit keyReleased();
    }
}

bool HotkeyListener::eventFilter(QObject *obj, QEvent *event)
{
    if ((event->type() == QEvent::KeyPress) || (event->type() == QEvent::KeyRelease)) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);
        if (m_key.matches(QKeySequence(keyEvent->modifiers() + keyEvent->key()))) {
            if (!keyEvent->isAutoRepeat()) {
                if (event->type() == QEvent::KeyPress) {
                    emit keyPressed();
                } else {
                    emit keyReleased();
                }
            }
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

void HotkeyListener::reinstallHotkey()
{
    if (qApp == nullptr) {
        return qWarning("HotkeyListener::ReinstallHotkey: qApp not available");
    }
    qApp->removeEventFilter(this);
    disconnect(qApp, nullptr, this, nullptr);
    if (!m_key.isEmpty() && m_enabled) {
        qApp->installEventFilter(this);
        connect(qApp, &QGuiApplication::applicationStateChanged, this, &HotkeyListener::onApplicationStateChanged, Qt::UniqueConnection);
    }
}

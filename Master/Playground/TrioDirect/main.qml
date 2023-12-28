import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import com.allrising.test 1.0

ApplicationWindow {
    title: qsTr("Trio Direct Console")
    width: 640
    height: 480
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: messageDialog.show(qsTr("Open action triggered"));
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    MainForm {
        id: mainForm
        anchors.fill: parent
        send.onClicked: {
            trio.Execute(command.text)
            command.text = ""
        }
        command.onAccepted: {
            trio.Execute(command.text)
            command.text = ""
        }
        connect.onClicked: {
            trio.axisCount = mainForm.axisCount
            trio.host = hostAddress.text
            trio.port = hostPort.value
            trio.Connect()
        }
        disconnect.onClicked: trio.Disconnect()
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }

    MotionDriverTrio {
        id: trio
        onLineBufferChanged: {
            mainForm.history.append(lineBuffer)
        }
        onStateChanged: {
            console.log(trio.socketState)
            mainForm.status = trio.socketState
        }
        Component.onCompleted: mainForm.status = trio.socketState
    }
}

import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

Item {
    id: mainForm
    width: 640
    height: 480

    property alias connect: connect
    property alias disconnect: disconnect
    property alias send: send
    property alias axisCount: axisCount.value
    property alias hostAddress: hostAddress
    property alias hostPort: hostPort
    property alias status: status.text
    property alias command: command
    property alias history: history

    TextArea {
        id: history
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.bottom: send.top
        anchors.bottomMargin: 5
    }

    Button {
        id: disconnect
        x: 531
        y: 451
        text: qsTr("Disconnect")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
    }

    SpinBox {
        id: hostPort
        x: 368
        y: 452
        anchors.verticalCenter: connect.verticalCenter
        value: 23
        maximumValue: 65535
        anchors.right: connect.left
        anchors.rightMargin: 5
    }

    TextField {
        id: hostAddress
        x: 263
        y: 453
        text: "192.168.1.250"
        anchors.verticalCenter: hostPort.verticalCenter
        anchors.right: hostPort.left
        anchors.rightMargin: 5
        placeholderText: qsTr("Host Address")
    }

    Text {
        id: status
        y: 457
        text: qsTr("")
        anchors.verticalCenter: axisCount.verticalCenter
        horizontalAlignment: Text.AlignRight
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: axisCount.left
        anchors.rightMargin: 5
        font.pixelSize: 12
    }

    SpinBox {
        id: axisCount
        x: 215
        y: 452
        height: 21
        anchors.verticalCenter: hostAddress.verticalCenter
        clip: false
        value: 5
        anchors.right: hostAddress.left
        anchors.rightMargin: 5
    }

    Button {
        id: connect
        x: 381
        y: 451
        height: 26
        text: qsTr("Connect")
        anchors.verticalCenter: disconnect.verticalCenter
        anchors.right: disconnect.left
        anchors.rightMargin: 5
    }

    Button {
        id: send
        x: 568
        y: 418
        text: qsTr("Send")
        anchors.bottom: disconnect.top
        anchors.bottomMargin: 5
        isDefault: true
        anchors.right: parent.right
        anchors.rightMargin: 0
    }

    TextField {
        id: command
        y: 420
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.verticalCenter: send.verticalCenter
        anchors.right: send.left
        anchors.rightMargin: 5
        placeholderText: qsTr("Send command")
    }
}

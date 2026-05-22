import QtQuick

Item {
    id: root

    property string source: ""
    property int radius: 8

    implicitWidth: 120
    implicitHeight: 120

    Rectangle {
        anchors.fill: parent
        radius: root.radius
        color: "#2a2a2a"
        visible: img.status !== Image.Ready

        Text {
            anchors.centerIn: parent
            text: "\u266B"
            font.pixelSize: Math.min(parent.width, parent.height) * 0.4
            color: "#888"
        }
    }

    Rectangle {
        anchors.fill: parent
        radius: root.radius
        clip: true
        visible: img.status === Image.Ready

        Image {
            id: img
            anchors.fill: parent
            source: root.source
            sourceSize.width: parent.width * 2
            sourceSize.height: parent.height * 2
            fillMode: Image.PreserveAspectCrop
            smooth: true
        }
    }

    Behavior on scale {
        NumberAnimation { duration: 200; easing.type: Easing.OutBack }
    }
}

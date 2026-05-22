import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: root

    property string errorMessage: ""

    modal: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: Math.min(parent.width * 0.8, 400)
    height: contentColumn.implicitHeight + topPadding + bottomPadding + 40

    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 200 }
    }

    exit: Transition {
        NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 150 }
    }

    ColumnLayout {
        id: contentColumn
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 12

        Text {
            text: root.errorMessage
            wrapMode: Text.WordWrap
            color: "#333"
            Layout.fillWidth: true
        }

        Button {
            text: qsTr("OK")
            Layout.alignment: Qt.AlignRight
            onClicked: root.close()
        }
    }
}

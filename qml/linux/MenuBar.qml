import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

MenuBar {
    id: root

    signal openFolder()
    signal quit()

    Menu {
        title: qsTr("File")

        MenuItem {
            text: qsTr("Open Folder...")
            onTriggered: root.openFolder()
        }

        MenuSeparator {}

        MenuItem {
            text: qsTr("Exit")
            onTriggered: root.quit()
        }
    }

    Menu {
        title: qsTr("View")

        MenuItem {
            text: qsTr("Show Playlist")
            checkable: true
            checked: true
        }

        MenuItem {
            text: qsTr("Show Now Playing")
            checkable: true
            checked: true
        }
    }

    Menu {
        title: qsTr("Help")

        MenuItem {
            text: qsTr("About Music Player")
            onTriggered: aboutDialog.open()
        }
    }

    Dialog {
        id: aboutDialog
        title: qsTr("About")
        standardButtons: Dialog.Ok
        x: (parent.width - width) / 2
        y: (parent.height - height) / 3
        width: 300

        ColumnLayout {
            spacing: 8

            Text {
                text: qsTr("Music Player")
                font.pixelSize: 18
                font.weight: Font.Bold
            }

            Text {
                text: qsTr("Version 0.1")
                color: "#666"
            }

            Text {
                text: qsTr("A cross-platform media player built with Qt 6 and QML.")
                wrapMode: Text.WordWrap
            }
        }
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Drawer {
    id: drawer

    property string currentView: qsTr("Library")

    signal viewSelected(string view)

    width: Math.min(parent.width * 0.75, 300)
    height: parent.height

    ListView {
        anchors.fill: parent
        topMargin: 48

        model: [
            { icon: "\u266B", label: qsTr("Library") },
            { icon: "\u25B6", label: qsTr("Now Playing") },
            { icon: "\u2630", label: qsTr("Playlists") },
            { icon: "\u2699", label: qsTr("Settings") }
        ]

        delegate: ItemDelegate {
            width: parent.width
            height: 56

            contentItem: RowLayout {
                spacing: 16
                Layout.leftMargin: 16

                Text {
                    text: modelData.icon
                    font.pixelSize: 20
                    color: modelData.label === drawer.currentView ? "#1DB954" : "#fff"
                }

                Text {
                    text: modelData.label
                    font.pixelSize: 16
                    color: modelData.label === drawer.currentView ? "#1DB954" : "#fff"
                    font.weight: modelData.label === drawer.currentView ? Font.Bold : Font.Normal
                }
            }

            highlighted: modelData.label === drawer.currentView

            onClicked: {
                drawer.currentView = modelData.label
                drawer.viewSelected(modelData.label)
                drawer.close()
            }
        }

        ScrollBar.vertical: ScrollBar {}
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Window
import mplayer

Window {
    id: root

    property string platform: appController.platform
    property bool isMobilePlatform: platform === "android" || platform === "ios"
    property bool isAndroid: platform === "android"
    property bool isIos: platform === "ios"
    property bool isLinux: platform === "linux" || platform === "wasm"
    property bool isWasm: platform === "wasm"
    property bool useMobileLayout: isMobilePlatform || (isWasm && width < 768)

    width: useMobileLayout ? 420 : 960
    height: useMobileLayout ? 780 : 640
    visible: true
    title: qsTr("Music Player")

    Shortcut {
        sequence: "Space"
        onActivated: appController.togglePlayPause()
    }

    Shortcut {
        sequence: StandardKey.MoveToPreviousChar
        onActivated: appController.previousTrack()
    }

    Shortcut {
        sequence: StandardKey.MoveToNextChar
        onActivated: appController.nextTrack()
    }

    Shortcut {
        sequence: StandardKey.Open
        onActivated: {
            if (useMobileLayout && platformLoader.item && platformLoader.item.openFolder)
                platformLoader.item.openFolder()
        }
    }

    Loader {
        id: platformLoader
        anchors.fill: parent

        sourceComponent: {
            if (isAndroid || (isWasm && width < 768))
                return androidPage
            if (isIos)
                return iosPage
            return linuxPage
        }
    }

    Component {
        id: androidPage
        AndroidMainPage {}
    }

    Component {
        id: iosPage
        IosMainPage {}
    }

    Component {
        id: linuxPage
        LinuxMainPage {}
    }

    ErrorDialog {
        id: globalErrorDialog
    }

    Connections {
        target: appController.database
        function onDatabaseError(message) {
            globalErrorDialog.errorMessage = message
            globalErrorDialog.open()
        }
    }
}

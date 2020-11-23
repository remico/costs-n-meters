import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import AppScreens 1.0

ApplicationWindow {
    id: window
    width: Screen.width
    height: Screen.height
    visible: true
    title: qsTr("Costs & Meters")

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C1" : "\u25A1"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }
    }

    Drawer {
        id: drawer
        width: Screen.orientation === Qt.PortraitOrientation ?
                   Screen.width * 0.4
                 : Screen.width * 0.28
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Settings")
                width: parent.width
                onClicked: {
                    var settingsScreen = Qt.createComponent("assets:/qml/AppScreens/ScreenSettings.qml")
                    stackView.push(settingsScreen)
                    drawer.close()
                }
            }
//            ItemDelegate {
//                text: qsTr("Page 2")
//                width: parent.width
//                onClicked: {
//                    stackView.push("Page2Form.qml")
//                    drawer.close()
//                }
//            }
        }
    }

    StackView {
        id: stackView
        initialItem: ScreenHome { }
        anchors.fill: parent
    }
}

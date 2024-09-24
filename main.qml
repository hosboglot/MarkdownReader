import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: rootWindow
    visible: true

    ToolBar {
        id: toolBar
        width: parent.width

        RowLayout {
            anchors.fill: parent

            ToolButton {
                id: openDrawerButton
                icon.source: "menu-icon"
                onClicked: drawer.open()
            }

            Label {
                text: qsTr("documentName")
                elide: Label.ElideRight
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                id: searchDocumentButton
                icon.source: "search-doc-icon"
            }
        }
    }

    Drawer {
        id: drawer
        height: parent.height
        width: 0.66 * parent.width

        ToolBar {
            id: toolBarDrawer
            width: parent.width

            RowLayout {
                anchors.fill: parent

                Label {
                    verticalAlignment: Qt.AlignVCenter
                    Layout.leftMargin: 0.05 * parent.width
                    Layout.fillWidth: true
                    text: qsTr("MarkdownReader")
                }

                ToolButton {
                    id: searchSourcesButton
                    icon.source: "search-icon"
                }
            }
        }

        Text {
            anchors.centerIn: parent
            text: qsTr("hello side panel")
        }
    }

    Text {
        anchors.centerIn: parent
        text: qsTr("hello window")
    }
}

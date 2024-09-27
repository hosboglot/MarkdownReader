import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs 1.3

import repositoryHandler 1.0

Item {
    id: reader
    anchors.fill: parent

    RepositoryHandler {
        id: repositoryHandler
        onLocalSourceChanged: {
            repositoryView.model = repositoryModel();
        }
    }

    FileDialog {
        id: repositoryDialog
        title: "Выберите директорию"
        selectFolder: true
        onAccepted: repositoryHandler.localSource = fileUrl
    }

    Drawer {
        id: drawer
        height: parent.height
        width: 0.66 * parent.width

        Page {
            anchors.fill: parent
            header: ToolBar {
                id: toolBarDrawer
                width: parent.width

                RowLayout {
                    // @disable-check M16
                    anchors.fill: parent

                    Label {
                        verticalAlignment: Qt.AlignVCenter
                        Layout.leftMargin: 0.05 * parent.width
                        Layout.fillWidth: true
                        text: qsTr("MarkdownReader")
                    }

                    ToolButton {
                        icon.source: "icons/plus"
                        onClicked: repositoryDialog.open()
                    }

                    ToolButton {
                        icon.source: "icons/search"
                    }
                }
            }

            Component {
                id: repositoryDelegate
                Rectangle {
                    height: ListView.view.height / 20
                    width: ListView.view.width
                    color: ListView.isCurrentItem ? "lightgrey" : "white"
                    Text {
                        anchors.fill: parent
                        leftPadding: 10 * nestedDepth
                        verticalAlignment: Text.AlignVCenter
                        text: name
                    }
                }
            }

            ListView {
                id: repositoryView
                anchors.fill: parent
                clip: true
                focus: true
                delegate: repositoryDelegate
            }
        }
    }

    Page {
        anchors.fill: parent
        header: ToolBar {
            id: toolBar
            width: parent.width

            RowLayout {
                // @disable-check M16
                anchors.fill: parent

                ToolButton {
                    id: openDrawerButton
                    icon.source: "icons/menu"
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
                    icon.source: "icons/search-document"
                }
            }
        }

        Text {
            anchors.fill: parent
            verticalAlignment: Qt.AlignVCenter
            horizontalAlignment: Qt.AlignHCenter
            text: qsTr("hello window")
        }
    }
}

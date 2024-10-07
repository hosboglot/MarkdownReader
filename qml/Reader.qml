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
        onOpenDocumentChanged: {
            documentView.model = documentModel();
        }
    }

    FileDialog {
        id: repositoryDialog
        title: "Выберите директорию"
        selectFolder: true
        onAccepted: repositoryHandler.localSource = fileUrl
    }

    Popup {
        id: documentSearchPopup
        height: reader.height
        width: reader.width

        contentItem: ColumnLayout {
            anchors.fill: parent
            TextField {
                Layout.fillWidth: true
                placeholderText: "Введите запрос"
                onPreeditTextChanged: documentSearchView.model.setFilterFixedString(text)
            }
            ListView {
                id: documentSearchView
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                focus: true
                spacing: 10

                delegate: BlockDelegate {
                    baseUrl: repositoryHandler.openDocument
                    onClicked: {
                        documentView.positionViewAtIndex(
                                    ListView.view.model.getModelIndex(index),
                                    ListView.Beginning);
                        documentSearchPopup.close();
                    }
                }
            }
        }
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

            ListView {
                id: repositoryView
                anchors.fill: parent
                clip: true
                focus: true
                delegate: RepositoryDelegate {
                    onClicked: {
                        if (isDir) {
                            isOpen = !isOpen;
                        } else {
                            ListView.view.currentIndex = index;
                            repositoryHandler.openDocument = path;
                            drawer.close();
                        }
                    }
                }
            }
        }
    }

    Page {
        anchors.fill: parent
        header: ToolBar {
            id: toolBar
            width: parent.width

            RowLayout {
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
                    onClicked: {
                        if (!repositoryHandler.documentModel()) {
                            return;
                        }
                        documentSearchView.model = repositoryHandler.searchDocumentModel();
                        documentSearchPopup.open();
                    }
                }
            }
        }

        background: Rectangle {
            anchors.fill: parent
            color: "white"
        }

        ListView {
            id: documentView
            anchors.fill: parent
            clip: true
            focus: true
            spacing: 10

            delegate: BlockDelegate {
                baseUrl: repositoryHandler.openDocument
            }
        }
    }
}

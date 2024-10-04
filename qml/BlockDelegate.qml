import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtMultimedia 5.15

Rectangle {
    id: blockDelegate
    property alias baseUrl: textContent.baseUrl
    property bool hasBadge: badge.length > 0 || color.length > 0
    property color badgeColor: model.color.length > 0 ? model.color : "#3498DB"

    width: ListView.view.width
    height: gridLayout.height

    signal clicked()
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            parent.clicked();
            tagsVisible = !tagsVisible;
        }
    }

    GridLayout {
        id: gridLayout
        anchors {
            left: parent.left
            right: parent.right
        }

        columns: 2
        rows: 3

        Label {
            id: tagsText
            Layout.preferredHeight: visible ? tagsText.contentHeight : 0
            Layout.columnSpan: 2
            Layout.row: 0
            Layout.column: 0

            visible: tagsVisible

            wrapMode: Text.Wrap
            font.italic: true
            text: "Тэги: " + tags
        }

        Rectangle {
            id: badgeRect
            Layout.minimumWidth: hasBadge ? 10 : 0
            Layout.maximumWidth: hasBadge ? 10 : 0
            Layout.fillHeight: true
            Layout.rowSpan: 2
            Layout.row: 1
            Layout.column: 0
            visible: hasBadge

            radius: width / 3
            color: badgeColor
        }

        Label {
            id: badgeLabel
            Layout.row: 1
            Layout.column: 1
            visible: hasBadge

            font.bold: true
            color: badgeColor
            text: badge.length > 0 ? badge : "Важно"
        }

        Rectangle {
            id: contentRect
            Layout.row: 2
            Layout.column: 1
            Layout.preferredHeight: Math.max(textContent.contentHeight,
                                             videoPlayer.childrenRect.height)
            Layout.fillWidth: true

            Text {
                id: textContent
                anchors.fill: parent

                visible: type === "text"

                textFormat: Text.MarkdownText
                wrapMode: Text.Wrap
                text: content
            }

            Rectangle {
                id: videoPlayer
                anchors.fill: parent

                visible: type === "video"

                MediaPlayer {
                    source: content
                }
            }
        }
    }
}

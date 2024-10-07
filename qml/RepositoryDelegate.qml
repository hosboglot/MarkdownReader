import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

ItemDelegate {
    id: repositoryDelegate
    width: ListView.view.width

    height: isHidden ? 0 : entryText.contentHeight * 2
    Behavior on height {
        NumberAnimation { duration: 100 }
    }

    visible: !isHidden

    highlighted: ListView.isCurrentItem

    contentItem: RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 10 * nestedDepth

        Canvas {
            Layout.alignment: Qt.AlignVCenter
            Layout.preferredHeight: entryText.contentHeight / 3
            Layout.preferredWidth: Layout.preferredHeight * 2
            onPaint: {
                var ctx = getContext("2d");
                ctx.beginPath();
                ctx.moveTo(0, 0);
                ctx.lineTo(width, 0);
                ctx.lineTo(width / 2, height);
                ctx.closePath();
                ctx.fill();
                ctx.stroke();
            }
            visible: isDir
            rotation: isOpen ? 0 : -90
            Behavior on rotation {
                NumberAnimation { duration: 100 }
            }
        }

        Text {
            id: entryText
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height
            verticalAlignment: Text.AlignVCenter
            text: name
        }
    }
}


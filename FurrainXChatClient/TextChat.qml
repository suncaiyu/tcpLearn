import QtQuick 2.0
import QtQuick.Controls 2.5
Column {
    id : co
    anchors.right: sentByMe ? parent.right : undefined
    spacing: 6
    property bool sentByMe: Me
    property int listWidth
    property string thiscontent
    property int typevalue : -1
    function testShow(t) {
        console.log(t)
    }

    onTypevalueChanged: {
        co.testShow(typevalue)
    }

    Label {
        id: timestampText
        text: Qt.formatDateTime(new Date(), "yyyy-MM-dd hh:mm:ss")
        color: "gray"
        anchors.right: sentByMe ? parent.right : undefined
    }

    Row {
        id: messageRow
        spacing: 6
        anchors.right: sentByMe ? parent.right : undefined

        Image {
            id: avatarLeft
            height: 32
            width: height
            visible: !sentByMe
            source: "qrc:/robot.png"
        }
        Rectangle {
            id : textRect
            width: Math.min(messageText.implicitWidth + 24, listWidth- (sentByMe ? avatarRight.width : avatarLeft.width) - messageRow.spacing)
            height: messageText.implicitHeight + 24
            color: sentByMe ? "lightgrey" : "steelblue"

            radius : 15

            TextEdit {
                textFormat: Text.RichText
                id: messageText
                text: thiscontent
                color: sentByMe ? "black" : "white"
                anchors.fill: parent
                anchors.margins: 12
                wrapMode: Label.Wrap
                readOnly: true
                selectByMouse: true
            }
        }

        Image {
            id: avatarRight
            height: 32
            width: height
            visible: sentByMe
            source: "qrc:/me.png"
        }
    }
}

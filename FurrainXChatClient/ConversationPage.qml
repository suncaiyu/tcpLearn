/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.6
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0

Item {
    id: root
    signal setType(int t)
    objectName: "rootPage"
    function addToSession(text, me)
    {
        if(text.length <= 0)
        {
            return;
        }

        listView.model.insert(0, {"content": text, "Me": me});
        if (me) {
            chat.SendToOther(text)
        }
//        root.setType(77)
        messageField.text = "";
        messageField.focus = true;
        //verScrollBar.increase()
        verScrollBar.setPosition(1.0)
    }

    ColumnLayout {
        objectName: "columnL"
        anchors.fill: parent

        ListView {
            id: listView
            objectName: "ll"
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: pane.leftPadding + messageField.leftPadding
            displayMarginBeginning: 40
            displayMarginEnd: 40
            verticalLayoutDirection: ListView.BottomToTop
            spacing: 16
            model: ListModel {
            }

            delegate:TextChat {
                id : sss
                listWidth: listView.width
                thiscontent: content
                sentByMe: Me
                Component.onCompleted: {
                    root.setType.connect(sss.testShow)
                }
            }

            ScrollBar.vertical: ScrollBar {
                id: verScrollBar
            }
        }

        Rectangle{
            Layout.fillWidth: true
            height: 1
            color: "gray"
        }

        Pane {
            id: pane
            Layout.fillWidth: true
            RowLayout {
                width: parent.width
                spacing: 6

                TextArea {
                    textFormat: Text.RichText
                    id: messageField
                    Layout.fillWidth: true
                    placeholderText: qsTr("输入消息")
                    wrapMode: TextArea.Wrap
                    DropArea {
                        anchors.fill:parent
                        onDropped: {
                            if (drop.text.toLowerCase().indexOf("jpg") || drop.text.toLowerCase().indexOf("png") || drop.text.toLowerCase().indexOf("bmp") || drop.text.toLowerCase().indexOf("gif")) {
//                                console.log(drop.text)
                                messageField.append("<img src = " + drop.text + ">")
                            }
                        }
                    }

                    Keys.onReturnPressed: {
                        addToSession(messageField.text, true)
                    }
                    Keys.onEnterPressed: {
                        addToSession(messageField.text, true)
                    }
                }

                Button {
                    id: sendButton
                    text: qsTr("发送")
                    enabled: messageField.length > 0
                    onClicked: {
                        addToSession(messageField.text, true)
                    }
                }
            }
        }
        Connections {
            target: chat
            onOtherMsgSignal:{
                addToSession(str, false)
            }
        }
    }
}


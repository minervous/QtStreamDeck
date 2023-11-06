import QtQuick
import QtQuick.Controls
import minervous.streamdeck

Window {
    id: root

    width: 640
    height: 400
    visible: true
    title: qsTr('Example Project')

    property Window childWindow

    StreamDeck {
        id: deck
        //expectedDeviceType: StreamDeck.STREAMDECK_MINI
        property url pressedImage: "/Users/imayornykov/StreamDeck/QtStreamDeck/examples/cpp-example/images/Pressed.png"
        property url normalImage: "/Users/imayornykov/StreamDeck/QtStreamDeck/examples/cpp-example/images/Released.png"

        onPressed:
        {
            textIndex.text = 'Last pressed:' + index
        }

        onConnectedChanged: {
            reopen()
        }

        function reopen() {
            //console.info(StreamDeckManager.devices)
            if (connected) {
                if (!isOpen) {
                    open()
                    for (var i = 0; i < keyCount; ++i) {
                        setImageUrl(i, normalImage)
                    }
                }
            } else if (isOpen){
                close()
            }
        }

        Component.onCompleted: {
            init()
            reopen()
            console.info(StreamDeckManager.devices)
        }
    }

    Column {

        Row {
            Switch {
                id: emulatorSwitch
                onToggled: {
                    if (checked) {
                        if (!childWindow) {
                            childWindow = emulatorWindowComponent.createObject()
                        }
                        childWindow.show()
                    } else if (childWindow) {
                        childWindow.hide();
                    }
                }
                Component {
                    id: emulatorWindowComponent
                    Window {
                        width: 640
                        height: 400
                        x: root.x + root.width
                        y: root.y
                        visible: true
                        title: qsTr('Emulator')
                        color: 'gray'
                        QmlDeckEmulatorContent {
                            anchors.fill: parent
                        }
                    }
                }
            }
            Text {
                text: "Emulator window"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Text {
            id: textType
            text: 'Type: ' + deck.expectedDeviceType + '/' + deck.connectedDeviceType + ' ' + deck.keyRows + 'x' + deck.keyColumns + (deck.connected ? ' connected' : ' disconnected')
        }

        Text {
            text: "Desctiption: " + deck.modelName + " | " + deck.serialNumber + " | " + deck.firmwareVersion
        }

        Text {
            text: "Brightness: " + deck.brightness
        }

        Grid {
            id: grid
            rows: deck.keyRows
            columns:  deck.keyColumns
            spacing: 20
            Repeater {
                model: deck.keyCount
                delegate: Rectangle {
                    width: 72
                    height: 72
                    color: 'gray'
                    radius: 5

                    Image {
                        anchors.fill: parent
                        source: deck.buttonsState[index] ? deck.pressedImage :  deck.normalImage
                        onSourceChanged: {
                            deck.setImageUrl(index, source)
                        }
                    }

                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: -3
                        color: 'transparent'
                        border.color: 'blue'
                        border.width: 3
                        visible: deck.buttonsState[index]
                        radius: 8
                    }
                }
            }
        }
        Text {
            id: textIndex
        }
    }
}

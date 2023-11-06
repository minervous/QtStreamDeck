import QtQuick
import QtQuick.Controls
import Qt5Compat.GraphicalEffects
import minervous.streamdeck

Item {
    id: emulatorItem

    StreamDeckEmulator
    {
        id: emulator
        property var images: []

        Component.onCompleted: {
            init();
        }

        onImageSentBase64: {
            images[keyIndex] = imageData;
        }
    }

    Column {

        Row {
            Switch {
                id: emulatorConnected
                checked: emulator.connected
                onToggled: {
                    emulator.connected = checked
                    checked = emulator.connected;
                }
            }
            Text {
                text: "Connected"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Text {
            text: "isOpen: " + emulator.isOpen
        }
        Text {
            text: "Desctiption: " + emulator.modelName + " | " + emulator.serialNumber + " | " + emulator.firmwareVersion
        }
        Text {
            text: "Brightness: " + deck.brightness
        }

        Grid {
            id: emulatorGrid
            rows: emulator.keyRows
            columns:  emulator.keyColumns
            spacing: 20
            Repeater {
                model: deck.keyCount
                delegate: Item {
                    width: 72
                    height: 72
                    Rectangle {
                        id: background
                        anchors.fill: parent
                        radius: 5
                        color: "lightgray"
                        clip: true
                    }
                    Image {
                        id: img
                        anchors.fill: parent
                        source:  emulator.images && index < emulator.images.length ? emulator.images[index] : ""
                        layer.effect: OpacityMask {
                            maskSource: background
                        }
                        layer.enabled: true

                        Connections {
                            target: emulator
                            function onImageSentBase64(keyIndex, imageData) {
                                if (keyIndex === index) {
                                    console.warn("±±±±±±±±±±±±±±±±" + keyIndex)
                                    img.source = imageData;
                                }
                            }
                        }
                    }
                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        onPressed: {
                            emulator.press(index);
                        }
                        onReleased: {
                            emulator.release(index);
                        }
                        onCanceled: {
                            emulator.release(index);
                        }
                    }
                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: -3
                        color: 'transparent'
                        border.color: 'blue'
                        border.width: 3
                        visible: mouseArea.pressed
                        radius: 8
                    }
                }
            }
        }
    }
}

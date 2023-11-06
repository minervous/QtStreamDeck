import QtQuick
import QtQuick.Controls
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

        Grid {
            id: emulatorGrid
            rows: emulator.keyRows
            columns:  emulator.keyColumns
            spacing: 20
            Repeater {
                model: deck.keyCount
                delegate: Rectangle {
                    width: 72
                    height: 72
                    color: "lightgray"
                    Image {
                        id: img
                        anchors.fill: parent
                        source:  emulator.images && index < emulator.images.length ? emulator.images[index] : ""
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
                }
            }
        }
    }
}

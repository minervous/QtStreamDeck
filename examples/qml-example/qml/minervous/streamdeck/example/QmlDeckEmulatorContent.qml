import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects

import minervous.streamdeck

Window {
    id: emulatorWindow
    minimumHeight: contentColumn.implicitHeight + 20
    minimumWidth: Math.max(contentColumn.implicitWidth, devicePanel.width) + 40
    title: qsTr('Stream Deck Emulator')
    color: 'gray'

    StreamDeckEmulator {
        id: emulator
        property list<string> images: []

        onConnectedChanged: {
            if (!connected) {
                images = []
            }
        }

        onResetCalled: {
            images = []
        }

        onImageSent: {
            images[keyIndex] = imageDataBase64
        }

        Component.onCompleted: {
            init()
        }
    }

    Column {
        id: contentColumn
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 5

//        StreamDeckQmlControls {
//            width: 100
//            height: 100
//        }

        GridLayout {
            columns:  2
            width: parent.width
            columnSpacing: 5
            rowSpacing: 5

            Text {
                text: 'Connected'
                font.bold: true
            }
            Switch {
                id: emulatorConnected
                checked: emulator.connected
                onToggled: {
                    emulator.connected = checked
                    checked = emulator.connected
                }
            }

            Text {
                text: 'Device type:'
                font.bold: true
            }
            ComboBox {
                textRole: "text"
                valueRole: "value"
                onActivated: emulator.deviceType = currentValue
                Component.onCompleted: currentIndex = indexOfValue(emulator.deviceType)
                model: [
                    { value: StreamDeckType.STREAMDECK_MINI, text: qsTr("STREAMDECK_MINI") },
                    { value: StreamDeckType.STREAMDECK_ORIGINAL, text: qsTr("STREAMDECK_ORIGINAL") },
                    { value: StreamDeckType.STREAMDECK_MK2, text: qsTr("STREAMDECK_MK2") },
                    { value: StreamDeckType.STREAMDECK_XL, text: qsTr("STREAMDECK_XL") },
                    { value: StreamDeckType.STREAMDECK_PEDAL, text: qsTr("STREAMDECK_PEDAL") }
                ]
            }

            Text {
                text: 'Manufacturer:'
                font.bold: true
            }
            TextInput {
                text: emulator.manufacturer
                onEditingFinished: {
                    emulator.manufacturer = text
                }
            }

            Text {
                text: 'Product:'
                font.bold: true
            }
            TextInput {
                text: emulator.modelName
                onEditingFinished: {
                    emulator.modelName = text
                }
            }

            Text {
                text: 'Serial number:'
                font.bold: true
            }
            TextInput {
                text: emulator.serialNumber
                onEditingFinished: {
                    emulator.serialNumber = text
                }
            }

            Text {
                text: 'Firmware version:'
                font.bold: true
            }
            TextInput {
                text: emulator.firmwareVersion
                onEditingFinished: {
                    emulator.firmwareVersion = text
                }
            }

            Text {
                text: 'Brightness:'
                font.bold: true
            }
            Text {
                text: emulator.brightness
            }

            Text {
                text: 'isOpen:'
                font.bold: true
            }
            Text {
                text: emulator.isOpen
            }
        }

        Rectangle {
            id: devicePanel
            property int sizeUnit: 40
            property int buttonSize: sizeUnit * 1.6
            width: emulatorGrid.width + sizeUnit * 1.2 * 2
            height: emulatorGrid.height + sizeUnit * (1.2 + 1.7)
            anchors.horizontalCenter: parent.horizontalCenter
            color: "#303030"
            radius: sizeUnit * 0.4

            Text {
                color: 'white'
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.top
                anchors.verticalCenterOffset: emulatorGrid.anchors.topMargin / 2
                text: 'STREAM DECK'
                font.bold: true
                font.pixelSize: emulatorGrid.anchors.topMargin * 0.3
            }

            Grid {
                id: emulatorGrid
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: devicePanel.sizeUnit * 1.7
                rows: emulator.keyRows
                columns:  emulator.keyColumns

                spacing: devicePanel.sizeUnit * 0.3

                Repeater {
                    model: Math.min(emulatorGrid.rows * emulatorGrid.columns, emulator.keyCount)
                    delegate: Item {
                        width: devicePanel.buttonSize
                        height: devicePanel.buttonSize
                        Rectangle {
                            id: background
                            anchors.fill: parent
                            radius: devicePanel.sizeUnit * 0.2
                            color: "#383838"
                            clip: true
                        }
                        Image {
                            id: img
                            anchors.fill: parent
                            source:  emulator.images && index < emulator.images.length ? emulator.images[index] : ''
                            layer.effect: OpacityMask {
                                maskSource: background
                            }
                            layer.enabled: true
                        }
                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            onPressed: {
                                emulator.press(index)
                            }
                            onReleased: {
                                emulator.release(index)
                            }
                            onCanceled: {
                                emulator.release(index)
                            }
                        }
                        Rectangle {
                            anchors.fill: parent
                            anchors.margins: - border.width
                            color: 'transparent'
                            border.color: mouseArea.pressed ? 'blue' : "#404040"
                            border.width: mouseArea.pressed ? 3 : 2
                            radius: background.radius + border.width
                        }
                    }
                }
            }
        }
    }
}

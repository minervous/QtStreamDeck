import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import minervous.streamdeck

Window {
    id: root

    property Window childWindow

    minimumWidth: Math.max(content.implicitWidth + 40, 500)
    minimumHeight: Math.max(content.implicitHeight + 20, 300)
    visible: true
    title: qsTr('Example Project')
    color: 'olive'

    onClosing: {
        childWindow?.close();
    }

    StreamDeck {
        id: deck
        //expectedDeviceType: StreamDeck.STREAMDECK_MINI
        property url pressedImage: 'qrc:/examples/images/Pressed.png'
        property url normalImage: 'qrc:/examples/images/Released.png'

        onPressed: (index) => {
            textIndex.text = index
        }

        onConnectedChanged: {
            reopen()
        }

        function reopen() {
            if (connected) {
                if (!isOpen) {
                    open()
                    for (const i of keyCount) {
                        setImageUrl(i, normalImage)
                    }
                }
            } else if (isOpen) {
                close()
            }
        }

        Component.onCompleted: {
            init()
            reopen()
            console.info(StreamDeckManager.devices)
        }
    }

    component NameLabel: Label {
        font.bold: true
    }
    component ValueLabel: Label {
    }

    Column {
        id: content
        anchors.left: parent.left
        anchors.leftMargin: 20
        spacing: 10

        GridLayout {
            columns:  2
            width: parent.width
            columnSpacing: 5
            rowSpacing: 5

            NameLabel {
                text: 'Emulator window'
            }
            Switch {
                id: emulatorSwitch
                onToggled: {
                    if (checked) {
                        if (!childWindow) {
                            childWindow = emulatorWindowComponent.createObject()
                        }
                        childWindow.show()
                    } else if (childWindow) {
                        childWindow.hide()
                    }
                    checked = Qt.binding(() => childWindow?.visible)
                }
                Component {
                    id: emulatorWindowComponent
                    QmlDeckEmulatorContent {
                        x: root.x + root.width
                        y: root.y
                    }
                }
            }

            NameLabel {
                text: 'ExpectedType:'
            }
            ComboBox {
                id: expectedTypeComboBox
                textRole: 'text'
                valueRole: 'value'
                onActivated: deck.expectedDeviceType = currentValue
                Component.onCompleted: currentIndex = indexOfValue(deck.expectedDeviceType)
                model: [
                    { value: StreamDeckType.STREAMDECK_MINI, text: qsTr('STREAMDECK_MINI') },
                    { value: StreamDeckType.STREAMDECK_ORIGINAL, text: qsTr('STREAMDECK_ORIGINAL') },
                    { value: StreamDeckType.STREAMDECK_MK2, text: qsTr('STREAMDECK_MK2') },
                    { value: StreamDeckType.STREAMDECK_XL, text: qsTr('STREAMDECK_XL') },
                    { value: StreamDeckType.STREAMDECK_PEDAL, text: qsTr('STREAMDECK_PEDAL') },
                    { value: StreamDeckType.STREAMDECK_ANY, text: qsTr('STREAMDECK_ANY') }
                ]
            }

            NameLabel {
                text: 'ConnectedType:'
            }
            ValueLabel {
                text: deck.connected ? deck.connectedDeviceType : 'disconnected'
            }

            NameLabel {
                text: 'Description:'
            }
            ValueLabel {
                text: '${deck.manufacturer} | ${deck.modelName} | ${deck.serialNumber} | ${deck.firmwareVersion}'
            }

            NameLabel {
                text: 'Brightness:'
            }
            Slider {
                from: 0
                to: 100
                value: deck.brightness
                Layout.preferredWidth: expectedTypeComboBox.implicitWidth
                enabled: deck.connected && deck.isOpen && deck.hasDisplay
                onPressedChanged: {
                    if (!pressed) {
                        deck.brightness = value;
                        value = Qt.binding(() => deck.brightness)
                    }
                }
            }

            NameLabel {
                text: 'Last pressed:'
            }
            ValueLabel {
                id: textIndex
            }
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
                    color: '#303030'
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
    }
}

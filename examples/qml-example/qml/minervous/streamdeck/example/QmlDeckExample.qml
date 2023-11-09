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
        //expectedDeviceType: StreamDeck.Mini
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
                    for (var i = 0; i < keyCount; ++i) {
                        setImageUrl(i, normalImage)
                    }
                }
            } else if (isOpen) {
                close()
            }
        }

        Component.onCompleted: {
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
                    StreamDeckEmulatorWindow {
                        x: root.x + root.width
                        y: root.y
                    }
                }
            }

            NameLabel {
                text: 'ExpectedType:'
            }
            ComboBox {
                textRole: 'text'
                valueRole: 'value'
                onActivated: deck.expectedDeviceType = currentValue
                model: [
                    { value: StreamDeck.Mini, text: qsTr('Mini') },
                    { value: StreamDeck.Original, text: qsTr('Original') },
                    { value: StreamDeck.MK2, text: qsTr('MK2') },
                    { value: StreamDeck.XL, text: qsTr('XL') },
                    { value: StreamDeck.Pedal, text: qsTr('Pedal') },
                    { value: StreamDeck.Any, text: qsTr('Any') }
                ]
                Component.onCompleted: currentIndex = indexOfValue(deck.expectedDeviceType)
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
                text: [deck.manufacturer, deck.modelName, deck.serialNumber, deck.firmwareVersion].join(' | ')
            }

            NameLabel {
                text: 'Brightness:'
            }
            Slider {
                from: 0
                to: 100
                value: deck.brightness
                Layout.preferredWidth: 200
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

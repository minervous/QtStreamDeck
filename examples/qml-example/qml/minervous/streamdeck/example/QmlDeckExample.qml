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

    function deviceTypeToString(type)
    {
        switch(type) {
        case StreamDeck.Original  :
            return 'Stream Deck Original'
        case StreamDeck.OriginalV2:
            return 'Stream Deck OriginalV2'
        case StreamDeck.MK2       :
            return 'Stream Deck MK2'
        case StreamDeck.Mini      :
            return 'Stream Deck Mini'
        case StreamDeck.MiniMK2   :
            return 'Stream Deck MiniMK2'
        case StreamDeck.XL        :
            return 'Stream Deck XL'
        case StreamDeck.XLV2      :
            return 'Stream Deck XLV2'
        case StreamDeck.Pedal     :
            return 'Stream Deck Pedal'
        case StreamDeck.Any       :
            return 'Any Stream Deck'
        case StreamDeck.Unknown   :
        default:
            return 'Unknown device / diconnected'
        }
    }

    onClosing: {
        childWindow?.close();
    }

    StreamDeck {
        id: deck

        property url pressedImage: 'qrc:/examples/images/Pressed.png'
        property url normalImage: 'qrc:/examples/images/Released.png'

        onPressed: (index) => {
            textIndex.text = index
        }

        onIsOpenChanged: {
            if (isOpen) {
                for (let i = 0; i < keyCount; ++i) {
                    sendImage(i, normalImage)
                }
            }
        }

        Component.onCompleted: {
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
                model: ListModel {}
                Component.onCompleted: {
                    const types = [StreamDeck.Any, StreamDeck.Mini, StreamDeck.Original, StreamDeck.MK2, StreamDeck.XL, StreamDeck.Pedal];
                    for(const t of types)
                    {
                        model.append({'value': t, 'text': root.deviceTypeToString(t)});
                    }
                    currentIndex = indexOfValue(deck.expectedDeviceType)
                }
            }

            NameLabel {
                text: 'ConnectedType:'
            }
            ValueLabel {
                text: root.deviceTypeToString(deck.deviceType)
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
                            deck.sendImage(index, source)
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

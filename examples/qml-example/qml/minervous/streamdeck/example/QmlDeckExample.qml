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
        case StreamDeck.Original:
            return 'Stream Deck Original'
        case StreamDeck.OriginalV2:
            return 'Stream Deck OriginalV2'
        case StreamDeck.MK2:
            return 'Stream Deck MK2'
        case StreamDeck.Mini:
            return 'Stream Deck Mini'
        case StreamDeck.MiniMK2:
            return 'Stream Deck MiniMK2'
        case StreamDeck.XL:
            return 'Stream Deck XL'
        case StreamDeck.XLV2:
            return 'Stream Deck XLV2'
        case StreamDeck.Pedal:
            return 'Stream Deck Pedal'
        case StreamDeck.Any:
            return 'Any Stream Deck'
        case StreamDeck.Unknown:
        default:
            return 'Unknown device / diconnected'
        }
    }

    onClosing: {
        childWindow?.close();
    }

    StreamDeckKeyModel {
        id: keyModel

        StreamDeckKeyEntry {
            id: keyForGrabbedImage
            image: deck.grabbedImage

            onKeyPressed: {
                itemToGrab.startAnimation();
            }
            Component.onCompleted: {
                itemToGrab.grabAndSend();
            }
        }

        Instantiator {
            id: inst
            model: deck.keyCount ? deck.keyCount - 1 : 0

            delegate: StreamDeckKeyEntry {
                imageSource: pressed ? deck.pressedImage : deck.normalImage
                onKeyReleased: {
                    console.warn('Instantiator delegate', index)
                }
            }
        }
    }

    StreamDeck {
        id: deck

        property url pressedImage: 'qrc:/examples/images/Pressed.png'
        property url normalImage: 'data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAEgAAABICAYAAABV7bNHAAABJWlDQ1BrQ0dDb2xvclNwYWNlQWRvYmVSR0IxOTk4AAAokWNgYFJILCjIYRJgYMjNKykKcndSiIiMUmB/zsDNwAeEOgyWicnFBY4BAT4MQACjUcG3awyMIPqyLsgsTHm8gCsltTgZSP8B4uzkgqISBgbGDCBbubykAMTuAbJFkrLB7AUgdhHQgUD2FhA7HcI+AVYDYd8BqwkJcgayPwDZfElgNhPILr50CFsAxIbaCwKCjin5SakKIN9rGFpaWmiS6AeCoCS1ogREO+cXVBZlpmeUKDgCQypVwTMvWU9HwcjAyJiBARTuENWfA8HhySh2BiGGAAixORIMDP5LGRhY/iDETHoZGBboMDDwT0WIqRkyMAjoMzDsm5NcWlQGNYaRCWgnIT4AgwtKiH2vsnIAAAE8ZVhJZk1NACoAAAAIAAkBDgACAAAAOwAAAHoBEgADAAAAAQABAAABGgAFAAAAAQAAALYBGwAFAAAAAQAAAL4BKAADAAAAAQACAAABMQACAAAADQAAAMYBOwACAAAAIwAAANSCmAACAAAADgAAAPiHaQAEAAAAAQAAAQYAAAAAT0sgZW1vamkgIHBuZyBzdGlja2VyLCAzRCByZW5kZXJpbmcgdHJhbnNwYXJlbnQgYmFja2dyb3VuZAAAAAABLAAAAAEAAAEsAAAAAXJhd3BpeGVsLmNvbQAAcmF3cGl4ZWwuY29tIC8gU2FrYXJpbiBTdWttYW5hdGhhbQAAUmF3cGl4ZWwgTHRkLgAABJAAAAcAAAAEMDIxMKAAAAcAAAAEMDEwMKACAAQAAAABAAAASKADAAQAAAABAAAASAAAAACuerpmAAAACXBIWXMAAC4jAAAuIwF4pT92AAALHmlUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iWE1QIENvcmUgNi4wLjAiPgogICA8cmRmOlJERiB4bWxuczpyZGY9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkvMDIvMjItcmRmLXN5bnRheC1ucyMiPgogICAgICA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIgogICAgICAgICAgICB4bWxuczp4bXBSaWdodHM9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9yaWdodHMvIgogICAgICAgICAgICB4bWxuczpleGlmPSJodHRwOi8vbnMuYWRvYmUuY29tL2V4aWYvMS4wLyIKICAgICAgICAgICAgeG1sbnM6dGlmZj0iaHR0cDovL25zLmFkb2JlLmNvbS90aWZmLzEuMC8iCiAgICAgICAgICAgIHhtbG5zOmRjPSJodHRwOi8vcHVybC5vcmcvZGMvZWxlbWVudHMvMS4xLyIKICAgICAgICAgICAgeG1sbnM6cGx1cz0iaHR0cDovL25zLnVzZXBsdXMub3JnL2xkZi94bXAvMS4wLyIKICAgICAgICAgICAgeG1sbnM6eG1wPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIj4KICAgICAgICAgPHhtcFJpZ2h0czpXZWJTdGF0ZW1lbnQ+aHR0cHM6Ly93d3cucmF3cGl4ZWwuY29tL3NlcnZpY2VzL2xpY2Vuc2VzPC94bXBSaWdodHM6V2ViU3RhdGVtZW50PgogICAgICAgICA8ZXhpZjpDb2xvclNwYWNlPjY1NTM1PC9leGlmOkNvbG9yU3BhY2U+CiAgICAgICAgIDxleGlmOlBpeGVsWERpbWVuc2lvbj4xMDA8L2V4aWY6UGl4ZWxYRGltZW5zaW9uPgogICAgICAgICA8ZXhpZjpFeGlmVmVyc2lvbj4wMjEwPC9leGlmOkV4aWZWZXJzaW9uPgogICAgICAgICA8ZXhpZjpGbGFzaFBpeFZlcnNpb24+MDEwMDwvZXhpZjpGbGFzaFBpeFZlcnNpb24+CiAgICAgICAgIDxleGlmOlBpeGVsWURpbWVuc2lvbj4xMDA8L2V4aWY6UGl4ZWxZRGltZW5zaW9uPgogICAgICAgICA8dGlmZjpSZXNvbHV0aW9uVW5pdD4yPC90aWZmOlJlc29sdXRpb25Vbml0PgogICAgICAgICA8dGlmZjpPcmllbnRhdGlvbj4xPC90aWZmOk9yaWVudGF0aW9uPgogICAgICAgICA8dGlmZjpYUmVzb2x1dGlvbj4zMDA8L3RpZmY6WFJlc29sdXRpb24+CiAgICAgICAgIDx0aWZmOllSZXNvbHV0aW9uPjMwMDwvdGlmZjpZUmVzb2x1dGlvbj4KICAgICAgICAgPGRjOnRpdGxlPgogICAgICAgICAgICA8cmRmOkFsdD4KICAgICAgICAgICAgICAgPHJkZjpsaSB4bWw6bGFuZz0ieC1kZWZhdWx0Ij5PSyBlbW9qaSAgcG5nIHN0aWNrZXIsIDNEIHJlbmRlcmluZyB0cmFuc3BhcmVudCBiYWNrZ3JvdW5kPC9yZGY6bGk+CiAgICAgICAgICAgIDwvcmRmOkFsdD4KICAgICAgICAgPC9kYzp0aXRsZT4KICAgICAgICAgPGRjOmRlc2NyaXB0aW9uPgogICAgICAgICAgICA8cmRmOkFsdD4KICAgICAgICAgICAgICAgPHJkZjpsaSB4bWw6bGFuZz0ieC1kZWZhdWx0Ij5PSyBlbW9qaSAgcG5nIHN0aWNrZXIsIDNEIHJlbmRlcmluZyB0cmFuc3BhcmVudCBiYWNrZ3JvdW5kPC9yZGY6bGk+CiAgICAgICAgICAgIDwvcmRmOkFsdD4KICAgICAgICAgPC9kYzpkZXNjcmlwdGlvbj4KICAgICAgICAgPGRjOnJpZ2h0cz4KICAgICAgICAgICAgPHJkZjpBbHQ+CiAgICAgICAgICAgICAgIDxyZGY6bGkgeG1sOmxhbmc9IngtZGVmYXVsdCI+UmF3cGl4ZWwgTHRkLjwvcmRmOmxpPgogICAgICAgICAgICA8L3JkZjpBbHQ+CiAgICAgICAgIDwvZGM6cmlnaHRzPgogICAgICAgICA8ZGM6c3ViamVjdD4KICAgICAgICAgICAgPHJkZjpCYWc+CiAgICAgICAgICAgICAgIDxyZGY6bGk+b2sgZW1vamk8L3JkZjpsaT4KICAgICAgICAgICAgICAgPHJkZjpsaT4zZCBvayBoYW5kPC9yZGY6bGk+CiAgICAgICAgICAgICAgIDxyZGY6bGk+ZW1vamk8L3JkZjpsaT4KICAgICAgICAgICAgICAgPHJkZjpsaT5zbWlsZXkgZmFjZTwvcmRmOmxpPgogICAgICAgICAgICAgICA8cmRmOmxpPnllcyBlbW9qaTwvcmRmOmxpPgogICAgICAgICAgICAgICA8cmRmOmxpPmVtb2ppIHBuZzwvcmRmOmxpPgogICAgICAgICAgICAgICA8cmRmOmxpPmZhY2UgM2QgaWxsdXN0cmF0aW9uPC9yZGY6bGk+CiAgICAgICAgICAgICAgIDxyZGY6bGk+MyBkaW1lbnNpb25hbDwvcmRmOmxpPgogICAgICAgICAgICAgICA8cmRmOmxpPjNkPC9yZGY6bGk+CiAgICAgICAgICAgICAgIDxyZGY6bGk+M2QgZW1vamk8L3JkZjpsaT4KICAgICAgICAgICAgICAgPHJkZjpsaT4zZCBlbW90aWNvbjwvcmRmOmxpPgogICAgICAgICAgICAgICA8cmRmOmxpPjNkIGdyYXBoaWNzPC9yZGY6bGk+CiAgICAgICAgICAgIDwvcmRmOkJhZz4KICAgICAgICAgPC9kYzpzdWJqZWN0PgogICAgICAgICA8ZGM6Y3JlYXRvcj4KICAgICAgICAgICAgPHJkZjpTZXE+CiAgICAgICAgICAgICAgIDxyZGY6bGk+cmF3cGl4ZWwuY29tIC8gU2FrYXJpbiBTdWttYW5hdGhhbTwvcmRmOmxpPgogICAgICAgICAgICA8L3JkZjpTZXE+CiAgICAgICAgIDwvZGM6Y3JlYXRvcj4KICAgICAgICAgPHBsdXM6TGljZW5zb3I+CiAgICAgICAgICAgIDxyZGY6U2VxPgogICAgICAgICAgICAgICA8cmRmOmxpIHJkZjpwYXJzZVR5cGU9IlJlc291cmNlIj4KICAgICAgICAgICAgICAgICAgPHBsdXM6TGljZW5zb3JVUkw+aHR0cHM6Ly93d3cucmF3cGl4ZWwuY29tL2ltYWdlLzg2NjQ2Nzk8L3BsdXM6TGljZW5zb3JVUkw+CiAgICAgICAgICAgICAgIDwvcmRmOmxpPgogICAgICAgICAgICA8L3JkZjpTZXE+CiAgICAgICAgIDwvcGx1czpMaWNlbnNvcj4KICAgICAgICAgPHhtcDpSYXRpbmc+NjwveG1wOlJhdGluZz4KICAgICAgICAgPHhtcDpDcmVhdG9yVG9vbD5yYXdwaXhlbC5jb208L3htcDpDcmVhdG9yVG9vbD4KICAgICAgPC9yZGY6RGVzY3JpcHRpb24+CiAgIDwvcmRmOlJERj4KPC94OnhtcG1ldGE+ClyHT3MAACD8SURBVHgB7Zt5lF5nfd+/993fd/YZSTOaGUkjyZK8SDJeABswMksA4xJIQw87tJQ2OSdp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'
        property int lastPressedIndex: 0
        property int animatedKeyIndex: 0
        property url grabbedUrl
        property var grabbedImage

        model: keyModel

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
                text: deck.lastPressedIndex
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
                        source: index === deck.animatedKeyIndex ? deck.grabbedUrl : (deck.buttonsState[index] ? deck.pressedImage :
                                                                                                                deck.normalImage)
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
        Item {
            id: itemToGrab
            width: 300
            height: 300
            visible: deck.hasDisplay && deck.connected

            property real pressedScale: deck.buttonsState[deck.animatedKeyIndex] ? 0.8 : 1.0
            Behavior on pressedScale {
                NumberAnimation {
                    duration: 200;
                }
            }

            function startAnimation() {
                transition.stop()
                timer.stop()
                if (transition.direction === RotationAnimation.Counterclockwise) {
                    transition.direction = RotationAnimation.Clockwise
                } else {
                    transition.direction = RotationAnimation.Counterclockwise
                }

                transition.start()
                if (deck.hasDisplay && deck.connected) {
                    timer.count = 0
                    timer.start()
                }
            }

            function grabAndSend() {
                itemToGrab.grabToImage(function(result) {
                    deck.grabbedUrl = result.url
                    deck.grabbedImage = result.image;
                    //deck.sendImage(deck.animatedKeyIndex, result.image)
                    if (!transition.running && pressedScale >= 1.0) {
                        timer.stop();
                    }
                }, deck.originalKeyImageSize)
            }

            Rectangle {
                anchors.fill: parent
                color: 'indigo'
            }

            Rectangle {
                id: rect
                width: 200
                height: 200
                anchors.centerIn: parent
                color: 'red'
                antialiasing: true
                scale: itemToGrab.pressedScale

                states: State {
                    name: "rotated"
                    PropertyChanges { target: rect; rotation: 180 }
                }

                RotationAnimation {
                    id: transition
                    target: rect
                    from: 0
                    to: 180
                    duration: 1000;
                    direction: RotationAnimation.Counterclockwise
                }
            }

            Label {
                anchors.centerIn: parent
                font.pixelSize: 60
                font.bold: true
                text: timer.count
                color: 'white'
                antialiasing: true
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    itemToGrab.startAnimation();
                }
            }

            Timer {
                id: timer
                property int count: 0
                interval: 50
                repeat: true
                onTriggered: {
                    count++;
                    itemToGrab.grabAndSend()
                }
            }
        }
    }
}

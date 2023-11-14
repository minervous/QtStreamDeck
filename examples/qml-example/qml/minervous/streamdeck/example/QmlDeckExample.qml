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
        property url normalImage: "data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAGQAAABkCAIAAAD/gAIDAAABJWlDQ1BrQ0dDb2xvclNwYWNlQWRvYmVSR0IxOTk4AAAokWNgYFJILCjIYRJgYMjNKykKcndSiIiMUmB/zsDNwAeEUgyiicnFBY4BAT4MQACjUcG3awyMIPqyLsgsTHm8gCsltTgZSP8B4uzkgqISBgbGDCBbubykAMTuAbJFkrLB7AUgdhHQgUD2FhA7HcI+AVYDYd8BqwkJcgayPwDZfElgNhPILr50CFsAxIbaCwKCjin5SakKIN9rGFpaWmiS6AeCoCS1ogREO+cXVBZlpmeUKDgCQypVwTMvWU9HwcjAyJiBARTuENWfA8HhySh2BiGGAAixORIMDP5LGRhY/iDETHoZGBboMDDwT0WIqRkyMAjoMzDsm5NcWlQGNYaRCWgnIT4AF4lKUiv75KcAAAE8ZVhJZk1NACoAAAAIAAkBDgACAAAAOwAAAHoBEgADAAAAAQABAAABGgAFAAAAAQAAALYBGwAFAAAAAQAAAL4BKAADAAAAAQACAAABMQACAAAADQAAAMYBOwACAAAAIwAAANSCmAACAAAADgAAAPiHaQAEAAAAAQAAAQYAAAAAT0sgZW1vamkgIHBuZyBzdGlja2VyLCAzRCByZW5kZXJpbmcgdHJhbnNwYXJlbnQgYmFja2dyb3VuZAAAAAABLAAAAAEAAAEsAAAAAXJhd3BpeGVsLmNvbQAAcmF3cGl4ZWwuY29tIC8gU2FrYXJpbiBTdWttYW5hdGhhbQAAUmF3cGl4ZWwgTHRkLgAABJAAAAcAAAAEMDIxMKAAAAcAAAAEMDEwMKACAAQAAAABAAAAZKADAAQAAAABAAAAZAAAAAAR8CLIAAAACXBIWXMAAC4jAAAuIwF4pT92AAALHmlUWHRYTUw6Y29tLmFkb2JlLnhtcAAAAAAAPHg6eG1wbWV0YSB4bWxuczp4PSJhZG9iZTpuczptZXRhLyIgeDp4bXB0az0iWE1QIENvcmUgNi4wLjAiPgogICA8cmRmOlJERiB4bWxuczpyZGY9Imh0dHA6Ly93d3cudzMub3JnLzE5OTkvMDIvMjItcmRmLXN5bnRheC1ucyMiPgogICAgICA8cmRmOkRlc2NyaXB0aW9uIHJkZjphYm91dD0iIgogICAgICAgICAgICB4bWxuczp4bXBSaWdodHM9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC9yaWdodHMvIgogICAgICAgICAgICB4bWxuczpleGlmPSJodHRwOi8vbnMuYWRvYmUuY29tL2V4aWYvMS4wLyIKICAgICAgICAgICAgeG1sbnM6dGlmZj0iaHR0cDovL25zLmFkb2JlLmNvbS90aWZmLzEuMC8iCiAgICAgICAgICAgIHhtbG5zOmRjPSJodHRwOi8vcHVybC5vcmcvZGMvZWxlbWVudHMvMS4xLyIKICAgICAgICAgICAgeG1sbnM6cGx1cz0iaHR0cDovL25zLnVzZXBsdXMub3JnL2xkZi94bXAvMS4wLyIKICAgICAgICAgICAgeG1sbnM6eG1wPSJodHRwOi8vbnMuYWRvYmUuY29tL3hhcC8xLjAvIj4KICAgICAgICAgPHhtcFJpZ2h0czpXZWJTdGF0ZW1lbnQ+aHR0cHM6Ly93d3cucmF3cGl4ZWwuY29tL3NlcnZpY2VzL2xpY2Vuc2VzPC94bXBSaWdodHM6V2ViU3RhdGVtZW50PgogICAgICAgICA8ZXhpZjpDb2xvclNwYWNlPjY1NTM1PC9leGlmOkNvbG9yU3BhY2U+CiAgICAgICAgIDxleGlmOlBpeGVsWERpbWVuc2lvbj44MDA8L2V4aWY6UGl4ZWxYRGltZW5zaW9uPgogICAgICAgICA8ZXhpZjpFeGlmVmVyc2lvbj4wMjEwPC9leGlmOkV4aWZWZXJzaW9uPgogICAgICAgICA8ZXhpZjpGbGFzaFBpeFZlcnNpb24+MDEwMDwvZXhpZjpGbGFzaFBpeFZlcnNpb24+CiAgICAgICAgIDxleGlmOlBpeGVsWURpbWVuc2lvbj44MDA8L2V4aWY6UGl4ZWxZRGltZW5zaW9uPgogICAgICAgICA8dGlmZjpSZXNvbHV0aW9uVW5pdD4yPC90aWZmOlJlc29sdXRpb25Vbml0PgogICAgICAgICA8dGlmZjpPcmllbnRhdGlvbj4xPC90aWZmOk9yaWVudGF0aW9uPgogICAgICAgICA8dGlmZjpYUmVzb2x1dGlvbj4zMDA8L3RpZmY6WFJlc29sdXRpb24+CiAgICAgICAgIDx0aWZmOllSZXNvbHV0aW9uPjMwMDwvdGlmZjpZUmVzb2x1dGlvbj4KICAgICAgICAgPGRjOnRpdGxlPgogICAgICAgICAgICA8cmRmOkFsdD4KICAgICAgICAgICAgICAgPHJkZjpsaSB4bWw6bGFuZz0ieC1kZWZhdWx0Ij5PSyBlbW9qaSAgcG5nIHN0aWNrZXIsIDNEIHJlbmRlcmluZyB0cmFuc3BhcmVudCBiYWNrZ3JvdW5kPC9yZGY6bGk+CiAgICAgICAgICAgIDwvcmRmOkFsdD4KICAgICAgICAgPC9kYzp0aXRsZT4KICAgICAgICAgPGRjOmRlc2NyaXB0aW9uPgogICAgICAgICAgICA8cmRmOkFsdD4KICAgICAgICAgICAgICAgPHJkZjpsaSB4bWw6bGFuZz0ieC1kZWZhdWx0Ij5PSyBlbW9qaSAgcG5nIHN0aWNrZXIsIDNEIHJlbmRlcmluZyB0cmFuc3BhcmVudCBiYWNrZ3JvdW5kPC9yZGY6bGk+CiAgICAgICAgICAgIDwvcmRmOkFsdD4KICAgICAgICAgPC9kYzpkZXNjcmlwdGlvbj4KICAgICAgICAgPGRjOnJpZ2h0cz4KICAgICAgICAgICAgPHJkZjpBbHQ+CiAgICAgICAgICAgICAgIDxyZGY6bGkgeG1sOmxhbmc9IngtZGVmYXVsdCI+UmF3cGl4ZWwgTHRkLjwvcmRmOmxpPgogICAgICAgICAgICA8L3JkZjpBbHQ+CiAgICAgICAgIDwvZGM6cmlnaHRzPgogICAgICAgICA8ZGM6c3ViamVjdD4KICAgICAgICAgICAgPHJkZjpCYWc+CiAgICAgICAgICAgICAgIDxyZGY6bGk+b2sgZW1vamk8L3JkZjpsaT4KICAgICAgICAgICAgICAgPHJkZjpsaT4zZCBvayBoYW5kPC9yZGY6bGk+CiAgICAgICAgICAgICAgIDxyZGY6bGk+ZW1vamk8L3JkZjpsaT4KICAgICAgICAgICAgICAgPHJkZjpsaT5zbWlsZXkgZmFjZTwvcmRmOmxpPgogICAgICAgICAgICAgICA8cmRmOmxpPnllcyBlbW9qaTwvcmRmOmxpPgogICAgICAgICAgICAgICA8cmRmOmxpPmVtb2ppIHBuZzwvcmRmOmxpPgogICAgICAgICAgICAgICA8cmRmOmxpPmZhY2UgM2QgaWxsdXN0cmF0aW9uPC9yZGY6bGk+CiAgICAgICAgICAgICAgIDxyZGY6bGk+MyBkaW1lbnNpb25hbDwvcmRmOmxpPgogICAgICAgICAgICAgICA8cmRmOmxpPjNkPC9yZGY6bGk+CiAgICAgICAgICAgICAgIDxyZGY6bGk+M2QgZW1vamk8L3JkZjpsaT4KICAgICAgICAgICAgICAgPHJkZjpsaT4zZCBlbW90aWNvbjwvcmRmOmxpPgogICAgICAgICAgICAgICA8cmRmOmxpPjNkIGdyYXBoaWNzPC9yZGY6bGk+CiAgICAgICAgICAgIDwvcmRmOkJhZz4KICAgICAgICAgPC9kYzpzdWJqZWN0PgogICAgICAgICA8ZGM6Y3JlYXRvcj4KICAgICAgICAgICAgPHJkZjpTZXE+CiAgICAgICAgICAgICAgIDxyZGY6bGk+cmF3cGl4ZWwuY29tIC8gU2FrYXJpbiBTdWttYW5hdGhhbTwvcmRmOmxpPgogICAgICAgICAgICA8L3JkZjpTZXE+CiAgICAgICAgIDwvZGM6Y3JlYXRvcj4KICAgICAgICAgPHBsdXM6TGljZW5zb3I+CiAgICAgICAgICAgIDxyZGY6U2VxPgogICAgICAgICAgICAgICA8cmRmOmxpIHJkZjpwYXJzZVR5cGU9IlJlc291cmNlIj4KICAgICAgICAgICAgICAgICAgPHBsdXM6TGljZW5zb3JVUkw+aHR0cHM6Ly93d3cucmF3cGl4ZWwuY29tL2ltYWdlLzg2NjQ2Nzk8L3BsdXM6TGljZW5zb3JVUkw+CiAgICAgICAgICAgICAgIDwvcmRmOmxpPgogICAgICAgICAgICA8L3JkZjpTZXE+CiAgICAgICAgIDwvcGx1czpMaWNlbnNvcj4KICAgICAgICAgPHhtcDpSYXRpbmc+NjwveG1wOlJhdGluZz4KICAgICAgICAgPHhtcDpDcmVhdG9yVG9vbD5yYXdwaXhlbC5jb208L3htcDpDcmVhdG9yVG9vbD4KICAgICAgPC9yZGY6RGVzY3JpcHRpb24+CiAgIDwvcmRmOlJERj4KPC94OnhtcG1ldGE+ChDEYTsAACv4SURBVHgB7ZwJsF1HeefPfre33Se9TbtkyZIlL8IbxtjYsQ0xGJslATIhMDCTCSFDJamZ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";
        property int lastPressedIndex: 0
        property int animatedKeyIndex: 0
        property url grabbedUrl

        onPressed: (index) => {
           lastPressedIndex = index
           if (index === animatedKeyIndex && hasDisplay) {
               itemToGrab.startAnimation();
           }
       }

        onIsOpenChanged: {
            if (isOpen && hasDisplay) {
                for (let i = 0; i < keyCount - 1; ++i) {
                    if (i !== animatedKeyIndex)
                        sendImage(i, normalImage)
                }
                itemToGrab.grabAndSend();
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

                        onSourceChanged: {
                            if (index !== deck.animatedKeyIndex)
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
                    deck.sendImage(deck.animatedKeyIndex, result.image)
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

import QtQuick
import StreamDeckQml

Window {

    width: 640
    height: 400
    visible: true
    title: qsTr('Example Project')

    StreamDeck {
        id: deck
        //expectedDeviceType: StreamDeck.STREAMDECK_MINI
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
                }
            } else if (isOpen){
                close()
            }
        }

        Component.onCompleted: {
            init()
            reopen()
            console.info(StreamDeckManager.devices)
            //expectedDeviceType = StreamDeckType.STREAMDECK_MK2
        }
    }

    Column {
        Text {
            id: textType
            text: 'Type: ' + deck.expectedDeviceType + '/' + deck.connectedDeviceType + ' ' + deck.keyRows + 'x' + deck.keyColumns + (deck.connected ? ' connected' : ' disconnected')
        }
        Grid {
            id: grid
            rows: deck.keyRows
            columns:  deck.keyColumns
            spacing: 20
            Repeater {
                model: deck.keyCount
                delegate: Rectangle {
                    width: 50
                    height: 50
                    color: deck.buttonsState[index] ? 'blue' : 'red'
                }
            }
        }
        Text {
            id: textIndex
        }
    }
}

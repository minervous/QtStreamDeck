import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Templates as T
import Qt5Compat.GraphicalEffects

import minervous.streamdeck

Window {
	id: root
	minimumHeight: contentColumn.implicitHeight + 20
	minimumWidth: Math.max(contentColumn.implicitWidth, devicePanel.width) + 40
	title: qsTr('Stream Deck Emulator')

	color: '#555'
	palette.windowText: '#e7dbb6'

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
		case StreamDeck.Plus:
			return 'Stream Deck +'
		case StreamDeck.Any:
			return 'Any Stream Deck'
		case StreamDeck.Unknown:
		default:
			return 'Unknown device'
		}
	}

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

		onImageSent: (keyIndex, imageDataBin, imageDataBase64) => {
			images[keyIndex] = imageDataBase64
		}
	}

	component NameLabel: Label {
		font.bold: true
	}
	component ValueLabel: Label {
	}
	component ValueTextField: TextField {
		Layout.preferredWidth: 200
	}

	Column {
		id: contentColumn
		anchors.horizontalCenter: parent.horizontalCenter
		spacing: 5

		GridLayout {
			columns:  2
			width: parent.width
			columnSpacing: 5
			rowSpacing: 5

			NameLabel {
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
			NameLabel {
				text: 'Device type:'
				font.bold: true
			}
			ComboBox {
				textRole: 'text'
				valueRole: 'value'
				onActivated: emulator.deviceType = currentValue
				model: ListModel {}
				Component.onCompleted: {
					const types = [StreamDeck.Mini, StreamDeck.Original, StreamDeck.MK2, StreamDeck.XL, StreamDeck.Pedal, StreamDeck.Plus];
					for(const t of types)
					{
						model.append({'value': t, 'text': root.deviceTypeToString(t)});
					}
					currentIndex = indexOfValue(emulator.deviceType)
				}
			}

			NameLabel {
				text: 'Manufacturer:'
				font.bold: true
			}
			ValueTextField {
				text: emulator.manufacturer
				onEditingFinished: {
					emulator.manufacturer = text
				}
			}

			NameLabel {
				text: 'Product:'
				font.bold: true
			}
			ValueTextField {
				text: emulator.modelName
				onEditingFinished: {
					emulator.modelName = text
				}
			}

			NameLabel {
				text: 'Serial number:'
				font.bold: true
			}
			ValueTextField {
				text: emulator.serialNumber
				onEditingFinished: {
					emulator.serialNumber = text
				}
			}

			NameLabel {
				text: 'Firmware version:'
				font.bold: true
			}
			ValueTextField {
				text: emulator.firmwareVersion
				onEditingFinished: {
					emulator.firmwareVersion = text
				}
			}

			NameLabel {
				text: 'Brightness:'
				font.bold: true
			}
			ValueLabel {
				text: emulator.hasDisplay ? emulator.brightness : '---'
			}

			NameLabel {
				text: 'isOpen:'
				font.bold: true
			}
			ValueLabel {
				text: emulator.isOpen
			}
		}

		Rectangle {
			id: devicePanel
			property int sizeUnit: 40
			property int buttonSize: sizeUnit * 1.6
			property int buttonSpacing: devicePanel.sizeUnit * 0.3
			property real brightnessOpacity: emulator.hasDisplay ? 0.05 + (emulator.brightness / 100.0 * 0.95) : 1.0
			property int regularContentMargin: sizeUnit * 1.2
			property int topContentMargin: sizeUnit * 1.7
			width: emulatorGrid.width + regularContentMargin * 2
			height: emulatorGrid.height + regularContentMargin + topContentMargin
			anchors.horizontalCenter: parent.horizontalCenter
			color: '#303030'
			radius: sizeUnit * 0.4

			Label {
				color: 'white'
				anchors.horizontalCenter: parent.horizontalCenter
				anchors.verticalCenter: parent.top
				anchors.verticalCenterOffset: devicePanel.topContentMargin / 2
				text: 'STREAM DECK'
				font.bold: true
				font.pixelSize: devicePanel.topContentMargin * 0.3
			}

			Grid {
				id: emulatorGrid
				anchors.horizontalCenter: parent.horizontalCenter
				anchors.top: parent.top
				anchors.topMargin: devicePanel.topContentMargin
				rows: emulator.keyRows
				columns:  emulator.keyColumns
				spacing: devicePanel.buttonSpacing

				Repeater {
					model: emulator.keyCount
					delegate: T.Button {
						id: button
						implicitWidth: devicePanel.buttonSize
						implicitHeight: devicePanel.buttonSize
						background: Item {
							Rectangle {
								id: backgroundRoundRect
								anchors.fill: parent
								opacity: devicePanel.brightnessOpacity
								radius: devicePanel.sizeUnit * 0.2
								color: '#383838'
							}
							Rectangle {
								anchors.fill: parent
								anchors.margins: - border.width
								color: 'transparent'
								border.color: button.down ? 'blue' : '#5a5a5a'
								border.width: button.down ? 3 : 2
								radius: backgroundRoundRect.radius + border.width
							}
						}
						contentItem: Image {
							opacity: devicePanel.brightnessOpacity
							source: emulator.images?.[index] ?? ''
							layer.effect: OpacityMask {
								maskSource: backgroundRoundRect
							}
							layer.enabled: true
						}

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
				}
			}
		}
	}
}

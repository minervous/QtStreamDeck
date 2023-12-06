import QtQuick

import minervous.streamdeck

StreamDeckKeyEntry {
	id: root
	required property size keySize
	property alias attachingVisualParent: itemGrabber.attachingVisualParent
	property alias width: keyItem.width
	property alias height: keyItem.height
	default property alias content: keyItem.data

	image: itemGrabber.image

	function updateKey() {
		itemGrabber.grab()
	}

	Item {
		id: keyItem
		width: root.keySize.width
		height: root.keySize.height
		visible: false
	}

	ItemGrabber {
		id: itemGrabber
		item: keyItem
		targetSize: root.keySize
	}

	Component.onCompleted: updateKey()
}

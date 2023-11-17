#pragma once

#include <QtQml/QQmlEngine>

#include "minervous/streamdeck/BaseKeyEntry.hpp"

namespace minervous::streamdeck
{

	struct QmlBaseKeyEntry
	{
		Q_GADGET
		QML_FOREIGN(minervous::streamdeck::BaseKeyEntry)
		QML_NAMED_ELEMENT(StreamDeckKeyEntry)
	};

}  // namespace minervous::streamdeck

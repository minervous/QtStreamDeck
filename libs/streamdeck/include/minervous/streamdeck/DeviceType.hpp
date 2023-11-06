#pragma once

#include <QtCore/QDebug>
#include <QtCore/QObject>

#include "StreamDeckLib_global.hpp"

#include "QtCore/qmetaobject.h"

namespace minervous::streamdeck
{

	Q_NAMESPACE

	class STREAMDECKLIB_EXPORT DeviceTypeGadget
	{
		Q_GADGET

	public:
		enum Type
		{
			UNKNOWN_DEVICE = 0,
			STREAMDECK_ORIGINAL,
			STREAMDECK_ORIGINAL_V2,
			STREAMDECK_MK2,
			STREAMDECK_MINI,
			STREAMDECK_MINI_MK2,
			STREAMDECK_XL,
			STREAMDECK_XL_V2,
			STREAMDECK_PEDAL,
			STREAMDECK_ANY
		};
		Q_ENUM(Type)
	};

	using DeviceType = DeviceTypeGadget::Type;

}  // namespace minervous::streamdeck

Q_DECLARE_METATYPE(minervous::streamdeck::DeviceTypeGadget::Type)

inline QTextStream & operator<<(QTextStream & outStream, const minervous::streamdeck::DeviceTypeGadget::Type & value)
{
	const QString notValidValue = QStringLiteral("Not valid value");
	QString enumToString{QMetaEnum::fromType<minervous::streamdeck::DeviceTypeGadget::Type>().valueToKey(value)};
	outStream << (enumToString.isEmpty() ? notValidValue : enumToString);
	return outStream;
}

inline QDebug & operator<<(QDebug & outStream, const minervous::streamdeck::DeviceTypeGadget::Type & value)
{
	const QString notValidValue = QStringLiteral("Not valid value");
	QString enumToString{QMetaEnum::fromType<minervous::streamdeck::DeviceTypeGadget::Type>().valueToKey(value)};
	outStream << (enumToString.isEmpty() ? notValidValue : enumToString);
	return outStream;
}

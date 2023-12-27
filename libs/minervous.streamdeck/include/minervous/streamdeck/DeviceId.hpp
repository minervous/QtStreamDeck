// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtCore/QDebug>
#include <QtCore/QObject>

#include "Device.hpp"
#include "LibExport.hpp"

namespace minervous::streamdeck
{

	class STREAMDECKLIB_EXPORT DeviceId
	{
		Q_GADGET

		Q_PROPERTY(minervous::streamdeck::Device::DeviceType type MEMBER type FINAL)
		Q_PROPERTY(QString serialNumber MEMBER serialNumber FINAL)

	public:
		DeviceId() = default;
		~DeviceId() = default;
		DeviceId(const DeviceId & rhs) = default;
		DeviceId & operator=(const DeviceId &) = default;

		DeviceId(const DeviceType type, const QString serialNumber = {})
			: type(type)
			, serialNumber(serialNumber)
		{}

		bool operator==(const DeviceId & rhs) const { return type == rhs.type && serialNumber == rhs.serialNumber; }

		bool operator<(const DeviceId & rhs) const
		{
			return type < rhs.type || (type == rhs.type && serialNumber < rhs.serialNumber);
		}

		bool operator!=(const DeviceId & rhs) const { return type != rhs.type || serialNumber != rhs.serialNumber; }

		DeviceType type = DeviceType::Unknown;
		QString serialNumber;
	};

}  // namespace minervous::streamdeck

Q_DECLARE_METATYPE(minervous::streamdeck::DeviceId)

inline QTextStream & operator<<(QTextStream & outStream, const minervous::streamdeck::DeviceId & value)
{
	outStream << QMetaType::fromType<minervous::streamdeck::DeviceId>().name() << "(type:" << value.type
			  << ", serialNumber:" << value.serialNumber << ")";
	return outStream;
}

inline QDebug & operator<<(QDebug & outStream, const minervous::streamdeck::DeviceId & value)
{
	outStream << QMetaType::fromType<minervous::streamdeck::DeviceId>().name() << "(type:" << value.type
			  << ", serialNumber:" << value.serialNumber << ")";
	return outStream;
}

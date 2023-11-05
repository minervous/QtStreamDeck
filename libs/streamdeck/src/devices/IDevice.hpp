#pragma once

#include <QtCore/QString>

#include <QtCore/qtmetamacros.h>

#include "StreamDeckLib_global.hpp"

namespace minervous::streamdeck
{

	class STREAMDECKLIB_EXPORT IDevice
	{
	public:
		IDevice() = default;
		virtual ~IDevice() = default;

		enum
		{
			VID = 0x0fd9
		};

		struct Configuration
		{
			int pid = 0;
			int vid = 0;
			int keyColumns = 0;
			int keyRows = 0;
			bool hasDisplay = false;
			int imageWidth = 0;
			int imageHeight = 0;
			QString imageFormat = {};
			bool imageHorizontalFlip = false;
			bool imageVerticalFlip = false;
			int imageRotation = 0;
		};

		virtual bool open(const QString & serial) = 0;
		virtual void close() = 0;
		virtual bool isOpen() const = 0;
		virtual QString manufacturer() const = 0;
		virtual QString serialNumber() const = 0;
		virtual QString product() const = 0;

		virtual const Configuration & getConfiguration() const = 0;
		virtual bool setBrightness(int percentage) = 0;
		virtual QString getFirmwareVersion() = 0;

		virtual bool reset() = 0;
		virtual int readButtonsStatus(QList<bool> & buttonsStates) = 0;

		virtual bool sendImage(int keyIndex, const QByteArray & imageData) = 0;

	protected:
		Q_DISABLE_COPY_MOVE(IDevice)
	};

}  // namespace minervous::streamdeck

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

		static const auto VID = 0x0fd9;

		enum class ImageFormat
		{
			Invalid,
			Bitmap,
			JPEG
		};

		struct Configuration
		{
			int pid = 0;
			int vid = VID;
			int keyColumns = 0;
			int keyRows = 0;
			bool hasDisplay = true;
			int imageWidth = 0;
			int imageHeight = imageWidth;
			ImageFormat imageFormat = ImageFormat::Invalid;
			bool imageHorizontalFlip = false;
			bool imageVerticalFlip = false;
			int imageRotation = 0;

			const char * imageFormatAsString() const
			{
				switch (imageFormat)
				{
				case ImageFormat::Bitmap:
					return "BMP";
				case ImageFormat::JPEG:
					return "JPEG";
				default:
					return "";
				}
			}
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

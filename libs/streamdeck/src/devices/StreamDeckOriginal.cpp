// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#include "StreamDeckOriginal.hpp"

using namespace minervous::streamdeck;

namespace
{
	static constexpr int REPORT_LENGTH = 18;
}

const IDevice::Configuration & StreamDeckOriginal::getConfiguration() const
{
	static const Configuration conf = createConfiguration();
	return conf;
}

IDevice::Configuration StreamDeckOriginal::createConfiguration()
{
	return {
		.pid = PID,
		.keyColumns = 5,
		.keyRows = 3,
		.imageWidth = 72,
		.imageFormat = ImageFormat::Bitmap,
		.imageHorizontalFlip = true,
		.imageVerticalFlip = true
	};
}

int StreamDeckOriginal::indexTransformation(int keyIndex)
{
	auto keyCol = keyIndex % getConfiguration().keyColumns;
	return (keyIndex - keyCol) + (getConfiguration().keyColumns - 1 - keyCol);
}

bool StreamDeckOriginal::setBrightness(int percentage)
{
	if (!_hid.isOpen())
	{
		return false;
	}

	QByteArray _send;
	_send.fill(0, REPORT_LENGTH);
	_send[0] = 0x05u;
	_send[1] = 0x55u;
	_send[2] = 0xAAu;
	_send[3] = 0xD1u;
	_send[4] = 0x01u;
	_send[5] = percentage % 101;  // brightness value [0..100]
	return _hid.sendFeatureReport(&_send) == _send.size();
}

QString StreamDeckOriginal::getFirmwareVersion()
{
	if (!_hid.isOpen())
	{
		return {};
	}

	QByteArray _send;
	_send.fill(0, REPORT_LENGTH);
	_send[0] = 0x04u;
	if (_send.size() == _hid.getFeatureReport(&_send))
	{
		return QString{_send.data() + 5};
	}
	else
	{
		return {};
	}
}

bool StreamDeckOriginal::reset()
{
	if (!_hid.isOpen())
	{
		return false;
	}

	QByteArray _send;
	_send.fill(0, REPORT_LENGTH);
	_send[0] = 0x0Bu;
	_send[1] = 0x63u;
	return _send.size() == _hid.sendFeatureReport(&_send);
}

int StreamDeckOriginal::readButtonsStatus(QList<bool> & buttonsStates)
{
	if (!_hid.isOpen())
	{
		return -1;
	}

	QByteArray readed;
	readed.fill(0, 16);
	int count = _hid.read(&readed, readed.size(), 0);
	if (count == readed.size())
	{
		for (int i(0); i < std::min(buttonsStates.size(), readed.size() - 1); ++i)
		{
			buttonsStates[indexTransformation(i)] = readed[1 + i] > 0;
		}
	}
	return count;
}

bool StreamDeckOriginal::sendImage(int keyIndex, const QByteArray & imageData)
{
	if (!_hid.isOpen())
	{
		return false;
	}

	bool success{true};
	constexpr int IMAGE_REPORT_LENGTH = 8191, IMAGE_REPORT_HEADER_LENGTH = 16,
				  IMAGE_REPORT_PAYLOAD_LENGTH = IMAGE_REPORT_LENGTH - IMAGE_REPORT_HEADER_LENGTH;

	int page_number = 0;
	int bytes_remaining = imageData.size();

	QByteArray header;
	header.fill(0, IMAGE_REPORT_HEADER_LENGTH);
	header[0] = 0x02u;
	header[1] = 0x01u;
	header[5] = static_cast<unsigned char>(indexTransformation(keyIndex) + 1);

	while (bytes_remaining > 0)
	{
		int this_length = std::min(bytes_remaining, IMAGE_REPORT_PAYLOAD_LENGTH);
		int bytes_sent = page_number * IMAGE_REPORT_PAYLOAD_LENGTH;

		header[2] = static_cast<unsigned char>(page_number + 1);
		header[4] = static_cast<unsigned char>(this_length == bytes_remaining ? 1 : 0);

		QByteArray payload;
		payload.reserve(IMAGE_REPORT_LENGTH);
		// Header
		payload.append(header);
		// Data
		payload.append(imageData.data() + bytes_sent, this_length);
		// Padding
		payload.append(IMAGE_REPORT_LENGTH - payload.size(), 0);
		if (payload.size() != _hid.write(&payload))
		{
			success = false;
			break;
		}
		bytes_remaining = bytes_remaining - this_length;
		page_number = page_number + 1;
	}

	return success;
}

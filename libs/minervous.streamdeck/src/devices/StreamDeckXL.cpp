// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#include "StreamDeckXL.hpp"

using namespace minervous::streamdeck;

StreamDeckXL::StreamDeckXL(quint16 pid)
	: AbstractDeviceBase()
	, _configuration{createConfiguration(pid)}
{}

const IDevice::Configuration & StreamDeckXL::getConfiguration() const
{
	return _configuration;
}

IDevice::Configuration StreamDeckXL::createConfiguration(quint16 pid)
{
	return {
		.pid = pid,
		.keyColumns = 8,
		.keyRows = 4,
		.imageWidth = 96,
		.imageFormat = ImageFormat::JPEG,
		.imageHorizontalFlip = true,
		.imageVerticalFlip = true,
	};
}

bool StreamDeckXL::setBrightness(int percentage)
{
	if (!_hid.isOpen())
	{
		return false;
	}

	QByteArray _send;
	_send.fill(0, 33);
	_send[0] = 0x03u;
	_send[1] = 0x08u;
	_send[2] = percentage % 101;  // brightness value [0..100]
	return _hid.sendFeatureReport(&_send) == _send.size();
}

QString StreamDeckXL::getFirmwareVersion()
{
	if (!_hid.isOpen())
	{
		return {};
	}

	QByteArray _send;
	_send.fill(0, 33);
	_send[0] = 0x05u;
	if (33 == _hid.getFeatureReport(&_send))
	{
		return QString{_send.data() + 6};
	}
	else
	{
		return {};
	}
}

bool StreamDeckXL::reset()
{
	if (!_hid.isOpen())
	{
		return false;
	}

	QByteArray _send;
	_send.fill(0, 33);
	_send[0] = 0x03u;
	_send[1] = 0x02u;
	return 33 == _hid.sendFeatureReport(&_send);
}

int StreamDeckXL::readButtonsStatus(QList<bool> & buttonsStates)
{
	if (!_hid.isOpen())
	{
		return -1;
	}

	QByteArray readed;
	readed.fill(0, 512);
	int count = _hid.read(&readed);
	if (count == readed.size())
	{
		for (int i(0); i < std::min(buttonsStates.size(), readed.size() - 4); ++i)
		{
			buttonsStates[i] = readed[4 + i] > 0;
		}
	}
	return count;
}

bool StreamDeckXL::sendImage(int keyIndex, const QByteArray & imageData)
{
	if (!_hid.isOpen())
	{
		return false;
	}

	bool success{true};
	constexpr int IMAGE_REPORT_LENGTH = 1024, IMAGE_REPORT_HEADER_LENGTH = 8,
				  IMAGE_REPORT_PAYLOAD_LENGTH = IMAGE_REPORT_LENGTH - IMAGE_REPORT_HEADER_LENGTH;

	int page_number = 0;
	int bytes_remaining = imageData.size();

	QByteArray header;
	header.resize(IMAGE_REPORT_HEADER_LENGTH);
	header.append(0x02u);
	header.append(0x07u);
	header.append(static_cast<unsigned char>(keyIndex));

	while (bytes_remaining > 0)
	{
		int this_length = std::min(bytes_remaining, IMAGE_REPORT_PAYLOAD_LENGTH);
		int bytes_sent = page_number * IMAGE_REPORT_PAYLOAD_LENGTH;

		header[3] = static_cast<unsigned char>(this_length == bytes_remaining ? 1 : 0);
		header[4] = static_cast<unsigned char>(this_length & 0xFF);
		header[5] = static_cast<unsigned char>(this_length >> 8);
		header[6] = static_cast<unsigned char>(page_number & 0xFF);
		header[7] = static_cast<unsigned char>(page_number >> 8);

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

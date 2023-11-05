#include "StreamDeckMini.hpp"

using namespace minervous::streamdeck;

StreamDeckMini::StreamDeckMini(quint16 pid)
	: AbstractDeviceBase()
	, m_configuration{pid, VID, 5, 3, true, 72, 72, "JPG", true, true, 0}
{}

const IDevice::Configuration & StreamDeckMini::getConfiguration() const
{
	return m_configuration;
}

bool StreamDeckMini::setBrightness(int percentage)
{
	if (!m_hid.isOpen())
	{
		return false;
	}

	QByteArray m_send;
	m_send.fill(0, 33);
	m_send[0] = 0x03u;
	m_send[1] = 0x08u;
	m_send[2] = percentage % 101;  // brightness value [0..100]
	return m_hid.sendFeatureReport(&m_send) == m_send.size();
}

QString StreamDeckMini::getFirmwareVersion()
{
	if (!m_hid.isOpen())
	{
		return {};
	}

	QByteArray m_send;
	m_send.fill(0, 33);
	m_send[0] = 0x05u;
	if (33 == m_hid.getFeatureReport(&m_send))
	{
		return QString{m_send.data() + 6};
	}
	else
	{
		return {};
	}
}

bool StreamDeckMini::reset()
{
	if (!m_hid.isOpen())
	{
		return false;
	}

	QByteArray m_send;
	m_send.fill(0, 33);
	m_send[0] = 0x03u;
	m_send[1] = 0x02u;
	return 33 == m_hid.sendFeatureReport(&m_send);
}

int StreamDeckMini::readButtonsStatus(QList<bool> & buttonsStates)
{
	if (!m_hid.isOpen())
	{
		return -1;
	}

	QByteArray readed;
	readed.fill(0, 512);
	int count = m_hid.read(&readed, readed.size(), 0);
	if (count == readed.size())
	{
		for (int i(0); i < std::min(buttonsStates.size(), readed.size() - 4); ++i)
		{
			buttonsStates[i] = readed[4 + i] > 0;
		}
	}
	return count;
}

bool StreamDeckMini::sendImage(int keyIndex, const QByteArray & imageData)
{
	if (!m_hid.isOpen())
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
		if (payload.size() != m_hid.write(&payload))
		{
			success = false;
			break;
		}
		bytes_remaining = bytes_remaining - this_length;
		page_number = page_number + 1;
	}

	return success;
}

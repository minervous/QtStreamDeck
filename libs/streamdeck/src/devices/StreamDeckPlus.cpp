#include "StreamDeckPlus.hpp"

using namespace minervous::streamdeck;

StreamDeckPlus::StreamDeckPlus()
	: StreamDeckOriginalV2{PID}
{}

const IDevice::Configuration & StreamDeckPlus::getConfiguration() const
{
	static const Configuration conf = createConfiguration();
	return conf;
}

IDevice::Configuration StreamDeckPlus::createConfiguration()
{
	return {
		.pid = PID,
		.keyColumns = 4,
		.keyRows = 2,
		.imageWidth = 120,
		.imageFormat = ImageFormat::JPEG,
	};
}

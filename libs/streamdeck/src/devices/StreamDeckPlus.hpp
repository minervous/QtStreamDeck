#pragma once

#include "StreamDeckOriginalV2.hpp"

namespace minervous::streamdeck
{

	class StreamDeckPlus: public StreamDeckOriginalV2
	{
	public:
		static const auto PID = 0x0084;

		StreamDeckPlus();
		~StreamDeckPlus() override = default;

		const Configuration & getConfiguration() const override;

		static Configuration createConfiguration();
	};

}  // namespace minervous::streamdeck

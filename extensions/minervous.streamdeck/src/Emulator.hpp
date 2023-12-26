// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlParserStatus>

#include "minervous/streamdeck/DeviceEmulator.hpp"

namespace minervous::streamdeck::qml
{
	class Emulator
		: public minervous::streamdeck::DeviceEmulator
		, public QQmlParserStatus
	{
		Q_OBJECT
		Q_INTERFACES(QQmlParserStatus)
		QML_NAMED_ELEMENT(Emulator)

	public:
		explicit Emulator(QObject * parent = nullptr)
			: minervous::streamdeck::DeviceEmulator{parent}
		{}

		void classBegin() override {}

		void componentComplete() override { init(); }
	};

}  // namespace minervous::streamdeck::qml

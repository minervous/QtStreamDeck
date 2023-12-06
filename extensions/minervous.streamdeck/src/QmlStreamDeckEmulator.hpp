#pragma once

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlParserStatus>

#include "minervous/streamdeck/DeviceEmulator.hpp"

namespace minervous::streamdeck
{
	class QmlStreamDeckEmulator
		: public minervous::streamdeck::DeviceEmulator
		, public QQmlParserStatus
	{
		Q_OBJECT
		QML_ELEMENT
		Q_INTERFACES(QQmlParserStatus)
		QML_NAMED_ELEMENT(Emulator)
		using Base = minervous::streamdeck::DeviceEmulator;

	public:
		explicit QmlStreamDeckEmulator(QObject * parent = nullptr)
			: Base{parent}
		{}

		Q_INVOKABLE void press(int index) { Base::press(index); }

		Q_INVOKABLE void release(int index) { Base::release(index); }

		void classBegin() override {}

		void componentComplete() override { init(); }
	};

}  // namespace minervous::streamdeck

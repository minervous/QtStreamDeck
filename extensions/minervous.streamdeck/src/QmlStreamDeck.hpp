#pragma once

#include <QtCore/QVariant>

#include <QtQml/qqmlparserstatus.h>

#include "QmlStreamDeckKeyModel.hpp"
#include "minervous/streamdeck/Device.hpp"

#include "qqmlintegration.h"

namespace minervous::streamdeck
{
	class QmlStreamDeck
		: public minervous::streamdeck::Device
		, public QQmlParserStatus
	{
		Q_OBJECT
		QML_ELEMENT
		Q_INTERFACES(QQmlParserStatus)
		QML_NAMED_ELEMENT(StreamDeck)
		using Base = minervous::streamdeck::Device;

		using DefaultPropertyType = QmlStreamDeckKeyModel::DefaultPropertyType;
		Q_PROPERTY(DefaultPropertyType data READ qmlData NOTIFY qmlDataChanged)

		Q_CLASSINFO("DefaultProperty", "data")

	public:
		QmlStreamDeck(QObject * parent = nullptr)
			: Base{parent}
			, _defaultData{new QmlStreamDeckKeyModel(this)}
		{
			connect(_defaultData, &QmlStreamDeckKeyModel::qmlDataChanged, this, &QmlStreamDeck::qmlDataChanged);
		}

		Q_INVOKABLE bool open() { return Base::open(); }

		Q_INVOKABLE void close() { Base::close(); }

		Q_INVOKABLE void reset() { Base::reset(); }

		Q_INVOKABLE void sendImage(int index, const QVariant image)
		{
			if (!image.isNull() && image.canConvert<QImage>())
			{
				QImage img(image.value<QImage>());
				Base::sendImage(index, img);
			}
			else
			{
				qWarning() << "Unexpected format. Could not send image" << image;
			}
		}

		Q_INVOKABLE void sendImage(int index, QUrl url) { Base::sendImage(index, url); }

		void classBegin() override {}

		void componentComplete() override
		{
			init();
			if (!model())
			{
				applyModel(_defaultData);
			}
		}

	signals:
		void qmlDataChanged();

	private:
		DefaultPropertyType qmlData() { return _defaultData->qmlData(); }

		QmlStreamDeckKeyModel * _defaultData;
	};

}  // namespace minervous::streamdeck

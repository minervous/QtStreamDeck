#pragma once

#include <QtCore/QSize>
#include <QtQml/QQmlEngine>
#include <QtGui/QImage>
#include <QtQuick/QQuickItem>

namespace minervous::streamdeck
{
	class QmlItemGrabber: public QObject, public QQmlParserStatus
	{
		Q_OBJECT
		QML_ELEMENT
		Q_INTERFACES(QQmlParserStatus)
		QML_NAMED_ELEMENT(ItemGrabber)

		Q_PROPERTY(QQuickItem * item READ item WRITE setItem NOTIFY itemChanged REQUIRED FINAL)
		Q_PROPERTY(QQuickItem * attachingVisualParent READ attachingVisualParent WRITE setAttachingVisualParent NOTIFY attachingVisualParentChanged FINAL)
		Q_PROPERTY(QSize targetSize READ targetSize WRITE setTargetSize NOTIFY targetSizeChanged FINAL)

		Q_PROPERTY(bool isReadyToGrab READ isReadyToGrab NOTIFY isReadyToGrabChanged FINAL)
		Q_PROPERTY(QImage image READ image NOTIFY grabbed FINAL)

	public:
		explicit QmlItemGrabber(QObject * parent = nullptr);
		~QmlItemGrabber() override = default;

		Q_INVOKABLE bool grab();

		QQuickItem * item() const;
		void setItem(QQuickItem * item);

		QQuickItem * attachingVisualParent() const;
		void setAttachingVisualParent(QQuickItem * attachingVisualParent);

		QSize targetSize() const;
		void setTargetSize(const QSize & targetSize);

		bool isReadyToGrab() const;

		const QImage & image() const;

		void classBegin() override;
		void componentComplete() override;

	signals:
		void itemChanged();
		void attachingVisualParentChanged();
		void targetSizeChanged();
		void isReadyToGrabChanged();
		void grabbed(const QImage & image);

	private:
		QQuickItem * _item = nullptr;
		QQuickItem * _attachingVisualParent = nullptr;
		QWindow * _windowToRenderItem = nullptr;
		QSize _targetSize;
		QImage _image;
		bool _ready = false;
		bool _completed = false;

		bool checkReadyToGrab();
		void checkAndGrabOnReadyChanged();
		void setDefautVisualParent();
	};

}  // namespace minervous::streamdeck

#include "QmlItemGrabber.hpp"

#include <QtQuick/QQuickItemGrabResult>
#include <QtQuick/QQuickWindow>

using namespace minervous::streamdeck;

QmlItemGrabber::QmlItemGrabber(QObject * parent)
    : QObject{parent}
{}

bool QmlItemGrabber::grab()
{
	checkReadyToGrab();

	if (!_ready)
		return false;

	// Store item state
	QQuickItem * itemParent = _item->parentItem();
	bool itemVisible = _item->isVisible();

	// Reparent to visible Item to render on the window
	if (_item->parentItem() != _attachingVisualParent)
	{
		// Make it invisible before reparenting to visible Window if it was outside the scene
		if (!_item->window())
			_item->setVisible(false);

		_item->setParentItem(_attachingVisualParent);
	}

	auto grabResult = _item->grabToImage(_targetSize);
	if (grabResult.isNull())
		return false;

	connect(
		grabResult.get(),
		&QQuickItemGrabResult::ready,
		this,
		[grabResult, itemParent, itemVisible, this]()
		{
			_image = grabResult->image();
			emit grabbed(_image);
			disconnect(grabResult.get(), nullptr, this, nullptr);

			// Restore previous item's state
			if (_item->parentItem() != itemParent)
			{
				_item->setParentItem(itemParent);
				_item->setVisible(itemVisible);
			}
		}
	);
	return true;
}

QQuickItem * QmlItemGrabber::item() const
{
	return _item;
}

void QmlItemGrabber::setItem(QQuickItem * item)
{
	if (_item != item)
	{
		_item = item;
		if (_completed)
			grab();
		emit itemChanged();
	}
}

QQuickItem * QmlItemGrabber::attachingVisualParent() const
{
	return _attachingVisualParent;
}

void QmlItemGrabber::setAttachingVisualParent(QQuickItem * attachingVisualParent)
{
	if (_attachingVisualParent != attachingVisualParent)
	{
		if (_attachingVisualParent)
		{
			disconnect(_attachingVisualParent, nullptr, this, nullptr);
			if (_windowToRenderItem)
			{
				disconnect(_windowToRenderItem, nullptr, this, nullptr);
			}
		}

		_attachingVisualParent = attachingVisualParent;

		if (_attachingVisualParent)
		{
			connect(
				_attachingVisualParent,
				&QQuickItem::windowChanged,
				this,
				[this](QQuickWindow * window)
				{
					if (_windowToRenderItem)
					{
						disconnect(_windowToRenderItem, nullptr, this, nullptr);
					}
					_windowToRenderItem = window;
					if (_windowToRenderItem)
					{
						connect(
							_windowToRenderItem,
							&QQuickWindow::visibleChanged,
							this,
							&QmlItemGrabber::checkAndGrabOnReadyChanged
						);
					}
					checkAndGrabOnReadyChanged();
				}
			);
			connect(
				_attachingVisualParent,
				&QQuickItem::visibleChanged,
				this,
				&QmlItemGrabber::checkAndGrabOnReadyChanged
			);

			_windowToRenderItem = _attachingVisualParent->window();
			if (_windowToRenderItem)
			{
				connect(
					_windowToRenderItem,
					&QQuickWindow::visibleChanged,
					this,
					&QmlItemGrabber::checkAndGrabOnReadyChanged
				);
			}

			if (_completed)
				grab();
		}
		else
		{

		}
		emit attachingVisualParentChanged();
	}
}

QSize QmlItemGrabber::targetSize() const
{
	return _targetSize;
}

void QmlItemGrabber::setTargetSize(const QSize & targetSize)
{
	if (_targetSize != targetSize)
	{
		_targetSize = targetSize;
		if (_completed)
			grab();
		emit targetSizeChanged();
	}
}

bool QmlItemGrabber::isReadyToGrab() const
{
	return _ready;
}

const QImage & QmlItemGrabber::image() const
{
	return _image;
}

void QmlItemGrabber::classBegin() {}

void QmlItemGrabber::componentComplete()
{
	setDefautVisualParent();
	grab();
	_completed = true;
}

bool QmlItemGrabber::checkReadyToGrab()
{
	bool ready = _item && _attachingVisualParent && _attachingVisualParent->isVisible() &&
				 _attachingVisualParent->window() && _attachingVisualParent->window()->isVisible() &&
				 !_targetSize.isEmpty();
	if (_ready != ready)
	{
		_ready = ready;
		emit isReadyToGrabChanged();
		return ready;
	}
	return false;
}

void QmlItemGrabber::checkAndGrabOnReadyChanged()
{
	if (checkReadyToGrab())
		grab();
}

void QmlItemGrabber::setDefautVisualParent()
{
	if (!_item)
		return;

	if (!_attachingVisualParent)
	{
		auto obj = _item->parent();
		QQuickItem * visualParent = nullptr;
		while (obj)
		{
			visualParent = qobject_cast<QQuickItem *>(obj);
			if (visualParent && visualParent->window())
			{
				setAttachingVisualParent(visualParent);
				break;
			}
			obj = obj->parent();
		}
	}
}

// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#include "ItemGrabber.hpp"

#include <QtQuick/QQuickItemGrabResult>
#include <QtQuick/QQuickWindow>

using namespace minervous::streamdeck::qml;

ItemGrabber::ItemGrabber(QObject * parent)
	: QObject{parent}
{}

bool ItemGrabber::grab()
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

QQuickItem * ItemGrabber::item() const
{
	return _item;
}

void ItemGrabber::setItem(QQuickItem * item)
{
	if (_item != item)
	{
		_item = item;
		if (_completed)
			grab();
		emit itemChanged();
	}
}

QQuickItem * ItemGrabber::attachingVisualParent() const
{
	return _attachingVisualParent;
}

void ItemGrabber::setAttachingVisualParent(QQuickItem * attachingVisualParent)
{
	if (_attachingVisualParent == attachingVisualParent)
		return;

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
						&ItemGrabber::checkAndGrabOnReadyChanged
					);
				}
				checkAndGrabOnReadyChanged();
			}
		);
		connect(_attachingVisualParent, &QQuickItem::visibleChanged, this, &ItemGrabber::checkAndGrabOnReadyChanged);

		_windowToRenderItem = _attachingVisualParent->window();
		if (_windowToRenderItem)
		{
			connect(_windowToRenderItem, &QQuickWindow::visibleChanged, this, &ItemGrabber::checkAndGrabOnReadyChanged);
		}

		if (_completed)
			grab();
	}

	emit attachingVisualParentChanged();
}

QSize ItemGrabber::targetSize() const
{
	return _targetSize;
}

void ItemGrabber::setTargetSize(const QSize & targetSize)
{
	if (_targetSize != targetSize)
	{
		_targetSize = targetSize;
		if (_completed)
			grab();
		emit targetSizeChanged();
	}
}

bool ItemGrabber::isReadyToGrab() const
{
	return _ready;
}

const QImage & ItemGrabber::image() const
{
	return _image;
}

void ItemGrabber::classBegin()
{
	// Nothing to do...
}

void ItemGrabber::componentComplete()
{
	grab();
	_completed = true;
}

bool ItemGrabber::checkReadyToGrab()
{
	if (_item && !_attachingVisualParent)
		setDefautVisualParent();

	if (bool ready = _item && _attachingVisualParent && _attachingVisualParent->isVisible() &&
					 _attachingVisualParent->window() && _attachingVisualParent->window()->isVisible() &&
					 !_targetSize.isEmpty();
		_ready != ready)
	{
		_ready = ready;
		emit isReadyToGrabChanged();
		return _ready;
	}
	else
		return false;
}

void ItemGrabber::checkAndGrabOnReadyChanged()
{
	if (checkReadyToGrab())
		grab();
}

void ItemGrabber::setDefautVisualParent()
{
	auto obj = _item->parent();
	QQuickItem * visualParent = nullptr;
	while (obj)
	{
		if (visualParent = qobject_cast<QQuickItem *>(obj); visualParent && visualParent->window())
		{
			setAttachingVisualParent(visualParent);
			break;
		}
		obj = obj->parent();
	}
}

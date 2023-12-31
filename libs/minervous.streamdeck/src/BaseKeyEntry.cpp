// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#include "BaseKeyEntry.hpp"

using namespace minervous::streamdeck;

BaseKeyEntry::BaseKeyEntry(QObject * parent)
	: QObject{parent}
{}

bool BaseKeyEntry::pressed() const
{
	return _pressed;
}

QUrl BaseKeyEntry::imageSource() const
{
	return _imageSource;
}

const BaseKeyEntry::ImageType & BaseKeyEntry::image() const
{
	return _image;
}

void BaseKeyEntry::setImageSource(QUrl url)
{
	if (_imageSource != url)
	{
		_imageSource = url;
		emit imageSourceChanged();
	}
}

void BaseKeyEntry::setImage(const ImageType & image)
{
	if (_image != image)
	{
		_image = image;
		emit imageChanged();
	}
}

void BaseKeyEntry::setPressed(bool pressed, bool initializationOnly)
{
	if (_pressed != pressed)
	{
		_pressed = pressed;
		emit pressedChanged();
		if (!initializationOnly)
		{
			if (pressed)
			{
				emit keyPressed();
			}
			else
			{
				emit keyReleased();
			}
		}
	}
}

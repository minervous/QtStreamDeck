#include "KeyModel.hpp"

using namespace minervous::streamdeck;

KeyModel::KeyModel(QObject * parent)
	:QObject(parent)
{
}

int KeyModel::count() const
{
	return _data.count();
}

void KeyModel::set(QList<BaseKeyEntry *> & list)
{
	for(const auto & entry: qAsConst(_data)) {
		disconnectEntry(entry);
	}

	int prevCount = _data.count();
	_data.clear();
	int index(0);
	for(const auto & entry: qAsConst(list))
	{
		// wrapp by QPointer
		_data.append(entry);
		emit modelEntryChanged(index, entry);
		connectEntry(index, entry);
		++index;
	}
	for(; index < prevCount; ++index)
	{
		emit modelEntryChanged(index);
	}
	if (prevCount != _data.count())
	{
		emit countChanged();
	}
}

void KeyModel::clear()
{
	if (!_data.empty())
	{
		QList<BaseKeyEntry *> emptyList;
		set(emptyList);
	}
}

void KeyModel::append(BaseKeyEntry * entry)
{
	_data.append(entry);
	emit modelEntryChanged(_data.count() - 1, entry);
	connectEntry(_data.count() - 1, entry);
	emit countChanged();
}

void KeyModel::remove(int index)
{
	if (index >=0 && index < _data.count())
	{
		disconnectEntry(_data[index]);
		_data.removeAt(index);
		for(int max(_data.count()); index < max; ++index)
		{
			emit modelEntryChanged(index, _data[index]);
			connectEntry(index, _data[index]);
		}
		emit modelEntryChanged(index, nullptr);
		emit countChanged();
	} else {
		qWarning() << "Could not remove entry. Index" << index << "is out of range";
	}
}

void KeyModel::insert(int index, BaseKeyEntry * entry)
{
	if (index >=0 && index < _data.count())
	{
		_data.insert(index, entry);
		for(int max(_data.count()); index < max; ++index)
		{
			disconnectEntry(_data[index]);
			emit modelEntryChanged(index, _data[index]);
			connectEntry(index, _data[index]);
		}
		emit countChanged();
	} else if (index == _data.count()) {
		append(entry);
	} else {
		qWarning() << "Could not insert entry. Index" << index << "is out of range";
	}
}
void KeyModel::replace(int index, BaseKeyEntry * entry)
{
	if (index >=0 && index < _data.count())
	{
		disconnectEntry(_data[index]);
		_data.replace(index, entry);
		emit modelEntryChanged(index, _data[index]);
		connectEntry(index, _data[index]);
		emit countChanged();
	} else {
		qWarning() << "Could not replace entry. Index" << index << "is out of range";
	}
}

BaseKeyEntry * KeyModel::at(int index)
{
	if (index >=0 && index < _data.count())
	{
		return _data.at(index);
	} else {
		qWarning() << "Could not find an entry in the model at index" << index << ". The index is out of range";
		return nullptr;
	}
}

void KeyModel::setKeyPressed(int keyIndex, bool state)
{
	if (keyIndex >=0 && keyIndex < _data.count())
	{
		auto entry {_data[keyIndex]};
		if (entry)
		{
			entry->setPressed(state);
		}
	}
}

void KeyModel::connectEntry(int index, BaseKeyEntry * entry)
{
	if (entry)
	{
		auto onImageChange = [=]() {
			emit imageChanged(index, entry);
		};
		connect(entry, &BaseKeyEntry::imageChanged, this, onImageChange);
		connect(entry, &BaseKeyEntry::imageSourceChanged, this, onImageChange);
	}
}
void KeyModel::disconnectEntry(BaseKeyEntry * entry)
{
	if (entry)
	{
		disconnect(entry, nullptr, this, nullptr);
	}
}

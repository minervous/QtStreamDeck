#include "KeyModel.hpp"

using namespace minervous::streamdeck;

KeyModel::KeyModel(QObject * parent)
	: QObject{parent}
{}

qsizetype KeyModel::count() const
{
	return _data.count();
}

void KeyModel::set(const QList<BaseKeyEntry *> & list)
{
	for (const auto & entry: qAsConst(_data))
	{
		disconnectEntry(entry);
	}

	int prevCount = _data.count();
	_data.clear();
	int index = 0;
	for (const auto & entry: qAsConst(list))
	{
		if (_data.contains(entry))
		{
			qWarning() << "Could not append the same entry to the model twice";
		}
		else
		{
			// wrapp by QPointer
			_data.append(entry);
			emit modelEntryChanged(index, entry);
			connectEntry(index, entry);
			++index;
		}
	}
	for (; index < prevCount; ++index)
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
	if (_data.contains(entry))
	{
		qWarning() << "Could not append the same entry to the model twice";
	}
	else
	{
		_data.append(entry);
		emit modelEntryChanged(_data.count() - 1, entry);
		connectEntry(_data.count() - 1, entry);
		emit countChanged();
	}
}

void KeyModel::remove(qsizetype index)
{
	if (index >= 0 && index < _data.count())
	{
		disconnectEntry(_data[index]);
		_data.removeAt(index);
		for (int max(_data.count()); index < max; ++index)
		{
			emit modelEntryChanged(index, _data[index]);
			connectEntry(index, _data[index]);
		}
		emit modelEntryChanged(index, nullptr);
		emit countChanged();
	}
	else
	{
		qWarning() << "Could not remove entry. Index" << index << "is out of range";
	}
}

void KeyModel::insert(qsizetype index, BaseKeyEntry * entry)
{
	if (index >= 0 && index < _data.count())
	{
		if (_data.contains(entry))
		{
			qWarning() << "Could not insert the same entry to the model twice";
		}
		else
		{
			_data.insert(index, entry);
			for (int max(_data.count()); index < max; ++index)
			{
				disconnectEntry(_data[index]);
				emit modelEntryChanged(index, _data[index]);
				connectEntry(index, _data[index]);
			}
			emit countChanged();
		}
	}
	else if (index == _data.count())
	{
		append(entry);
	}
	else
	{
		qWarning() << "Could not insert entry. Index" << index << "is out of range";
	}
}

void KeyModel::replace(qsizetype index, BaseKeyEntry * entry)
{
	if (index >= 0 && index < _data.count())
	{
		if (_data.contains(entry))
		{
			qWarning() << "Could not add the same entry to the model twice";
		}
		else
		{
			disconnectEntry(_data[index]);
			_data.replace(index, entry);
			emit modelEntryChanged(index, _data[index]);
			connectEntry(index, _data[index]);
			emit countChanged();
		}
	}
	else
	{
		qWarning() << "Could not replace entry. Index" << index << "is out of range";
	}
}

const BaseKeyEntry * KeyModel::at(qsizetype index) const noexcept
{
	return _data.at(index).data();
}

void KeyModel::connectEntry(int index, BaseKeyEntry * entry)
{
	if (entry)
	{
		auto onImageChange = [=]()
		{
			emit imageChanged(index, entry);
		};
		connect(entry, &BaseKeyEntry::imageChanged, this, onImageChange);
		connect(entry, &BaseKeyEntry::imageSourceChanged, this, onImageChange);
	}
}

void KeyModel::disconnectEntry(BaseKeyEntry * entry) const
{
	if (entry)
	{
		disconnect(entry, nullptr, this, nullptr);
	}
}

const BaseKeyEntry * KeyModel::operator[](qsizetype i) const noexcept
{
	return at(i);
}

BaseKeyEntry * KeyModel::operator[](qsizetype i)
{
	return _data[i];
}

bool KeyModel::contains(const BaseKeyEntry * e) const noexcept
{
	return _data.contains(e);
}

qsizetype KeyModel::indexOf(const BaseKeyEntry * e) const noexcept
{
	return _data.indexOf(e);
}

// auto KeyModel::operator<=>(const KeyModel & that) const
//{
//	if (_data == that._data)
//		return std::strong_ordering::equivalent;
//	else if (_data < that._data)
//		return std::strong_ordering::less;
//	else
//		return std::strong_ordering::greater;
// }

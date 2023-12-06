#include "QmlPagedKeyModel.hpp"

#include <QtCore/QCoreApplication>

using namespace minervous::streamdeck;

namespace
{
	static constexpr const int minKeyCount = 3;
}

QmlPagedKeyModel::DelayedKeysUpdateEvent::DelayedKeysUpdateEvent(QmlPagedKeyModel * sender)
	: QEvent(staticType())
	, sender{sender}
{}

QEvent::Type QmlPagedKeyModel::DelayedKeysUpdateEvent::staticType()
{
	static int type = QEvent::registerEventType();
	return static_cast<QEvent::Type>(type);
}

QmlPagedKeyModel::QmlPagedKeyModel(QObject * parent)
	: Base{parent}
{}

void QmlPagedKeyModel::classBegin() {}

void QmlPagedKeyModel::componentComplete()
{
	calculatePagesCount();
	updateKeys();
	_completed = true;
}

QObject * QmlPagedKeyModel::sourceModel() const
{
	return _sourceModel;
}

void QmlPagedKeyModel::setSourceModel(QObject * model)
{
	auto * newModel = qobject_cast<KeyModel *>(model);
	if (!newModel && model)
	{
		qWarning() << "Unexpected type of model" << model;
		return;
	}
	if (_sourceModel != newModel)
	{
		if (_sourceModel)
		{
			disconnect(_sourceModel, nullptr, this, nullptr);
		}

		_sourceModel = newModel;
		if (_sourceModel)
		{
			connect(
				_sourceModel,
				&KeyModel::countChanged,
				this,
				[this]()
				{
					if (_completed)
					{
						calculatePagesCount();
						updateKeys();
					}
				}
			);

			connect(
				_sourceModel,
				&KeyModel::modelEntryChanged,
				this,
				[this](int index, BaseKeyEntry * entry)
				{
					if (_completed && index >= _keyIndexBegin && index <= _keyIndexEnd)
					{
						if (entry)
						{
							// Update/Replace/Add entry case - Call later to avoid repeated calls of moved content
							// during insert/remove operations in the same loop Recalculation will be done on
							// countChanged or in next loop
							updateKeysLater();
						}
						else
						{
							// Remove entry case - call updateKeys immediately
							calculatePagesCount();
							updateKeys(true);
						}
					}
				}
			);
		}

		if (_completed)
			updateKeys();

		emit sourceModelChanged();
	}
}

int QmlPagedKeyModel::keysPerPage() const
{
	return _keysPerPage;
}

void QmlPagedKeyModel::setKeysPerPage(int count)
{
	if (count > 0 && count < minKeyCount)
	{
		qWarning() << "Unexpected value of keysPerPage" << count << ". It is expected to be" << minKeyCount
				   << "at least";
		// Not sure if that value should be skipped. Probably, it is enought to warn and just do not show keys.
		// return;
	}

	if (_keysPerPage != count)
	{
		_keysPerPage = count;
		if (_completed)
		{
			calculatePagesCount();
			updateKeys();
		}
		emit keysPerPageChanged();
	}
}

QObject * QmlPagedKeyModel::prevPageKeyEntry() const
{
	return _prevPageEntry;
}

void QmlPagedKeyModel::setPrevPageKeyEntry(QObject * entry)
{
	auto * pageEntry = qobject_cast<BaseKeyEntry *>(entry);
	if (!pageEntry && entry)
	{
		qWarning() << "Unexpected type of prevPageKeyEntry" << entry;
		return;
	}
	if (_prevPageEntry != pageEntry)
	{
		if (_prevPageEntry)
		{
			disconnect(_prevPageEntry, nullptr, this, nullptr);
		}

		_prevPageEntry = pageEntry;
		if (_prevPageEntry)
		{
			connect(_prevPageEntry, &BaseKeyEntry::keyPressed, this, &QmlPagedKeyModel::prevPage);
		}

		if (_completed)
			updateKeys();
		emit prevPageKeyEntryChanged();
	}
}

QObject * QmlPagedKeyModel::nextPageKeyEntry() const
{
	return _nextPageEntry;
}

void QmlPagedKeyModel::setNextPageKeyEntry(QObject * entry)
{
	auto * pageEntry = qobject_cast<BaseKeyEntry *>(entry);
	if (!pageEntry && entry)
	{
		qWarning() << "Unexpected type of nextPageKeyEntry" << entry;
		return;
	}
	if (_nextPageEntry != pageEntry)
	{
		if (_nextPageEntry)
		{
			disconnect(_nextPageEntry, nullptr, this, nullptr);
		}

		_nextPageEntry = pageEntry;
		if (_nextPageEntry)
		{
			connect(_nextPageEntry, &BaseKeyEntry::keyPressed, this, &QmlPagedKeyModel::nextPage);
		}

		if (_completed)
			updateKeys();
		emit nextPageKeyEntryChanged();
	}
}

int QmlPagedKeyModel::page() const
{
	return _page;
}

void QmlPagedKeyModel::setPage(int page)
{
	if (_completed && (page >= _pagesCount || page < 0))
	{
		qWarning() << "Requested page is out of range";
		page = std::clamp(page, 0, _pagesCount - 1);
	}

	if (_page != page)
	{
		_page = page;
		if (_completed)
		{
			updateKeys();
		}
		emit pageChanged();
	}
}

int QmlPagedKeyModel::pagesCount() const
{
	return _pagesCount;
}

void QmlPagedKeyModel::nextPage()
{
	if (_page < _pagesCount - 1)
		setPage(_page + 1);
}

void QmlPagedKeyModel::prevPage()
{
	if (_page > 0)
		setPage(_page - 1);
}

void QmlPagedKeyModel::calculatePagesCount()
{
	int pages = 1;
	auto sourceModelCount = _sourceModel->count();
	if (_sourceModel && _keysPerPage >= minKeyCount && sourceModelCount > _keysPerPage)
	{
		const auto keysOnFirstPage = _keysPerPage - 1;
		const auto keysOnRegularPage = _keysPerPage - 2;
		auto keysOnLastPage = (sourceModelCount - keysOnFirstPage) % keysOnRegularPage;
		if (keysOnLastPage == 1 || keysOnRegularPage == 1)
			keysOnLastPage = keysOnRegularPage + 1;
		pages = 2 + (sourceModelCount - keysOnFirstPage - keysOnLastPage) / keysOnRegularPage;
	}
	if (_pagesCount != pages)
	{
		_pagesCount = pages;
		emit pagesCountChanged();
	}
}

void QmlPagedKeyModel::addOrReplaceEntry(int index, BaseKeyEntry * entry)
{
	if (auto existEntryIndex = indexOf(entry); existEntryIndex >= 0 && index != existEntryIndex)
	{
		remove(existEntryIndex);
	}
	if (index < count())
	{
		if (at(index) != entry)
			replace(index, entry);
	}
	else
	{
		append(entry);
	}
}

void QmlPagedKeyModel::updateKeys(bool forceUpdate)
{
	if (_waitingForLaterUpdateKeys)
	{
		_waitingForLaterUpdateKeys = false;
		forceUpdate = true;
	}

	auto sourceModelCount = _sourceModel->count();

	if (!_sourceModel || _keysPerPage < minKeyCount || sourceModelCount == 0 || _pagesCount == 0)
	{
		clear();
		return;
	}

	auto currentPage = std::clamp(_page, 0, _pagesCount - 1);
	auto currentBeginIndex =
		std::clamp<int>(currentPage ? (_keysPerPage - 2) * currentPage + 1 : 0, 0, sourceModelCount - 1);

	auto currentEndIndex = std::clamp<int>(
		currentPage == _pagesCount - 1 ? std::max<int>(sourceModelCount - 1, 0)
									   : (_keysPerPage - 2) * (currentPage + 1),
		currentBeginIndex,
		sourceModelCount - 1
	);

	if (currentBeginIndex == _keyIndexBegin && currentEndIndex == _keyIndexEnd && currentPage == _page && !forceUpdate)
		return;

	int index = 0;
	if (_pagesCount > 1 && currentPage)
		addOrReplaceEntry(index++, _prevPageEntry);

	if (currentBeginIndex <= currentEndIndex)
	{
		for (auto i = currentBeginIndex; i <= currentEndIndex; ++i)
			addOrReplaceEntry(index++, (*_sourceModel)[i]);
	}

	if (_pagesCount > 1 && currentPage < _pagesCount - 1)
		addOrReplaceEntry(index++, _nextPageEntry);

	while (count() > index)
		remove(count() - 1);

	_keyIndexBegin = currentBeginIndex;
	_keyIndexEnd = currentEndIndex;
	if (_page != currentPage)
	{
		_page = currentPage;
		emit pageChanged();
	}

	qDebug() << "page=" << _page << ", source model indexes [" << _keyIndexBegin << "," << _keyIndexEnd << "]";
}

void QmlPagedKeyModel::updateKeysLater()
{
	if (_waitingForLaterUpdateKeys)
		return;
	_waitingForLaterUpdateKeys = true;
	qApp->postEvent(this, new DelayedKeysUpdateEvent(this));
}

void QmlPagedKeyModel::customEvent(QEvent * event)
{
	if (event->type() == DelayedKeysUpdateEvent::staticType())
	{
		DelayedKeysUpdateEvent * updateEvent = static_cast<DelayedKeysUpdateEvent *>(event);
		if (updateEvent->sender == this)
		{
			if (_waitingForLaterUpdateKeys)
			{
				calculatePagesCount();
				updateKeys(true);
			}
			return;
		}
	}
	Base::customEvent(event);
}

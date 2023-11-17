#pragma once

#include <QtCore/QObject>
#include "BaseKeyEntry.hpp"

namespace minervous::streamdeck
{
	class Device;

	class KeyModel: public QObject
	{
		Q_OBJECT

		Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)

	public:
		explicit KeyModel(QObject * parent = nullptr);

		int count() const;

		void set(QList<BaseKeyEntry *> & list);
		void clear();
		void append(BaseKeyEntry * entry);
		void remove(int index);
		void insert(int index, BaseKeyEntry * entry);
		void replace(int index, BaseKeyEntry * entry);
		BaseKeyEntry * at(int index);

	signals:
		void countChanged();
		void modelEntryChanged(int index, BaseKeyEntry * entry = nullptr);
		void imageChanged(int index, BaseKeyEntry * entry);

	protected:
		void setKeyPressed(int keyIndex, bool state);
		friend class Device;

		void connectEntry(int index, BaseKeyEntry * entry);
		void disconnectEntry(BaseKeyEntry * entry);

		QList< QPointer<BaseKeyEntry> > _data;
	};
}  // namespace minervous::streamdeck

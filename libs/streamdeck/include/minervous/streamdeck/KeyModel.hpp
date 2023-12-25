// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtCore/QList>
#include <QtCore/QObject>

#include "BaseKeyEntry.hpp"

namespace minervous::streamdeck
{
	class STREAMDECKLIB_EXPORT KeyModel: public QObject
	{
		Q_OBJECT

		Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)

		using StorageType = QList<QPointer<BaseKeyEntry> >;

	public:
		explicit KeyModel(QObject * parent = nullptr);

		qsizetype count() const;

		void set(const QList<BaseKeyEntry *> & list);
		void clear();
		void append(BaseKeyEntry * entry);
		void remove(qsizetype index);
		void insert(qsizetype index, BaseKeyEntry * entry);
		void replace(qsizetype index, BaseKeyEntry * entry);
		const BaseKeyEntry * at(qsizetype index) const noexcept;

		const BaseKeyEntry * operator[](qsizetype i) const noexcept;
		BaseKeyEntry * operator[](qsizetype i);

		bool contains(const BaseKeyEntry * e) const noexcept;
		qsizetype indexOf(const BaseKeyEntry * e) const noexcept;

		// auto operator<=>(const KeyModel&) const;

		inline bool isEmpty() const noexcept { return _data.isEmpty(); }

		// comfort
		inline KeyModel & operator<<(BaseKeyEntry * e)
		{
			append(e);
			return *this;
		}

		inline KeyModel & operator+=(BaseKeyEntry * e)
		{
			append(e);
			return *this;
		}

	signals:
		void countChanged();
		void modelEntryChanged(int index, BaseKeyEntry * entry = nullptr);
		void imageChanged(int index, BaseKeyEntry * entry);

	protected:
		void connectEntry(int index, BaseKeyEntry * entry);
		void disconnectEntry(BaseKeyEntry * entry) const;

		StorageType _data;
	};

}  // namespace minervous::streamdeck

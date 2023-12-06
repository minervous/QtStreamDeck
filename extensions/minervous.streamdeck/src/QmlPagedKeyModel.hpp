#pragma once

#include <QtQml/QQmlEngine>
#include <QtCore/qcoreevent.h>

#include "QmlKeyModel.hpp"


namespace minervous::streamdeck
{
	class QmlPagedKeyModel: public QmlKeyModel, public QQmlParserStatus
	{
		Q_OBJECT
		QML_ELEMENT
		Q_INTERFACES(QQmlParserStatus)
		QML_NAMED_ELEMENT(PagedKeyModel)
		using Base = minervous::streamdeck::QmlKeyModel;

		Q_PROPERTY(QObject * sourceModel READ sourceModel WRITE setSourceModel NOTIFY sourceModelChanged REQUIRED FINAL)
		Q_PROPERTY(int keysPerPage READ keysPerPage WRITE setKeysPerPage NOTIFY keysPerPageChanged REQUIRED FINAL)
		Q_PROPERTY(QObject * prevPageKeyEntry READ prevPageKeyEntry WRITE setPrevPageKeyEntry NOTIFY prevPageKeyEntryChanged REQUIRED FINAL)
		Q_PROPERTY(QObject * nextPageKeyEntry READ nextPageKeyEntry WRITE setNextPageKeyEntry NOTIFY nextPageKeyEntryChanged REQUIRED FINAL)

		Q_PROPERTY(int page READ page WRITE setPage NOTIFY pageChanged FINAL)
		Q_PROPERTY(int pagesCount READ pagesCount NOTIFY pagesCountChanged FINAL)
	public:
		explicit QmlPagedKeyModel(QObject * parent = nullptr);
		~QmlPagedKeyModel() override = default;

		QObject * sourceModel() const;
		void setSourceModel(QObject * model);

		int keysPerPage() const;
		void setKeysPerPage(int count);

		QObject * prevPageKeyEntry() const;
		void setPrevPageKeyEntry(QObject * entry);

		QObject * nextPageKeyEntry() const;
		void setNextPageKeyEntry(QObject * entry);

		int page() const;
		void setPage(int page);

		int pagesCount() const;

		void classBegin() override;
		void componentComplete() override;

		Q_INVOKABLE void nextPage();
		Q_INVOKABLE void prevPage();

	signals:
		void sourceModelChanged();
		void keysPerPageChanged();
		void prevPageKeyEntryChanged();
		void nextPageKeyEntryChanged();
		void pageChanged();
		void pagesCountChanged();

	protected:
		void customEvent(QEvent *event) override;

	private:
		class DelayedKeysUpdateEvent: public QEvent
		{
		public:
			DelayedKeysUpdateEvent(QmlPagedKeyModel * sender);

			static QEvent::Type staticType();

			QPointer<QmlPagedKeyModel> sender;
		};

		KeyModel * _sourceModel = nullptr;
		BaseKeyEntry * _prevPageEntry = nullptr;
		BaseKeyEntry * _nextPageEntry = nullptr;
		int _keysPerPage = 0;
		int _page = 0;
		int _pagesCount = 0;
		int _keyIndexBegin = 0;
		int _keyIndexEnd = 0;
		bool _completed = false;
		bool _waitingForLaterUpdateKeys = false;

		void calculatePagesCount();
		void addOrReplaceEntry(int index, BaseKeyEntry * entry);
		void updateKeys(bool forceUpdate = false);
		void updateKeysLater();

	};

}  // namespace minervous::streamdeck

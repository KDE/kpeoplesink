#ifndef KPEOPLESINK_H
#define KPEOPLESINK_H

#include <KPeopleBackend/AllContactsMonitor>
#include <KPeopleBackend/AbstractContact>
#include <QSet>
#include <sink/notifier.h>
#include <sink/notification.h>
#include <sink/store.h>

using namespace Sink;
using namespace Sink::ApplicationDomain;
using Sink::ApplicationDomain::SinkResource;

class Q_DECL_EXPORT KPeopleSink : public KPeople::AllContactsMonitor
{
    Q_OBJECT
    Notifier *m_notifier;
    QSet<QString> m_contactsSet;

public:
    KPeopleSink();
    virtual ~KPeopleSink();

public slots:
    void getContactstoKpeople();
    void processRecentlySyncedContacts(QByteArray resourceId);

private:
    QString getUri(Contact sinkContact, QByteArray resourceId);
};

#endif
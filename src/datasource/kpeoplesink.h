/*
 *  SPDX-FileCopyrightText: 2019 Rituka Patwal <ritukapatwal21@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

#ifndef KPEOPLESINK_H
#define KPEOPLESINK_H

#include <QSet>

#include <KPeopleBackend/AbstractContact>
#include <KPeopleBackend/AllContactsMonitor>

#include <sink/applicationdomaintype.h>
#include <sink/notifier.h>

class Q_DECL_EXPORT KPeopleSink : public KPeople::AllContactsMonitor
{
    Q_OBJECT
public:
    explicit KPeopleSink();

    QMap<QString, KPeople::AbstractContact::Ptr> contacts() override;

public Q_SLOTS:
    void initialSinkContactstoKpeople();
    void processRecentlySyncedContacts(QByteArray resourceId);
    void toRemoveContact(QSet<QString> contactUri);

private:
    QString getUri(Sink::ApplicationDomain::Contact sinkContact, QByteArray resourceId);

    Sink::Notifier *m_notifier;
    QMap<QString, KPeople::AbstractContact::Ptr> m_contactUriMap;
};

#endif

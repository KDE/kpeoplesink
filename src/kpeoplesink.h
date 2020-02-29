/*************************************************************************************
 *  Copyright (C) 2019 by Rituka Patwal <ritukapatwal21@gmail.com>                   *
 *                                                                                   *
 *  This library is free software; you can redistribute it and/or                    *
 *  modify it under the terms of the GNU Lesser General Public                       *
 *  License as published by the Free Software Foundation; either                     *
 *  version 2.1 of the License, or (at your option) any later version.               *
 *                                                                                   *
 *  This library is distributed in the hope that it will be useful,                  *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                *
 *  Lesser General Public License for more details.                                  *
 *                                                                                   *
 *  You should have received a copy of the GNU Lesser General Public                 *
 *  License along with this library; if not, write to the Free Software              *
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA   *
 *************************************************************************************/

#ifndef KPEOPLESINK_H
#define KPEOPLESINK_H

#include <QSet>

#include <KPeopleBackend/AbstractContact>
#include <KPeopleBackend/AllContactsMonitor>

#include <sink/notification.h>
#include <sink/notifier.h>
#include <sink/store.h>

using namespace Sink;
using namespace Sink::ApplicationDomain;

class Q_DECL_EXPORT KPeopleSink : public KPeople::AllContactsMonitor
{
    Q_OBJECT
    Notifier *m_notifier;
    QMap<QString, KPeople::AbstractContact::Ptr> m_contactUriMap;

public:
    KPeopleSink();
    virtual ~KPeopleSink();

    QMap<QString, KPeople::AbstractContact::Ptr> contacts() override;

public slots:
    void initialSinkContactstoKpeople();
    void processRecentlySyncedContacts(QByteArray resourceId);
    void toRemoveContact(QSet<QString> contactUri);

private:
    QString getUri(Contact sinkContact, QByteArray resourceId);
};

#endif

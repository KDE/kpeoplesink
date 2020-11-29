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

#include "sinkdatasource.h"

#include <KPluginFactory>

#include <sink/notification.h>
#include <sink/notifier.h>
#include <sink/store.h>

#include "kpeoplesink.h"

using namespace KPeople;
using namespace Sink;
using namespace Sink::ApplicationDomain;

KPeopleSinkDataSource::KPeopleSinkDataSource(QObject *parent, const QVariantList &args)
    : BasePersonsDataSourceV2(parent)
{
    Q_UNUSED(args);
}

bool KPeopleSinkDataSource::addContact(const QVariantMap &properties)
{
    const QList<Addressbook> sinkAdressbooks = Sink::Store::read<Addressbook>(Sink::Query());
    QByteArray vcard = properties.value("vcard").toByteArray();

    if (sinkAdressbooks.isEmpty()) {
        return false;
    }
    // get resourceId
    QByteArray resourceId = sinkAdressbooks.first().resourceInstanceIdentifier();
    auto contact = ApplicationDomainType::createEntity<Contact>(resourceId);

    contact.setVcard(vcard);
    contact.setResource(resourceId);
    contact.setAddressbook(sinkAdressbooks.first());

    Sink::Store::create<Contact>(contact).exec();

    return true;
}

bool KPeopleSinkDataSource::deleteContact(const QString &uri)
{
    QByteArray resourceId = uri.mid(7, 38).toUtf8();
    QString uid = uri.mid(46);
    qDebug() << resourceId << uid;

    const QList<Contact> contacts = Sink::Store::read<Contact>(Sink::Query().resourceFilter(resourceId));
    for (const Contact &contact : contacts) {
        QString uid1 = contact.getUid();
        if (uid == uid1) {
            Sink::Store::remove<Contact>(contact).exec();
            return true;
        }
    }
    return false;
}

QString KPeopleSinkDataSource::sourcePluginId() const
{
    return QStringLiteral("sink");
}

AllContactsMonitor *KPeopleSinkDataSource::createAllContactsMonitor()
{
    return new KPeopleSink();
}

K_PLUGIN_CLASS_WITH_JSON(KPeopleSinkDataSource, "kpeoplesink.json")

#include "sinkdatasource.moc"

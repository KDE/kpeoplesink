/*
 *  SPDX-FileCopyrightText: 2019 Rituka Patwal <ritukapatwal21@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

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

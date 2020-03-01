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
#include "sync-contacts.h"

#include <sink/resourcecontrol.h>
#include <sink/secretstore.h>
#include <sink/store.h>

#include <Accounts/Account>
#include <Accounts/AccountService>
#include <Accounts/Manager>
#include <Accounts/Service>

#include "core.h"
#include "getcredentialsjob.h"

SyncContacts::SyncContacts()
{
}

SyncContacts::SyncContacts(quint32 accountId, QString server, QString userName, QString password)
    : m_accountId(accountId)
    , m_server(server)
    , m_userName(userName)
    , m_password(password)
{
}

SyncContacts::~SyncContacts()
{
}

void SyncContacts::createResource()
{
    Accounts::Account *account = KAccounts::accountsManager()->account(m_accountId);

    const Accounts::ServiceList services = account->services(QStringLiteral("dav-contacts"));

    for (const auto &service : services) {
        account->selectService(service);

        QString rid = account->value("sink/resourceId").toString();

        if (rid.isEmpty()) {
            QByteArray b;
            b.setNum(m_accountId);
            auto resource = Sink::ApplicationDomain::CardDavResource::create(b);
            resource.setProperty("server", m_server);
            resource.setProperty("username", m_userName);
            rid = resource.identifier();

            Sink::Store::create(resource).exec().waitForFinished();
            qDebug() << "***CONTACT CREATED***";

            account->setValue("sink/resourceId", rid);
            account->syncAndBlock();
        }

        Sink::SecretStore::instance().insert(rid.toUtf8(), m_password);
        synchContact(rid.toUtf8());
    }
}

void SyncContacts::synchContact(QByteArray rid)
{
    // start resourcecontrol
    Sink::ResourceControl::start(rid);

    // Sync Addressbooks
    Sink::SyncScope scope1;
    scope1.setType<Sink::ApplicationDomain::Addressbook>();
    scope1.resourceFilter(rid);
    Sink::Store::synchronize(scope1).exec().waitForFinished();

    // flush
    Sink::ResourceControl::flushMessageQueue(rid).exec().waitForFinished();

    // Sync Contacts
    Sink::SyncScope scope2;
    scope2.setType<Sink::ApplicationDomain::Contact>();
    scope2.resourceFilter(rid);
    Sink::Store::synchronize(scope2).exec().waitForFinished();

    // flush
    Sink::ResourceControl::flushMessageQueue(rid).exec().waitForFinished();
}

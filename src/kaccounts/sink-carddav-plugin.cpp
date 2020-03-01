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

#include "sink-carddav-plugin.h"

#include <QDebug>
#include <QTimer>
#include <QUrl>

#include <KJob>
#include <KPluginFactory>

#include <Accounts/Account>
#include <Accounts/AccountService>
#include <Accounts/Manager>
#include <Accounts/Service>

#include <Core>
#include <GetCredentialsJob>

#include "sync-contacts.h"

K_PLUGIN_CLASS_WITH_JSON(KAccountsCardDavSinkPlugin, "sink-carddav.json")

KAccountsCardDavSinkPlugin::KAccountsCardDavSinkPlugin(QObject *parent, const QVariantList &args)
    : KAccountsDPlugin(parent, args)
{
    syncAccounts();
}

void KAccountsCardDavSinkPlugin::syncAccounts()
{
    Accounts::AccountIdList accountList = KAccounts::accountsManager()->accountList(QStringLiteral("dav-contacts"));

    Q_FOREACH (const quint32 accountId, accountList) {
        qDebug() << "Account IDs : " << accountId;
        getCredentials(accountId);
    }
}

void KAccountsCardDavSinkPlugin::onAccountCreated(const Accounts::AccountId accountId, const Accounts::ServiceList &serviceList)
{
    Accounts::Account *account = KAccounts::accountsManager()->account(accountId);

    if (!account) {
        qWarning() << "Invalid account for id" << accountId;
        return;
    }

    Q_FOREACH (const Accounts::Service &service, serviceList) {
        account->selectService(service);
        if (service.serviceType() == QLatin1String("dav-contacts") && account->isEnabled()) {
            getCredentials(accountId);
        }
    }
}

void KAccountsCardDavSinkPlugin::onServiceEnabled(const Accounts::AccountId accountId, const Accounts::Service &service)
{
    Accounts::Account *account = KAccounts::accountsManager()->account(accountId);

    if (!account) {
        qWarning() << "Invalid account for id" << accountId;
        return;
    }

    if (service.serviceType() == QLatin1String("dav-contacts")) {
        getCredentials(accountId);
    }
}

void KAccountsCardDavSinkPlugin::getCredentials(const Accounts::AccountId accountId)
{
    GetCredentialsJob *credentialsJob = new GetCredentialsJob(accountId, this);
    connect(credentialsJob, &GetCredentialsJob::finished, this, &KAccountsCardDavSinkPlugin::getAccountDetails);
    credentialsJob->start();
}

void KAccountsCardDavSinkPlugin::getAccountDetails(KJob *job)
{
    GetCredentialsJob *credentialsJob = qobject_cast<GetCredentialsJob *>(job);
    job->deleteLater();

    const QVariantMap &data = credentialsJob->credentialsData();
    Accounts::Account *account = KAccounts::accountsManager()->account(credentialsJob->accountId());

    QUrl carddavUrl;
    carddavUrl.setHost(account->value("dav/host").toString());
    carddavUrl.setScheme("https");

    const QString &userName = data.value("AccountUsername").toString();
    const QString &password = data.value("Secret").toString();

    qDebug() << "username : " << userName << "\npassword : " << password;

    SyncContacts *synccontact = new SyncContacts(credentialsJob->accountId(), carddavUrl.toString(), userName, password);
    synccontact->createResource();
}

void KAccountsCardDavSinkPlugin::onAccountRemoved(const Accounts::AccountId accountId)
{
}

void KAccountsCardDavSinkPlugin::onServiceDisabled(const Accounts::AccountId accountId, const Accounts::Service &service)
{
}

#include "sink-carddav-plugin.moc"

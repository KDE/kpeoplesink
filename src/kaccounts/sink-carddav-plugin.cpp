/*
 *  SPDX-FileCopyrightText: 2019 Rituka Patwal <ritukapatwal21@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

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
    const Accounts::AccountIdList accountList = KAccounts::accountsManager()->accountList(QStringLiteral("dav-contacts"));

    for (const quint32 accountId : accountList) {
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

    for (const Accounts::Service &service : serviceList) {
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
    Q_UNUSED(accountId)
}

void KAccountsCardDavSinkPlugin::onServiceDisabled(const Accounts::AccountId accountId, const Accounts::Service &service)
{
    Q_UNUSED(accountId)
    Q_UNUSED(service)
}

#include "sink-carddav-plugin.moc"

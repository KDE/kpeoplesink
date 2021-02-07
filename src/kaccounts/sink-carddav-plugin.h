/*
 *  SPDX-FileCopyrightText: 2019 Rituka Patwal <ritukapatwal21@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

#ifndef KACCOUNTSCARDDAVSINKPLUGIN_H
#define KACCOUNTSCARDDAVSINKPLUGIN_H

#include <QTimer>

#include <KJob>

#include <KAccountsDPlugin>

class KAccountsCardDavSinkPlugin : public KAccountsDPlugin
{
    Q_OBJECT

public:
    KAccountsCardDavSinkPlugin(QObject *parent, const QVariantList &args);

public Q_SLOTS:
    void onAccountCreated(const Accounts::AccountId accountId, const Accounts::ServiceList &serviceList) override;
    void onAccountRemoved(const Accounts::AccountId accountId) override;
    void onServiceEnabled(const Accounts::AccountId accountId, const Accounts::Service &service) override;
    void onServiceDisabled(const Accounts::AccountId accountId, const Accounts::Service &service) override;

private Q_SLOTS:
    void getCredentials(const Accounts::AccountId accountId);
    void getAccountDetails(KJob *job);
    void syncAccounts();
};

#endif // KACCOUNTSCARDDAVPLUGIN_H

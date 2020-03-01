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

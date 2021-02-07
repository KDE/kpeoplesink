/*
 *  SPDX-FileCopyrightText: 2019 Rituka Patwal <ritukapatwal21@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

#pragma once

#include <KPeopleBackend/BasePersonsDataSource>

class KPeopleSinkDataSource : public KPeople::BasePersonsDataSourceV2
{
public:
    KPeopleSinkDataSource(QObject *parent, const QVariantList &data);
    QString sourcePluginId() const override;

    bool addContact(const QVariantMap &properties) override;
    bool deleteContact(const QString &uri) override;
    KPeople::AllContactsMonitor *createAllContactsMonitor() override;
};

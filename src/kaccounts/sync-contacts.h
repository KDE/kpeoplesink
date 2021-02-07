/*
 *  SPDX-FileCopyrightText: 2019 Rituka Patwal <ritukapatwal21@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

#pragma once

#include <QObject>
#include <QUrl>

#include <sink/store.h>

class SyncContacts : public QObject
{
    Q_OBJECT

public:
    SyncContacts();
    SyncContacts(quint32 accountId, QString server, QString userName, QString password);
    ~SyncContacts();

public Q_SLOTS:
    void createResource();
    void synchContact(QByteArray rid);

private:
    quint32 m_accountId;
    QUrl m_server;
    QString m_userName;
    QString m_password;
};

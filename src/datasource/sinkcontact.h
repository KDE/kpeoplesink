/*
 *  SPDX-FileCopyrightText: 2019 Rituka Patwal <ritukapatwal21@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

#pragma once

#include <QImage>

#include <KContacts/Addressee>
#include <KPeopleBackend/AbstractEditableContact>

#include <sink/applicationdomaintype.h>

class SinkContact : public KPeople::AbstractEditableContact
{
public:
    SinkContact(const Sink::ApplicationDomain::Contact &contact);

    QVariant customProperty(const QString &key) const override;

    void setAddressee(const Sink::ApplicationDomain::Contact &contact);
    Sink::ApplicationDomain::Contact contact() const;

    bool setCustomProperty(const QString &key, const QVariant &value) override;

private:
    Sink::ApplicationDomain::Contact m_contact;
    KContacts::Addressee m_addressee;
};

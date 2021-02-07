/*
 *  SPDX-FileCopyrightText: 2019 Rituka Patwal <ritukapatwal21@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "sinkcontact.h"

#include <KContacts/VCardConverter>

#include <sink/store.h>

SinkContact::SinkContact(const Sink::ApplicationDomain::Contact &contact)
    : m_contact(contact)
{
    setAddressee(contact);
}

QVariant SinkContact::customProperty(const QString &key) const
{
    QVariant ret;
    if (key == NameProperty) {
        const QString name = m_addressee.realName();
        if (!name.isEmpty()) {
            return name;
        }
        if (!m_addressee.phoneNumbers().isEmpty()) {
            return m_addressee.phoneNumbers().at(0).number();
        }
        if (!m_addressee.preferredEmail().isEmpty()) {
            return m_addressee.preferredEmail();
        }
        return QVariant();
    } else if (key == VCardProperty) {
        return m_contact.getVcard();
    } else if (key == PictureProperty) {
        return m_addressee.photo().data();
    } else if (key == EmailProperty)
        return m_addressee.preferredEmail();

    else if (key == AllPhoneNumbersProperty) {
        QVariantList numbers;
        const auto phoneNumbers = m_addressee.phoneNumbers();
        for (const KContacts::PhoneNumber &phoneNumber : phoneNumbers) {
            numbers << phoneNumber.number();
        }
        return numbers;
    } else if (key == PhoneNumberProperty) {
        return m_addressee.phoneNumbers().isEmpty() ? QVariant() : m_addressee.phoneNumbers().at(0).number();
    }

    return ret;
}

void SinkContact::setAddressee(const Sink::ApplicationDomain::Contact &contact)
{
    m_contact = contact;
    KContacts::VCardConverter converter;
    m_addressee = converter.parseVCard(contact.getVcard());
}

Sink::ApplicationDomain::Contact SinkContact::contact() const
{
    return m_contact;
}

bool SinkContact::setCustomProperty(const QString &key, const QVariant &value)
{
    if (key == VCardProperty) {
        const QByteArray rawVCard = value.toByteArray();
        m_contact.setVcard(rawVCard);

        KContacts::VCardConverter converter;
        m_addressee = converter.parseVCard(rawVCard);
        // 1. fetch resourceId of contact
        QByteArray resourceID = m_contact.resourceInstanceIdentifier();
        // 2. call modify function
        Sink::Store::modify<Sink::ApplicationDomain::Contact>(Sink::Query().resourceFilter(resourceID), m_contact);
        return true;
    }
    return false;
}

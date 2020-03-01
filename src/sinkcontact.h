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
/*
 *  Copyright 2013-2014  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of
 *  the License or (at your option) version 3 or any later version
 *  accepted by the membership of KDE e.V. (or its successor approved
 *  by the membership of KDE e.V.), which shall act as a proxy
 *  defined in Section 14 of version 3 of the license.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


#include "parameter.h"

Parameter::Parameter(QObject* parent)
    : QObject(parent)
{

}

QString Parameter::name() const
{
    return m_name;
}

void Parameter::setName(const QString &name)
{
    m_name = name;
}

QString Parameter::info() const
{
    return m_info;
}

void Parameter::setInfo(const QString &info)
{
    m_info = info;
}

QString Parameter::type() const
{
    return m_type;
}

void Parameter::setType(const QString &type)
{
    m_type = type;
}

void Parameter::setTypeLink(const QString& link)
{
    m_typeLink = link;
}

QString Parameter::typeLink() const
{
    return m_typeLink;
}

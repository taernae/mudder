/***************************************************************************
 *   Copyright (C) 2013 by Jason Douglas                                   *
 *   larkin.dischai@gmail.com                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef XMLEXCEPTION_H
#define XMLEXCEPTION_H

#include <QString>
#include <QStringList>

class XmlException
{
public:
    XmlException(const QString &first) { m_warnings.append(first); }
    XmlException(const QStringList &list) { m_warnings = list; }

    void addWarning(const QString &warning) { m_warnings.append(warning); }
    void addWarnings(const QStringList &warnings) { m_warnings.append(warnings); }

    const QStringList & warnings() const { return m_warnings; }

private:
    QStringList m_warnings;
};

#endif // XMLEXCEPTION_H

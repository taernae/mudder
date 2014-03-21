/*
  Mudder, a cross-platform text gaming client

  Copyright (C) 2014 Jason Douglas
  larkin.dischai@gmail.com

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/


#ifndef PROFILE_H
#define PROFILE_H

#include <QHash>
#include <QList>
#include <QObject>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class Group;
class ProfileItem;
class XmlError;

class Profile : public QObject
{
    Q_OBJECT
public:
    explicit Profile(QObject *parent = 0);

    Group * rootGroup() const { return m_root; }
    Group * activeGroup() const { return m_activeGroup?m_activeGroup:m_root; }
    void setActiveGroup(Group *group);

    virtual void toXml(QXmlStreamWriter &xml);
    virtual void fromXml(QXmlStreamReader &xml, QList<XmlError *> &errors);

signals:
    void optionsChanged();
    void settingsChanged();

private:
    Group *m_root;
    Group *m_activeGroup;

    QHash<QString, ProfileItem *> m_itemMap;
};

#endif // PROFILE_H

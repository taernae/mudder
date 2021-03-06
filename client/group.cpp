/*
  Mudder, a cross-platform text gaming client

  Copyright (C) 2014 Jason Douglas
  jkdoug@gmail.com

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


#include "group.h"
#include "xmlerror.h"
#include "accelerator.h"
#include "alias.h"
#include "event.h"
#include "timer.h"
#include "trigger.h"
#include "variable.h"

Group::Group(QObject *parent) :
    ProfileItem(parent)
{
}

int Group::itemCount(bool recurse) const
{
    int count = m_items.count();
    if (recurse)
    {
        foreach (Group *group, m_groups)
        {
            count += group->itemCount(true);
        }
    }
    return count;
}

QList<Accelerator *> Group::sortedAccelerators(bool enabled, bool all) const
{
    return sortedItems<Accelerator>(enabled, all);
}

QList<Alias *> Group::sortedAliases(bool enabled, bool all) const
{
    return sortedItems<Alias>(enabled, all);
}

QList<Event *> Group::sortedEvents(bool enabled, bool all) const
{
    return sortedItems<Event>(enabled, all);
}

QList<Group *> Group::sortedGroups(bool enabled, bool all) const
{
    return sortedItems<Group>(enabled, all);
}

QList<Timer *> Group::sortedTimers(bool enabled, bool all) const
{
    return sortedItems<Timer>(enabled, all);
}

QList<Trigger *> Group::sortedTriggers(bool enabled, bool all) const
{
    return sortedItems<Trigger>(enabled, all);
}

QList<Variable *> Group::sortedVariables(bool all) const
{
    return sortedItems<Variable>(false, all);
}

void Group::toXml(QXmlStreamWriter &xml)
{
    if (!name().isEmpty())
    {
        ProfileItem::toXml(xml);
    }

    foreach (ProfileItem *item, m_items)
    {
        xml.writeStartElement(item->tagName());
        item->toXml(xml);
        xml.writeEndElement();
    }
}

void Group::fromXml(QXmlStreamReader &xml, QList<XmlError *> &errors)
{
    QString name(xml.attributes().value("name").toString());
    if (!validateName(name, group() != 0))
    {
        XmlError *err = new XmlError(xml.lineNumber(), xml.columnNumber());
        if (name.isEmpty())
        {
            err->setMessage(tr("missing 'name' attribute"));
        }
        else
        {
            err->setMessage(tr("invalid value for 'name' attribute: %1").arg(name));
        }
        errors << err;

        name = unnamed();
    }
    setName(name);

    enable(xml.attributes().value("enabled").compare("n", Qt::CaseInsensitive) != 0);
}

template<class C>
QList<C *> Group::sortedItems(bool enabled, bool all) const
{
    QList<C *> list;
    foreach (ProfileItem *item, m_items)
    {
        C *sub = qobject_cast<C *>(item);
        if (sub && (!enabled || sub->enabled()))
        {
            list << sub;
        }
    }

    if (all)
    {
        foreach (Group *group, m_groups)
        {
            list << group->sortedItems<C>(enabled, all);
        }
    }

    qSort(list.begin(), list.end());

    return list;
}

Group * Group::createGroup(const QString &name)
{
    Group *child = new Group(this);
    child->setName(name);

    m_items << child;
    m_groups << child;

    connect(child, SIGNAL(modified(ProfileItem*)), SIGNAL(modified(ProfileItem*)));

    return child;
}

int Group::addItem(ProfileItem *item)
{
    int row = m_items.indexOf(item);
    if (row >= 0)
    {
        Q_ASSERT(item->group() == this);
        return row;
    }

    if (item->group())
    {
        item->disconnect(item->group());
    }

    item->setParent(this);
    m_items << item;

    connect(item, SIGNAL(modified(ProfileItem *)), SIGNAL(modified(ProfileItem *)));

    Group *group = qobject_cast<Group *>(item);
    if (group)
    {
        m_groups << group;
    }

    return m_items.indexOf(item);
}

int Group::removeItem(ProfileItem *item)
{
    Group *group = qobject_cast<Group *>(item);
    if (group)
    {
        m_groups.removeOne(group);
    }

    int row = m_items.indexOf(item);
    m_items.removeOne(item);

    item->disconnect(this);

    return row;
}

int Group::removeItem(int index)
{
    if (index < 0 || index >= m_items.count())
    {
        return -1;
    }

    return removeItem(m_items.at(index));
}

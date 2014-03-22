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


#ifndef EVENT_H
#define EVENT_H

#include "executable.h"

class Event : public Executable
{
    Q_OBJECT
public:
    explicit Event(QObject *parent = 0);
    Event(const Event &rhs, QObject *parent = 0);

    Event & operator =(const Event &rhs);

    bool operator ==(const Event &rhs);
    bool operator !=(const Event &rhs);

    static bool lessSequence(const Event *lhs, const Event *rhs);

    virtual QString tagName() const { return "event"; }
    virtual void toXml(QXmlStreamWriter &xml);
    virtual void fromXml(QXmlStreamReader &xml, QList<XmlError *> &errors);

private:
    void clone(const Event &rhs);
};

#endif // EVENT_H

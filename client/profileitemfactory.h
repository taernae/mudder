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


#ifndef PROFILEITEMFACTORY_H
#define PROFILEITEMFACTORY_H

#include <QString>

class EditSetting;
class Group;
class ProfileItem;

class ProfileItemFactory
{
public:
    static ProfileItem * create(const QString &name, Group *parent = 0);
    static EditSetting * editor(const QString &name);

private:
    ProfileItemFactory() {}
};

#endif // PROFILEITEMFACTORY_H

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


#include "edittimer.h"
#include "ui_edittimer.h"
#include "luahighlighter.h"
#include "timer.h"
#include <QMessageBox>

EditTimer::EditTimer(QWidget *parent) :
    EditSetting(parent),
    ui(new Ui::EditTimer)
{
    ui->setupUi(this);

    connect(ui->interval, SIGNAL(timeChanged(QTime)), SLOT(changed()));
    connect(ui->name, SIGNAL(textChanged(QString)), SLOT(changed()));
    connect(ui->enabled, SIGNAL(stateChanged(int)), SLOT(changed()));
    connect(ui->once, SIGNAL(stateChanged(int)), SLOT(changed()));
    connect(ui->script, SIGNAL(textChanged()), SLOT(changed()));
}

EditTimer::~EditTimer()
{
    delete ui;
}

bool EditTimer::load(ProfileItem *item)
{
    Q_ASSERT(item != 0);
    if (!item)
    {
        return false;
    }

    Timer *timer = qobject_cast<Timer *>(item);
    Q_ASSERT(timer != 0);
    if (!timer)
    {
        return false;
    }

    m_name = timer->name();
    ui->name->setText(m_name);

    m_enabled = timer->enabledFlag();
    ui->enabled->setChecked(m_enabled);

    m_interval = timer->interval();
    ui->interval->setTime(m_interval);

    m_once = timer->once();
    ui->once->setChecked(m_once);

    m_contents = timer->contents();
    ui->script->setPlainText(m_contents);
    ui->script->setSyntaxHighlighter(new LuaHighlighter());

    ui->timesFired->setText(QLocale::system().toString(timer->firedCount()));
    ui->avgTime->setText(QString::number(timer->averageTime()));

    return true;
}

bool EditTimer::save(ProfileItem *item)
{
    Q_ASSERT(item != 0);
    if (!item)
    {
        return false;
    }

    Timer *timer = qobject_cast<Timer *>(item);
    Q_ASSERT(timer != 0);
    if (!timer)
    {
        return false;
    }

    QString name(ui->name->text());
    if (name.isEmpty())
    {
        QMessageBox::critical(this, tr("Invalid Timer"), tr("Name may not be left empty."));
        return false;
    }
    if (!ProfileItem::validateName(name))
    {
        QMessageBox::critical(this, tr("Invalid Timer"), tr("You may only use alphanumeric characters, underscores, and certain special characters in the name."));
        return false;
    }

    QString contents(ui->script->toPlainText().trimmed());
    if (contents.isEmpty())
    {
        QMessageBox::critical(this, tr("Invalid Timer"), tr("Script may not be left empty."));
        return false;
    }

    m_name = name;
    timer->setName(m_name);

    m_interval = ui->interval->time();
    timer->setInterval(m_interval);

    m_enabled = ui->enabled->isChecked();
    timer->enable(m_enabled);
    m_once = ui->once->isChecked();
    timer->setOnce(m_once);

    m_contents = contents;
    timer->setContents(m_contents);

    timer->setFailed(false);

    emit itemModified(false, true);

    return true;
}

void EditTimer::enableItem(bool flag)
{
    m_enabled = flag;
    ui->enabled->setChecked(flag);
}

void EditTimer::changed()
{
    if (m_name.isNull())
    {
        emit itemModified(false, false);
        return;
    }

    QString script(ui->script->toPlainText().trimmed());

    bool changed = m_name != ui->name->text() ||
        m_enabled != ui->enabled->isChecked() ||
        m_interval != ui->interval->time() ||
        m_once != ui->once->isChecked() ||
        m_contents != script;

    bool valid = !ui->name->text().isEmpty() &&
        ui->interval->time().isValid() &&
        !script.isEmpty();

    emit itemModified(changed, valid);
}

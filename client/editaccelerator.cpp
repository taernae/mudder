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


#include "editaccelerator.h"
#include "ui_editaccelerator.h"
#include "luahighlighter.h"
#include "logger.h"
#include "accelerator.h"

EditAccelerator::EditAccelerator(QWidget *parent) :
    EditSetting(parent),
    ui(new Ui::EditAccelerator)
{
    ui->setupUi(this);

    connect(ui->name, SIGNAL(textChanged(QString)), SLOT(changed()));
    connect(ui->script, SIGNAL(textChanged()), SLOT(changed()));
    connect(ui->enabled, SIGNAL(stateChanged(int)), SLOT(changed()));
    connect(ui->key, SIGNAL(keySequenceChanged(QKeySequence)), SLOT(changed()));
}

EditAccelerator::~EditAccelerator()
{
    delete ui;
}

bool EditAccelerator::load(ProfileItem *item)
{
    Q_ASSERT(item != 0);
    if (!item)
    {
        LOG_ERROR("Attempted to load an accelerator from a null address.");
        return false;
    }

    LOG_TRACE("EditAccelerator::load", item->fullName());

    Accelerator *accelerator = qobject_cast<Accelerator *>(item);
    if (!accelerator)
    {
        LOG_ERROR("Attempted to load an accelerator from a non-accelerator item.");
        return false;
    }

    m_key = accelerator->key();
    ui->key->setKeySequence(m_key);

    m_name = accelerator->name();
    ui->name->setText(m_name);

    m_enabled = accelerator->enabledFlag();
    ui->enabled->setChecked(m_enabled);

    m_contents = accelerator->contents();
    ui->script->setPlainText(m_contents);
    ui->script->setSyntaxHighlighter(new LuaHighlighter());

    ui->timesFired->setText(QLocale::system().toString(accelerator->executionCount()));
    ui->avgTime->setText(QString::number(accelerator->averageTime()));

    return true;
}

bool EditAccelerator::save(ProfileItem *item)
{
    if (!item)
    {
        LOG_ERROR("Attempted to save an accelerator to a null address.");
        return false;
    }

    LOG_TRACE("EditAccelerator::save", item->fullName());

    Accelerator *accelerator = qobject_cast<Accelerator *>(item);
    if (!accelerator)
    {
        LOG_ERROR("Attempted to save an accelerator to a non-accelerator item.");
        return false;
    }

    accelerator->setKey(ui->key->keySequence());
    accelerator->setName(ui->name->text());
    accelerator->enable(ui->enabled->isChecked());
    accelerator->setContents(ui->script->toPlainText().trimmed());
    accelerator->setFailed(false);

    changed();

    return true;
}

void EditAccelerator::changed()
{
    if (m_name.isNull())
    {
        emit itemModified(false, false);
        return;
    }

    QString script(ui->script->toPlainText().trimmed());

    bool changed = m_name != ui->name->text() ||
            m_key != ui->key->keySequence() ||
            m_enabled != ui->enabled->isChecked() ||
            m_contents != script;

    bool valid = !ui->name->text().isEmpty() &&
            !ui->key->keySequence().isEmpty();

    emit itemModified(changed, valid);
}
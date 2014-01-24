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


#include "codeeditorwindow.h"
#include "luahighlighter.h"
#include "options.h"
#include "xmlhighlighter.h"
#include <QAction>
#include <QFileDialog>
#include <QFileInfo>
#include <QMdiArea>
#include <QMessageBox>
#include <QTabBar>
#include <QToolBar>

struct CodeEditorWindowData
{
    CodeEditorWindowData() :
        mdi(0),
        subWindow(0),
        editor(0),
        toolBar(0),
        actionNew(0),
        actionOpen(0),
        actionSave(0),
        actionSaveAs(0)
    {}

    QMdiArea * mdi;
    QMdiSubWindow * subWindow;
    CodeEditorWidget * editor;
    QToolBar * toolBar;

    QString defaultPath;

    QAction * actionNew;
    QAction * actionOpen;
    QAction * actionSave;
    QAction * actionSaveAs;
};

CodeEditorWindow::CodeEditorWindow(QWidget *parent) :
    QMainWindow(parent)
{
    d = new CodeEditorWindowData;

    d->defaultPath = OPTIONS->homePath();

    d->mdi = new QMdiArea(this);
    d->mdi->setDocumentMode(true);
    d->mdi->setViewMode(QMdiArea::TabbedView);
    d->mdi->setTabsClosable(true);
    d->mdi->setTabsMovable(true);
    d->mdi->setOption(QMdiArea::DontMaximizeSubWindowOnActivation, false);
    connect(d->mdi, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(subWindowActivated(QMdiSubWindow*)));

    setCentralWidget(d->mdi);

    d->toolBar = new QToolBar(this);
    d->toolBar->setIconSize(QSize(16, 16));

    d->actionNew = new QAction(tr("&New"), this);
    d->actionNew->setIcon(QIcon(":/icons/small_new"));
    d->actionNew->setToolTip(tr("Create new file"));
    d->actionNew->setShortcut(QKeySequence::New);
    d->actionNew->setShortcutContext(Qt::WindowShortcut);
    d->toolBar->addAction(d->actionNew);
    connect(d->actionNew, SIGNAL(triggered()), this, SLOT(actionNew()));

    d->actionOpen = new QAction(tr("&Open"), this);
    d->actionOpen->setIcon(QIcon(":/icons/small_open"));
    d->actionOpen->setToolTip(tr("Load file from disk"));
    d->actionOpen->setShortcut(QKeySequence::Open);
    d->actionOpen->setShortcutContext(Qt::WindowShortcut);
    d->toolBar->addAction(d->actionOpen);
    connect(d->actionOpen, SIGNAL(triggered()), this, SLOT(actionOpen()));

    d->actionSave = new QAction(tr("&Save"), this);
    d->actionSave->setIcon(QIcon(":/icons/small_save"));
    d->actionSave->setToolTip(tr("Save file to disk"));
    d->actionSave->setShortcut(QKeySequence::Save);
    d->actionSave->setShortcutContext(Qt::WindowShortcut);
    d->toolBar->addAction(d->actionSave);
    connect(d->actionSave, SIGNAL(triggered()), this, SLOT(actionSave()));

    d->actionSaveAs = new QAction(tr("Save &As"), this);
    d->actionSaveAs->setIcon(QIcon(":/icons/small_save_as"));
    d->actionSaveAs->setToolTip(tr("Save file to disk with a new name"));
    d->actionSaveAs->setShortcut(QKeySequence::SaveAs);
    d->actionSaveAs->setShortcutContext(Qt::WindowShortcut);
    d->toolBar->addAction(d->actionSaveAs);
    connect(d->actionSaveAs, SIGNAL(triggered()), this, SLOT(actionSaveAs()));

    addToolBar(d->toolBar);

    QTabBar * mdiTabBar = d->mdi->findChild<QTabBar *>();
    Q_ASSERT(mdiTabBar != 0);
    mdiTabBar->setExpanding(false);
}

QString CodeEditorWindow::defaultPath() const
{
    return d->defaultPath;
}

void CodeEditorWindow::setDefaultPath(const QString &path)
{
    d->defaultPath = path;
}

bool CodeEditorWindow::loadFile(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        return false;
    }

    CodeEditorWidget * widget = new CodeEditorWidget(this);
    if (!widget->loadFile(fileName))
    {
        OPTIONS->removeRecentFile(fileName);

        QMessageBox::warning(this, tr("File Error"), tr("Failed to load file."));

        delete widget;
        return false;
    }

    OPTIONS->addRecentFile(fileName);
//        statusBar()->showMessage(tr("Script loaded"), 2000);

//        connect(script->document(), SIGNAL(contentsChanged()), this, SLOT(onScriptChanged()));

    QMdiSubWindow *win = new QMdiSubWindow(this);
    win->setWidget(widget);
    win->setWindowTitle(QFileInfo(fileName).fileName());
    win->setWindowIcon(QIcon());
    win->setAttribute(Qt::WA_DeleteOnClose);

    QFileInfo fi(fileName);
    if (fi.suffix().compare("lua", Qt::CaseInsensitive) == 0)
    {
        widget->setSyntaxHighlighter(new LuaHighlighter());
    }
    else if (fi.suffix().compare("mp", Qt::CaseInsensitive) == 0 ||
             fi.suffix().compare("xml", Qt::CaseInsensitive) == 0)
    {
        widget->setSyntaxHighlighter(new XmlHighlighter());
    }

    d->mdi->addSubWindow(win);

    win->showMaximized();

    return true;
}

void CodeEditorWindow::actionNew()
{
    static int num = 0;

    CodeEditorWidget * widget = new CodeEditorWidget(this);

    QMdiSubWindow * win = new QMdiSubWindow(this);
    win->setWidget(widget);
    win->setWindowTitle(tr("(untitled-%1)").arg(++num));
    win->setAttribute(Qt::WA_DeleteOnClose);
    widget->setSyntaxHighlighter(new LuaHighlighter());

    d->mdi->addSubWindow(win);

    win->showMaximized();

//    connect(script->document(), SIGNAL(contentsChanged()), this, SLOT(onScriptChanged()));

//    updateActions();
}

void CodeEditorWindow::actionOpen()
{
    QString path(d->defaultPath);

    QString fileName(QFileDialog::getOpenFileName(this, tr("Open File"), path, tr("Lua script files (*.lua);;XML source files (*.xml *.mp);;All files (*)")));
    loadFile(fileName);
}

bool CodeEditorWindow::actionSave()
{
    if (!d->editor)
    {
        return false;
    }

    if (!d->editor->fileName().isEmpty())
    {
        if (d->editor->saveFile())
        {
            OPTIONS->addRecentFile(d->editor->fileName());
            return true;
        }

        return false;
    }

    return actionSaveAs();
}

bool CodeEditorWindow::actionSaveAs()
{
    if (!d->editor)
    {
        return false;
    }

    QString path(d->defaultPath);

    if (!d->editor->fileName().isEmpty())
    {
        QFileInfo fi(d->editor->fileName());
        QDir dir(fi.path());
        if (dir.exists())
        {
            path = fi.filePath();
        }
    }

    QString fileName(QFileDialog::getSaveFileName(this, tr("Save As"), path, tr("Lua script files (*.lua);;XML source files (*.xml *.mp);;All files (*)")));
    if (!fileName.isEmpty() && d->editor->saveFile(fileName))
    {
        OPTIONS->addRecentFile(fileName);
        return true;
    }

    return false;
}

void CodeEditorWindow::actionPrint()
{

}

void CodeEditorWindow::actionPrintPreview()
{

}

void CodeEditorWindow::actionPrintPdf()
{

}

void CodeEditorWindow::showEditorSettings()
{

}

void CodeEditorWindow::showSearchBox()
{

}

void CodeEditorWindow::printPreview(QPrinter *printer)
{
    Q_UNUSED(printer)
}

void CodeEditorWindow::updateSaveAction()
{

}

void CodeEditorWindow::subWindowActivated(QMdiSubWindow *win)
{
    d->subWindow = win;
    d->editor = win ? qobject_cast<CodeEditorWidget *>(win->widget()) : 0;
}
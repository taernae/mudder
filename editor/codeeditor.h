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


#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include "editor_global.h"
#include <QPlainTextEdit>
#include <QSyntaxHighlighter>

class EDITORSHARED_EXPORT CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = 0);
    virtual ~CodeEditor();

    QSyntaxHighlighter * syntaxHighlighter() const { return m_syntaxHighlighter; }
    void setSyntaxHighlighter(QSyntaxHighlighter *highlighter);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void removeWordHighlighting();

public slots:
    void updateCursorPosition();

    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &rect, int dy);

    void highlightWords(const QStringList &words, const QBrush &brush);

private slots:
    void handleSettingsChange(const QString &key, const QVariant &val);

protected:
    void resizeEvent(QResizeEvent *e);

private:
    QWidget *m_lineNumberArea;
    QSyntaxHighlighter *m_syntaxHighlighter;
    QPair<int, int> m_countCache;
};

#endif // CODEEDITOR_H

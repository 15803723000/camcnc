/******************************************************************************\

  CAMotics is an Open-Source simulation and CAM software.
  Copyright (C) 2011-2019 Joseph Coffland <joseph@cauldrondevelopment.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

\******************************************************************************/

#include "coordtabmanager.h"

#include "QtWin.h"
#include "NCEdit.h"
#include "GCodeHighlighter.h"
#include "TPLHighlighter.h"

#include <camotics/project/File.h>

#include <cbang/Exception.h>
#include <cbang/os/SystemUtilities.h>

#include <QFile>
#include <QString>
#include <QMessageBox>
#include <QSplitter>

using namespace CAMotics;
using namespace cb;
using namespace std;


coordtabmanager::coordtabmanager(QWidget *parent) :
  QTabWidget(parent), win(0), offset(1) {

<<<<<<< HEAD
  cout <<"coord parent: " << parent <<endl;
  //cout <<"coord win: " << win <<endl;

  while (parent && !win) {
    win = dynamic_cast<QtWin *>(parent);
    cout <<"coord win: " << win <<endl;
    parent = parent->parentWidget();
    cout <<"coord parent: " << parent <<endl;
  }

  //cout <<"coord parent: " << parent <<endl;
  //cout <<"coord win: " << win <<endl;

  if (!win) THROW("CoordTab QtWin not found");
=======
  while (parent && !win) {
    win = dynamic_cast<QtWin *>(parent);
    parent = parent->parentWidget();
  }

  if (!win) THROW("QtWin not found");
>>>>>>> xutian push

  connect(this, SIGNAL(tabCloseRequested(int)),
          SLOT(on_tabCloseRequested(int)));
}

<<<<<<< HEAD

=======
>>>>>>> xutian push
void coordtabmanager::open(const SmartPointer<Project::File> &file,
                          int line, int col) {
  // Check if we already have this file open in a tab检查是否已在选项卡中打开此文件

<<<<<<< HEAD
=======

>>>>>>> xutian push
  unsigned tab;
  for (tab = offset; tab < (unsigned)QTabWidget::count(); tab++)
    if (getFile(tab) == file) break;

  // Create new tab创建新选项卡
  if ((unsigned)QTabWidget::count() <= tab) {
    SmartPointer<Highlighter> highlighter;
    if (file->isTPL()) highlighter = new TPLHighlighter;
    else highlighter = new GCodeHighlighter;

    QApplication::setOverrideCursor(Qt::WaitCursor);
    NCEdit *editor = new NCEdit(file, highlighter, this);

    QFile qFile(QString::fromUtf8(file->getPath().c_str()));
    qFile.open(QFile::ReadOnly);
    QString contents = qFile.readAll();
    qFile.close();
    contents.replace('\t', "  ");

    editor->loadDarkScheme();
    editor->setWordWrapMode(QTextOption::NoWrap);
    editor->setPlainText(contents);

    connect(editor, SIGNAL(find()), SIGNAL(find()));
    connect(editor, SIGNAL(findNext()), SIGNAL(findNext()));
    connect(editor, SIGNAL(findResult(bool)), SIGNAL(findResult(bool)));

    QString title = QString::fromUtf8(file->getBasename().c_str());
    tab = (unsigned)QTabWidget::addTab(editor, title);
    QApplication::restoreOverrideCursor();
  }

  // Get editor获得编辑器
  NCEdit *editor = (NCEdit *)QTabWidget::widget(tab);

  // Switch to tab切换到选项卡
  QTabWidget::setCurrentIndex(tab);

  // Select line and column选择行和列
  if (0 < line) {
    QTextCursor c = editor->textCursor();

    c.setPosition(0, QTextCursor::MoveAnchor);
    c.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, line - 1);

    if (0 < col)
      c.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, col);

    c.select(QTextCursor::LineUnderCursor);

    editor->setTextCursor(c);
  }

  // Get input focus获得输入焦点
  editor->setFocus();
}

<<<<<<< HEAD

=======
>>>>>>> xutian push
void coordtabmanager::validateTabIndex(unsigned tab) const {
  if (tab < offset || (unsigned)QTabWidget::count() <= tab)
    THROW("Invalid file tab index " << tab);
}

const SmartPointer<Project::File> &coordtabmanager::getFile(unsigned tab) const {
  validateTabIndex(tab);
  return ((NCEdit *)QTabWidget::widget(tab))->getFile();
}

<<<<<<< HEAD
=======
bool coordtabmanager::isModified(unsigned tab) const {
  validateTabIndex(tab);
  return QTabWidget::tabText(tab).endsWith(" *");
}



bool coordtabmanager::checkSave(unsigned tab) {
  if (!isModified(tab)) return true;

  // Select tab选择选项卡
  QTabWidget::setCurrentIndex(tab);

  int response =
    QMessageBox::question(this, "File Modified", "The current file has "
                          "been modified.  Would you like to save it?",
                          QMessageBox::Cancel | QMessageBox::No |
                          QMessageBox::Yes, QMessageBox::Yes);

  if (response == QMessageBox::Yes) save(tab);
  else if (response == QMessageBox::Cancel) return false;
  return true;
}


bool coordtabmanager::checkSaveAll() {
  bool all = false;

  try {
    for (int tab = offset; tab < QTabWidget::count(); tab++)
      if (isModified(tab)) {
        if (!all) {
          // Select tab
          QTabWidget::setCurrentIndex(tab);

          int response =
            QMessageBox::question(this, "File Modified", "The current file has "
                                  "been modified.  Would you like to save?",
                                  QMessageBox::Cancel | QMessageBox::No |
                                  QMessageBox::Save | QMessageBox::SaveAll,
                                  QMessageBox::SaveAll);

          if (response == QMessageBox::SaveAll) all = true;
          if (response == QMessageBox::No) continue;
          if (response == QMessageBox::Cancel) return false;
        }

        save(tab);
      }

    return true;

  } catch (const Exception &e) {
    win->warning(tr("Failed to save: %1").arg(e.getMessage().c_str()));
  }

  return false;
}


void coordtabmanager::save() {
  save(currentIndex(), false);
}


void coordtabmanager::saveAs() {
  save(currentIndex(), true);
}


void coordtabmanager::save(unsigned tab, bool saveAs) {
  validateTabIndex(tab);
  if (!saveAs && !isModified(tab)) return;

  // Get absolute path获得绝对路径
  NCEdit *editor = (NCEdit *)QTabWidget::widget(tab);
  Project::File &file = *editor->getFile();
  QString path = file.getPath().c_str();

  // Get type打字
  bool tpl = editor->isTPL();

  if (saveAs) {
    path = win->openFile(tr("Save file"), tpl ? "TPL (*.tpl)" :
                         "GCode (*.nc *.ngc *.gcode)", path, true);
    if (path.isEmpty()) return;

    string ext = SystemUtilities::extension(path.toStdString());
    if (ext.empty()) path.append(tpl ? ".tpl" : ".gcode");

    else if (tpl && ext != "tpl") {
      win->warning(tr("TPL file must have .tpl extension"));
      return;

    } else if (!tpl && (ext == "xml" || ext == "tpl")) {
      win->warning(tr("GCode file cannot have .tpl or .xml extension"));
      return;
    }
  }

  // Save data
  QString content = editor->toPlainText();
  QFile qFile(path);
  if (!qFile.open(QFile::ReadOnly | QIODevice::ReadOnly))
    THROW("Could not save '" << path.toStdString() << "'");
  // qFile.write(content.toUtf8());
  qFile.close();

  // Update file path
  string _path = path.toStdString();
  _path = SystemUtilities::absolute(_path);
  if (_path != file.getPath()) {
    file.setPath(_path);

    // Update tab title
    QString title(QString::fromUtf8(file.getBasename().c_str()));
    QTabWidget::setTabText(tab, title);
  }

  // Set unmodified未修改设置
  editor->document()->setModified(false);

  // Notify通知
  win->showMessage(tr("Saved %1").arg(file.getBasename().c_str()));
}


void coordtabmanager::saveAll() {
  for (int tab = offset; tab < QTabWidget::count(); tab++) save(tab);
}


void coordtabmanager::revert() {
  revert(currentIndex());
}


void coordtabmanager::revert(unsigned tab) {
  // Check if modified
  if (!isModified(tab)) return;

  // Get file
  NCEdit *editor = (NCEdit *)QTabWidget::widget(tab);
  Project::File &file = *editor->getFile();

  if (!file.exists()) return;

  // Read data
  QFile qFile(file.getPath().c_str());
  qFile.open(QFile::ReadOnly);
  QString contents = qFile.readAll();
  qFile.close();
  contents.replace('\t', " ");

  // Revert
  editor->setPlainText(contents);

  // Set unmodified
  editor->document()->setModified(false);

  // Notify
  win->showMessage(tr("Reverted %1").arg(file.getBasename().c_str()));
}


void coordtabmanager::revertAll() {
  for (int tab = offset; tab < QTabWidget::count(); tab++) revert(tab);
}


void coordtabmanager::close(unsigned tab, bool canSave, bool removeTab) {
  if (canSave && !checkSave(tab)) return;
  delete (NCEdit *)QTabWidget::widget(tab);
  if (removeTab) QTabWidget::removeTab(tab);
}


void coordtabmanager::closeAll(bool canSave, bool removeTab) {
  for (int tab = offset; tab < QTabWidget::count(); tab++)
    close(tab, canSave, removeTab);
}


NCEdit *coordtabmanager::getEditor(unsigned tab) const {
  validateTabIndex(tab);
  return (NCEdit *)QTabWidget::widget(tab);
}


NCEdit *coordtabmanager::getCurrentEditor() const {
  return getEditor(QTabWidget::currentIndex());
}


int coordtabmanager::getEditorIndex(NCEdit *editor) const {
  for (int i = offset; i < QTabWidget::count(); i++)
    if (QTabWidget::widget(i) == editor) return i;

  return -1;
}


void coordtabmanager::on_modificationChanged(NCEdit *editor, bool changed) {
  int tab = getEditorIndex(editor);
  if (tab == -1) return;
  QString title = QTabWidget::tabText(tab);

  if (changed && !title.endsWith(" *"))
    QTabWidget::setTabText(tab, title + tr(" *"));

  else if (!changed && title.endsWith(" *"))
    QTabWidget::setTabText(tab, title.left(title.size() - 2));

  win->updateActions();
}


void coordtabmanager::on_tabCloseRequested(int index) {
  close(index, true, false);
}


void coordtabmanager::on_actionUndo_triggered() {getCurrentEditor()->undo();}
void coordtabmanager::on_actionRedo_triggered() {getCurrentEditor()->redo();}
void coordtabmanager::on_actionCut_triggered() {getCurrentEditor()->cut();}
void coordtabmanager::on_actionCopy_triggered() {getCurrentEditor()->copy();}
void coordtabmanager::on_actionPaste_triggered() {getCurrentEditor()->paste();}


void coordtabmanager::on_actionSelectAll_triggered() {
  getCurrentEditor()->selectAll();
}


void coordtabmanager::on_find(QString find, bool regex, int options) {
  getCurrentEditor()->find(find, regex, options);
}


void coordtabmanager::on_replace(QString find, QString replace, bool regex,
                                int options, bool all) {
  getCurrentEditor()->replace(find, replace, regex, options, all);
}
>>>>>>> xutian push

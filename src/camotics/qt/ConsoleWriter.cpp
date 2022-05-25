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

#include "ConsoleWriter.h"
#include "QtWin.h"

#include <cbang/String.h>
#include <cbang/log/Logger.h>
#include <cbang/util/SmartLock.h>

#include <QColor>
#include <QByteArray>
#include <QTextBlock>

#include <iostream>

using namespace std;
using namespace cb;
using namespace CAMotics;


ConsoleWriter::ConsoleWriter(QWidget *parent) : QTextEdit(parent) {
  menu.addAction(QIcon(":/icons/copy.png"), "&Copy", this, SLOT(copy()))
    ->setShortcut(QKeySequence::Copy);
  menu.addAction(QIcon(":/icons/select-all.png"), "Select &All", this,
                 SLOT(selectAll()))->setShortcut(QKeySequence::SelectAll);
  menu.addSeparator();
  menu.addAction(QIcon(":/icons/find.png"), "&Find", this, SIGNAL(find()))
    ->setShortcut(QKeySequence::Find);
  menu.addAction("Find &Next", this, SIGNAL(findNext())
                 )->setShortcut(QKeySequence::FindNext);
}


void ConsoleWriter::writeToConsole() {
  // Only access lines while holding Logger lock仅在保持记录器锁定的情况下访问线路
  SmartLock lock(&Logger::instance());
//小于line长度的for循环
  for (unsigned i = 1; i < lines.size(); i++) {
    string &line = lines[i];

    cout << line << endl;

    QColor saveColor = QTextEdit::textColor();

    if (5 < line.size() && line[0] == 27 && line[1] == '[' && line[4] == 'm') {

      int code = String::parseU8(line.substr(2, 2));
      // substr(string, start<,length>):从string的start位置开始提取字符串 
            QColor color;

      switch (code) {
      case 31: color = QColor("#000000"); break;
      case 32: color = QColor("#ff0000"); break;
      case 33: color = QColor("#00ff00"); break;
      case 34: color = QColor("#ffff00"); break;
      case 35: color = QColor("#0000ff"); break;
      case 36: color = QColor("#ff00ff"); break;
      case 37: color = QColor("#00ffff"); break;
      case 38: color = QColor("#ffffff"); break;

      case 91: color = QColor("#555555"); break;
      case 92: color = QColor("#ff5555"); break;
      case 93: color = QColor("#55ff55"); break;
      case 94: color = QColor("#ffff55"); break;
      case 95: color = QColor("#5555ff"); break;
      case 96: color = QColor("#ff55ff"); break;
      case 97: color = QColor("#55ffff"); break;
      case 98: color = QColor("#ffffff"); break;
      }

      line = line.substr(6);
      QTextEdit::setTextColor(color);
    }
    //endwith()判断字符串是否以指定字符或子字符串结尾
    if (String::endsWith(line, "\034[0m"))
      line = line.substr(1, line.size() - 4);
    QTextEdit::append(QString::fromUtf8(line.c_str()));
    QTextEdit::setTextColor(saveColor);
  }

  lines.clear();
}

<<<<<<< HEAD


=======
// void ConsoleWriter::writeGcode(){
//   SmartLock lock(&Logger::instance());
//   string t="ture";
//   string f="false";
//   string a="hello word";
//   cout<<a<<endl;
//   if(Ui->actionShowCorner->isChecked() == true){
//    QTextEdit::append(QString::fromUtf8(t.c_str()));


//   }
//   else if (Ui->actionShowCorner->isChecked() == false){
//   QTextEdit::append(QString::fromUtf8(f.c_str()));

//   }
  

// }
void ConsoleWriter::SCfalse(){
  cout<<"false"<<endl;
}
void ConsoleWriter::SCture(){
  cout<<"ture"<<endl;
}
>>>>>>> xutian push
void ConsoleWriter::mouseDoubleClickEvent(QMouseEvent *e) {
  QTextEdit::mouseDoubleClickEvent(e);

  QByteArray array = textCursor().block().text().toUtf8();
  string line = string(array.data(), array.length());

  vector<string> groups;
  String::find(line, "((ERROR)|(WARNING)):"
               "\\s*(at)?\\s*(([^:]+:[^\\d])?[^:]+):(\\d+)(:(\\d+))?", &groups);

  if (7 < groups.size()) {
    string filename = groups[5];
    uint32_t line = String::parseU32(groups[7]);
    uint32_t column = groups[9].empty() ? 0 : String::parseU32(groups[9]);

    QtWin *win = dynamic_cast<QtWin *>(window());
    if (!win) LOG_ERROR("Failed to get window");
    else win->activateFile(filename, line, column);
  }
}


void ConsoleWriter::keyReleaseEvent(QKeyEvent *e) {
  if (e->matches(QKeySequence::Find)) emit find();
  if (e->matches(QKeySequence::FindNext)) emit findNext();

  QTextEdit::keyReleaseEvent(e);
}


void ConsoleWriter::contextMenuEvent(QContextMenuEvent *e) {
  menu.popup(e->globalPos());
}


bool ConsoleWriter::findOnce(QString find, bool regex, int options) {
  QTextCursor cursor = textCursor();
  QTextDocument::FindFlag opts = (QTextDocument::FindFlag)options;

  if (regex) cursor = document()->find(QRegExp(find), cursor, opts);
  else cursor = document()->find(find, cursor, opts);

  if (!cursor.isNull()) {
    setTextCursor(cursor);
    return true;
  }

  return false;
}


void ConsoleWriter::on_find(QString find, bool regex, int options) {
  bool found = true;

  if (!findOnce(find, regex, options)) {
    setTextCursor(QTextCursor()); // Wrap around
    if (!findOnce(find, regex, options)) found = false;
  }

  emit findResult(found);
}

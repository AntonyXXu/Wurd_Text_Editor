#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>

TextEditor* createTextEditor(Undo* un)
{
  return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
  : TextEditor(undo) {
  init();
}

StudentTextEditor::~StudentTextEditor()
{

}

bool StudentTextEditor::load(std::string file) {
  return false;  // TODO
}

bool StudentTextEditor::save(std::string file) {
  return false;  // TODO
}

void StudentTextEditor::reset() {
  m_lines.clear();
  init();
}

void StudentTextEditor::move(Dir dir) {
  switch (dir)
  {
  case UP:
  {
    break;
  }
  case DOWN:
  {
    break;
  }
  case LEFT:
  {
    break;
  }
  case RIGHT:
  {
    break;
  }
  case HOME:
  {
    break;
  }
  case END:
  {
    break;
  }
  }
}

void StudentTextEditor::del() {
  // TODO
}

void StudentTextEditor::backspace() {
  // TODO
}

void StudentTextEditor::insert(char ch) {
  // TODO
}

void StudentTextEditor::enter() {
  // TODO
}

void StudentTextEditor::getPos(int& row, int& col) const {
  row = 0; col = 0; // TODO
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
  return 0; // TODO
}

void StudentTextEditor::undo() {
  // TODO
}

void StudentTextEditor::init() {
  m_row = 0;
  m_col = 0;
  m_currentRow = m_lines.begin();
}
#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
using namespace std;


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
    //Currently at line 0
    if (!m_row)
    {
      return;
    }
    m_linesItr--;
    m_row--;
    //If previous line is shorter, new column is the last member of the previous line
    if (m_col > m_linesItr->length())
    {
      m_col = m_linesItr->length();
    }
    break;
  }
  case DOWN:
  {
    //Currently at the maximum line
    if (m_row >= m_lines.size() - 1)
    {
      return;
    }
    m_linesItr++;
    m_row++;
    //If previous line is shorter, new column is the last member of the previous line
    if (m_col > m_linesItr->length())
    {
      m_col = m_linesItr->length();
    }
    break;
  }
  case LEFT:
  {
    //Currently at first column
    if (!m_col)
    {
      if (!m_row)
      {
        return;
      }
      //Go to previous line
      m_linesItr--;
      m_row--;
      m_col = m_linesItr->length();
    }
    else {
      m_col--;
    }
    break;
  }
  case RIGHT:
  {
    //Currently at last column
    if (m_col == m_linesItr->length())
    {
      if (m_row >= m_lines.size() - 1)
      {
        return;
      }
      //Go to next line
      m_row++;
      m_linesItr++;
      m_col = 0;
    }
    else
    {
      m_col++;
    }
    break;
  }
  case HOME:
  {
    m_col = 0;
    break;
  }
  case END:
  {
    m_col = m_linesItr->size();
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
  if (ch == '\t')
  {

    m_col += 4;
  }

  m_col++;
}

void StudentTextEditor::enter() {
  //Splice the current string at column, and insert new line to the next line list
  string rightOfEnter = m_linesItr->substr(m_col, m_linesItr->length() - m_col);
  //Replace the current line with left side of text
  *m_linesItr = m_linesItr->substr(0, m_col);

  m_lines.insert(m_linesItr, rightOfEnter);
  m_row++;
  m_col = 0;

}

void StudentTextEditor::getPos(int& row, int& col) const {
  row = m_row;
  col = m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
  //if (startRow < 0 || numRows < 0 || startRow > m_lines.size())
  //{
  //  return -1;
  //}
  //lines.clear();
  //list<string>::const_iterator itr = m_lines.begin();
  //for (int i = 0; i < startRow; i++)
  //{
  //  itr++;
  //}
  //while (itr != m_lines.end() && numRows > 0)
  //{
  //  lines.push_back(*itr);
  //  itr++;
  //  numRows--;
  //}
  //return lines.size();
  return 0;
}

void StudentTextEditor::undo() {
  // TODO
}

void StudentTextEditor::init() {
  m_row = 0;
  m_col = 0;
  m_lines.push_back("");
  m_linesItr = m_lines.begin();
}
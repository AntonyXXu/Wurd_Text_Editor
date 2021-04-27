#include "StudentTextEditor.h"
#include "Undo.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;


TextEditor* createTextEditor(Undo* un) {
  return new StudentTextEditor(un);
}

StudentTextEditor::StudentTextEditor(Undo* undo)
  : TextEditor(undo) {
  init();
}

StudentTextEditor::~StudentTextEditor() {
  m_lines.clear();
}

bool StudentTextEditor::load(std::string file) {
  ifstream infile(file);
  if (!infile) {
    return false;
  }
  reset();
  string textLine;
  // Iterate through each line and populate the text editor based on file text
  while (getline(infile, textLine)) {
    textLine.erase(remove(textLine.begin(), textLine.end(), '\r'), textLine.end());
    m_lines.push_back(textLine);
  }

  // Remove the first line initialized (except if file was empty)
  if (!m_lines.empty()) {
    m_lines.pop_front();
    m_linesItr = m_lines.begin();
  }
  return true;
}

bool StudentTextEditor::save(std::string file) {
  ofstream outfile(file);
  if (!outfile) {
    return false;
  }
  // Iterate through the text editor and save each line to the saved file. 
  list<string>::iterator itr = m_lines.begin();
  while (itr != m_lines.end()) {
    outfile << *itr << endl;
    itr++;
  }
  return true;  // TODO
}

void StudentTextEditor::reset() {
  m_lines.clear();
  init();
}

void StudentTextEditor::move(Dir dir) {
  switch (dir)
  {
  case UP:
    // Currently at line 0
    if (!m_row) {
      m_col = 0;
      return;
    }
    m_linesItr--;
    m_row--;

    // If previous line is shorter, new column is the last member of the previous line
    if (m_col > m_linesItr->length())
    {
      m_col = m_linesItr->length();
    }
    break;

  case DOWN:
    // Currently at the maximum line
    if (m_row == m_lines.size() - 1) {
      m_col = m_linesItr->length();
      return;
    }
    m_linesItr++;
    m_row++;

    // If next line is shorter, new column is the last member of the previous line
    if (m_col > m_linesItr->length()) {
      m_col = m_linesItr->length();
    }
    break;

  case LEFT:
    // Currently at first column
    if (!m_col) {
      if (!m_row) {
        return;
      }
      // Go to previous line
      m_linesItr--;
      m_row--;
      m_col = m_linesItr->length();
    }
    else
    {
      m_col--;
    }
    break;

  case RIGHT:
    // Currently at last column
    if (m_col == m_linesItr->length()) {
      if (m_row == m_lines.size() - 1) {
        return;
      }
      // Go to next line
      m_row++;
      m_linesItr++;
      m_col = 0;
    }
    else {
      m_col++;
    }
    break;

  case HOME:
    m_col = 0;
    break;

  case END:
    m_col = m_linesItr->size();
    break;
  }
}

void StudentTextEditor::del() {
  // Delete the next character. If at the end of the line, 
  // join the next line with the current one. 
  // Then, send the new data to the undo stack
  if (m_col == m_linesItr->length()) {
    if (m_row == m_lines.size() - 1) {
      return;
    }
    m_linesItr++;
    m_row++;
    deleteLineHelper();
    getUndo()->submit(Undo::JOIN, m_row, m_col);
  }
  else {
    char ch = (*m_linesItr)[m_col];
    m_linesItr->erase(m_col, 1);
    getUndo()->submit(Undo::DELETE, m_row, m_col, ch);
  }
}

void StudentTextEditor::backspace() {
  // Delete the previous character. If at column 0, 
  // join the current line with the previous line. 
  // Then, send the new data to the undo stack
  if (!m_col) {
    if (!m_row) {
      return;
    }
    deleteLineHelper();
    getUndo()->submit(Undo::JOIN, m_row, m_col);
  }
  else {
    m_col -= 1;
    char ch = (*m_linesItr)[m_col];
    m_linesItr->erase(m_col, 1);
    getUndo()->submit(Undo::DELETE, m_row, m_col, ch);
  }
}

void StudentTextEditor::insert(char ch) {
  // Insert new data, and send to undo stack
  if (ch == '\t')
  {
    for (int i = 0; i < 4; i++)
    {
      m_linesItr->insert(m_col, 1, ' ');
      m_col++;
      getUndo()->submit(Undo::INSERT, m_row, m_col, ' ');
    }
  }
  else
  {
    m_linesItr->insert(m_col, 1, ch);
    m_col++;
    getUndo()->submit(Undo::INSERT, m_row, m_col, ch);
  }
}

void StudentTextEditor::enter() {
  // Split the line
  getUndo()->submit(Undo::SPLIT, m_row, m_col);
  splitLineHelper();
}

void StudentTextEditor::getPos(int& row, int& col) const {
  row = m_row;
  col = m_col;
}

int StudentTextEditor::getLines(int startRow, int numRows, std::vector<std::string>& lines) const {
  if (startRow < 0 || numRows < 0 || startRow > m_lines.size()) {
    return -1;
  }
  lines.clear();
  list<string>::iterator itr = m_linesItr;
  // Move iterator to startRow and push to lines vector
  moveIteratorHelper(m_row, startRow, itr);
  while (itr != m_lines.end() && numRows > 0) {
    lines.push_back(*itr);
    itr++;
    numRows--;
  }
  return lines.size();
}

void StudentTextEditor::undo() {
  int row, col, count;
  string text;
  Undo::Action action = getUndo()->get(row, col, count, text);
  if (action == Undo::ERROR) {
    return;
  }

  // Move the editing location to the undo location
  moveIteratorHelper(m_row, row, m_linesItr);
  m_row = row;
  m_col = col;
  string leftOfEnter;
  string rightOfEnter;

  // Perform required action for undo
  switch (action)
  {
  case Undo::INSERT:
    m_linesItr->insert(m_col, text);
    break;

  case Undo::DELETE:
    m_col = m_col - count;
    m_linesItr->erase(m_col, count);
    break;

  case Undo::SPLIT:
    splitLineHelper();
    break;

  case Undo::JOIN:
    m_linesItr++;
    m_row++;
    m_col = 0;
    deleteLineHelper();
    break;
  }
}

void StudentTextEditor::init() {
  m_row = 0;
  m_col = 0;
  m_lines.push_back("");
  m_linesItr = m_lines.begin();
}

void StudentTextEditor::deleteLineHelper() {
  // Save and delete the current line
  string currentLine = (*m_linesItr);
  m_linesItr = m_lines.erase(m_linesItr);
  m_linesItr--;
  m_row--;
  m_col = m_linesItr->length();
  // Join the saved line with the previous line
  m_linesItr->insert(m_linesItr->length(), currentLine);
}

void StudentTextEditor::splitLineHelper() {
  // Splice the current line at the cursor location
  string leftOfEnter = m_linesItr->substr(0, m_col);
  string rightOfEnter = m_linesItr->substr(m_col, m_linesItr->length() - m_col);
  // Replace the current line with left side of text
  m_lines.insert(m_linesItr, leftOfEnter);
  *m_linesItr = rightOfEnter;
  m_row++;
  m_col = 0;
}

void StudentTextEditor::moveIteratorHelper(int currentRow, int desiredRow, std::list<std::string>::iterator& iterator) const
{
  if (currentRow >= desiredRow) {
    for (int i = currentRow; i > desiredRow; i--) {
      iterator--;
    }
  }
  else {
    for (int i = currentRow; i < desiredRow; i++) {
      iterator++;
    }
  }
}
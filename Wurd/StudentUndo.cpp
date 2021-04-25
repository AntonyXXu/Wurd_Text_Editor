#include "StudentUndo.h"

Undo* createUndo()
{
  return new StudentUndo;
}

StudentUndo::~StudentUndo() {
  clear();
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {

  if (!m_undoStack.empty())
  {
    UndoData previous = m_undoStack.top();
    if (action == previous.action && row == previous.row)
    {
      if (action == INSERT && col == previous.col + 1)
      {
        //Insertion undo linking
        previous.text += ch;
        previous.row = row;
        previous.col = col;
        return;
      }
      if (action == DELETE)
      {

        if (col == previous.col - 1)
        {
          //Backspace undo linking
          previous.text = ch + previous.text;
          previous.col = col;

          return;
        }
        else if (col == previous.col)
        {
          //Delete undo linking
          previous.text += ch;
          return;
        }
      }
    }
  }
  UndoData newUndo;
  newUndo.action = action;
  newUndo.row = row;
  newUndo.col = col;
  newUndo.text = ch;
  m_undoStack.push(newUndo);
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
  if (m_undoStack.empty())
  {
    return ERROR;
  }
  UndoData undo = m_undoStack.top();
  row = undo.row;
  col = undo.col;
  Action action = ERROR;
  switch (undo.action)
  {
  case INSERT:
    action = DELETE;
    count = undo.text.length();
    break;
  case SPLIT:
    action = JOIN;
    break;
  case DELETE:
    action = INSERT;
    text = undo.text;
    break;
  case JOIN:
    action = SPLIT;
    break;
  }
  m_undoStack.pop();
  return action;
}

void StudentUndo::clear() {
  while (!m_undoStack.empty())
  {
    m_undoStack.pop();
  }
}

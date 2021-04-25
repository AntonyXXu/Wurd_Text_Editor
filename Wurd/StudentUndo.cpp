#include "StudentUndo.h"

Undo* createUndo()
{
  return new StudentUndo;
}

StudentUndo::~StudentUndo() {
  clear();
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
  UndoData newUndo;
  newUndo.action = action;
  newUndo.row = row;
  newUndo.col = col;
  newUndo.text = ch;
  if (!m_undoStack.empty())
  {
    //Replace the previous undo with the new undo for batching
    //Batching does not occur if the previously inserted char is space
    UndoData previous = m_undoStack.top();
    if (action == previous.action
      && row == previous.row)
    {
      if (action == INSERT
        && col == previous.col + 1
        && previous.text[previous.text.size() - 1] != ' '
        || ch == ' ')
      {
        //Insertion undo linking
        //Only triggers if the text is the same word
        //and if the inserted text is a tab/space
        newUndo.text = previous.text + ch;
        m_undoStack.pop();
      }
      else if (action == DELETE)
      {
        if (col == previous.col - 1)
        {
          //Backspace deletion undo linking
          newUndo.text += previous.text;
          m_undoStack.pop();
        }
        else if (col == previous.col)
        {
          //Delete deletion undo linking
          newUndo.text = previous.text + ch;
          m_undoStack.pop();
        }
      }
    }
  }

  m_undoStack.push(newUndo);
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
  if (m_undoStack.empty()) {
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
  while (!m_undoStack.empty()) {
    m_undoStack.pop();
  }
}

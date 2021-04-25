#ifndef STUDENTUNDO_H_
#define STUDENTUNDO_H_

#include<stack>
#include "Undo.h"

class StudentUndo : public Undo {
public:
  virtual ~StudentUndo();
  void submit(Action action, int row, int col, char ch = 0);
  Action get(int& row, int& col, int& count, std::string& text);
  void clear();

private:
  struct UndoData
  {
    Action action;
    std::string text;
    int row;
    int col;
  };
  std::stack<UndoData> m_undoStack;
};

#endif // STUDENTUNDO_H_

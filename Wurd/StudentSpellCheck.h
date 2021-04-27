#ifndef STUDENTSPELLCHECK_H_
#define STUDENTSPELLCHECK_H_

#include "SpellCheck.h"

#include <string>
#include <vector>

class StudentSpellCheck : public SpellCheck {
public:
  StudentSpellCheck();
  virtual ~StudentSpellCheck();
  bool load(std::string dict_file);
  bool spellCheck(std::string word, int maxSuggestions, std::vector<std::string>& suggestions);
  void spellCheckLine(const std::string& line, std::vector<Position>& problems);
  static const int CHARS = 27;

private:
  struct TrieNode {
    TrieNode()
    {
      for (int i = 0; i < CHARS; i++)
      {
        childrenNodes.push_back(nullptr);
      }
      leaf = false;
    }
    std::vector<TrieNode*> childrenNodes;
    bool leaf;
  };
  TrieNode* m_trieRoot;
  void insert(std::string word);
  void clear(TrieNode* node);
  bool search(std::string word) const;
  int charIndex(char ch) const;
};

#endif  // STUDENTSPELLCHECK_H_

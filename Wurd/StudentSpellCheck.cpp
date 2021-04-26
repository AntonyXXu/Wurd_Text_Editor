#include "StudentSpellCheck.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

SpellCheck* createSpellCheck()
{
  return new StudentSpellCheck;
}

StudentSpellCheck::StudentSpellCheck() {
  m_trieRoot = nullptr;
}

StudentSpellCheck::~StudentSpellCheck() {
  clear(m_trieRoot);

}

bool StudentSpellCheck::load(std::string dictionaryFile) {
  ifstream infile(dictionaryFile);
  if (!infile) {
    return false;
  }
  //Reset previous dictionary and load the new one in. Trim \r characters
  clear(m_trieRoot);
  m_trieRoot = new TrieNode();
  string word;
  while (getline(infile, word)) {
    word.erase(remove(word.begin(), word.end(), '\r'), word.end());
    insert(word);
  }
  return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
  if (search(word)) {
    return true;
  }
  //Check for suggestions
  //Suggestions must only be one letter different and same length
  bool difference = false;
  for (int i = 0; i < word.size(); i++) {

  }

  return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
  // TODO
}

void StudentSpellCheck::insert(string word) {
  if (word.size() == 0) {
    return;
  }
  TrieNode* currNode = m_trieRoot;
  for (int i = 0; i < word.size(); i++) {
    int index = charIndex(tolower(word[i]));
    if (!currNode->childrenNodes[index]) {
      currNode->childrenNodes[index] = new TrieNode();
    }
    currNode = currNode->childrenNodes[index];
  }
  currNode->leaf = true;

}

void StudentSpellCheck::clear(TrieNode* node) {
  if (node == nullptr) {
    return;
  }
  for (int i = 0; i < CHARS; i++) {
    clear(node->childrenNodes[i]);
  }
  delete node;
  node = nullptr;
}

bool StudentSpellCheck::search(string word) const {
  TrieNode* currNode = m_trieRoot;
  for (int i = 0; i < word.size(); i++) {
    if (!currNode) {
      return false;
    }
    int index = charIndex(tolower(word[i]));
    currNode = currNode->childrenNodes[index];
  }
  return currNode->leaf;

}

int StudentSpellCheck::charIndex(char ch) const {
  int index = ch - 'a';
  //Default index = 26 for special characters
  if (index > 25 || index < 0) {
    index = 26;
  }
  return index;

}
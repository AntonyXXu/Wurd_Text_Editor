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
    cout << word << endl;
    insert(word);
  }
  return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {
  return false; // TODO
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

bool StudentSpellCheck::search(string text) const {
  return false;

}

int StudentSpellCheck::charIndex(char ch) {
  int a = ch - 'a';
  return ch - 'a';

}
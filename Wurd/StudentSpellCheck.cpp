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
  // Reset previous dictionary and load the new one in. Trim \r characters
  clear(m_trieRoot);
  m_trieRoot = new TrieNode();
  string word;
  // Insert each word of the dictionary into the Trie
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
  suggestions.clear();
  // For each letter index, change the index to a different letter
  for (int i = 0; i < word.size(); i++) {
    string replWord = word;
    for (int j = 0; j < CHARS; j++) {
      // If it's the same letter, continue.
      if (word[i] == 'a' + j) {
        continue;
      }

      // Replace the index of the word and search it
      replWord[i] = 'a' + j;
      if (search(replWord)) {
        suggestions.push_back(replWord);
        if (suggestions.size() >= max_suggestions) {
          break;
        }
      }
    }
  }
  return false;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
  problems.clear();
  int firstPos, lastPos = 0;
  string word;
  bool wordInit = false;

  for (lastPos; lastPos < line.size(); lastPos++) {

    char ch = line[lastPos];
    // Checks if the first word is initialized. If it is not, and the current position is a letter or apostrophe, initialize the word and position
    if (!wordInit) {
      if (ch == '\'' || charIndex(tolower(ch)) < 26) {
        wordInit = true;
        firstPos = lastPos;
      }
      continue;
    }

    // Word is initialized, check if current index is an ending to the word
    if (charIndex(ch) >= 26 && ch != '\'') {
      word = line.substr(firstPos, lastPos - firstPos);

      // If the word is misspelled, push it to problems vector
      if (!search(word)) {
        Position pos;
        pos.start = firstPos;
        pos.end = lastPos - 1;
        problems.push_back(pos);
      }
      // Reset the word initialization since a new word has started
      wordInit = false;
    }
  }

}

void StudentSpellCheck::insert(string word) {
  if (word.size() == 0) {
    return;
  }
  TrieNode* currNode = m_trieRoot;
  for (int i = 0; i < word.size(); i++) {
    // Iterate through each letter of the word
    int index = charIndex(tolower(word[i]));

    // If the node is empty, create a new TrieNode
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
  // Effectively pre-order traverse and delete all nodes
  for (int i = 0; i < CHARS; i++) {
    clear(node->childrenNodes[i]);
  }
  delete node;
  node = nullptr;
}

bool StudentSpellCheck::search(string word) const {
  TrieNode* currNode = m_trieRoot;
  // Iterate through the word and the trie
  for (int i = 0; i < word.size(); i++) {
    if (!currNode) {
      return false;
    }
    int index = charIndex(tolower(word[i]));
    currNode = currNode->childrenNodes[index];
  }
  // Last node must be a leaf
  if (!currNode || !currNode->leaf) {
    return false;
  }
  return true;

}

int StudentSpellCheck::charIndex(char ch) const {
  int index = ch - 'a';
  // Default index = 26 for special characters
  if (index > 25 || index < 0) {
    index = 26;
  }
  // Return an index for checking the TrieNode vector
  return index;
}

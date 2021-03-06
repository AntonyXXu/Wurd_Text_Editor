# Wurd Text Editor in C++
Created logic and data structures behind a simple text editor, capable of spellcheck, undo, loading, and saving. A list of strings keeps track of all the lines of text with an iterator keeping track of the current line. I used a Trie to implement the spell checker, and a stack of undo data to maintain undo functions. Note I did not create the GUI.
Feel free to try it out and download the Wurd.zip file.

I built the StudentTextEditor.cpp/.h, StudentUndo.cpp/.h, and the StudentSpellCheck.cpp/.h files.

This project was created based on UCLA CS-32 Data Structures and Algorithms Project 4 in 2021. Spec sheet is attached.

## Demo:
![wurd_demo](https://user-images.githubusercontent.com/77988513/116180397-11fdf400-a6d6-11eb-8236-d3918f869903.gif)


## Details
#### Text Editor
- The entire text document is maintainted with a list of strings. An iterator keeps track of the current editing line. 
- Main actions are moving within the editor, insertion, deletion, enter, backspace functions which satisfy the big O requirements from the specifications.
- The text editor can save and load any specified text file. It can even edit the code I used to create the text editor!
#### Undo Capabilities
- Upon insertion, deletion, or changing of the text, the changed text is sent to an Undo Class, which pushes the data on top of a stack. The class also checks if the undo data can be 'batched' together (such as undoing a "word" instead of undoing 'w', 'o', 'r', 'd' through four undo actions).
- When the user calls undo through Control+Z, the data is popped and sent to the above text editor class, which reverts the insertion, change, split, or join action.
#### Spell Check Functionality (in progress)
- Spell Check created with a Trie/prefix tree.
- The spell checker loads a dictionary to initialize the trie for spell checking text editor words
- Spelling suggestions was took me a while to think of.
  - If the word is not within the dictionary, the algorithm goes through each individual index of the string {O(word size)}
    - Then, it replaces the given index with a new character, and searches the new word within the dictionary {O(word size)}. If the new word is found, it's pushed to the suggestion array. 
- Spell Check Display (Highlights all of the mis-spelled words) 
  - This function iterates through the editor line, and returns a vector of words that are misspelled. Primarly used a simple search algorithm to find all the incorrectly spelled characters on the line.

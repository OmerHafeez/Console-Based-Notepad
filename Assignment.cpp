#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>
using namespace std;

class Node {
public:
    char data;
    Node* right;
    Node* down;
    Node* up;
    Node* left;

    Node(char data) {
        this->data = data;
        this->right = nullptr;
        this->down = nullptr;
        this->up = nullptr;
        this->left = nullptr;
    }
};

class TwoDLinkedList {
public:
    Node* head;
    Node* cursor;

    TwoDLinkedList() {
        head = nullptr;
        cursor = nullptr;
    }

    void insertChar(char data) {
        if (!head) {
            head = new Node(data);
            cursor = head;
        }
        else if (cursor) {
            Node* newNode = new Node(data);

            if (cursor->right) {
                newNode->right = cursor->right;
                cursor->right->left = newNode;
            }
            else {
                cursor->right = newNode;
                newNode->left = cursor;
            }

            cursor->right = newNode;
            cursor = newNode;
        }
    }

    void insertNewline() {
        if (cursor) {
            Node* newline = new Node('\n');

            if (cursor->right) {
                newline->right = cursor->right;
                cursor->right->left = newline;
            }
            cursor->right = newline;
            newline->left = cursor;

            Node* currentLine = cursor;
            while (currentLine->up) {
                currentLine = currentLine->up;
            }
            currentLine->up = newline;
            newline->down = currentLine;

            cursor = newline;
        }
    }

    void deleteBackspace() {
        if (cursor && cursor->left) {
            Node* prev = cursor->left;

            if (prev == head) {
                if (prev->down) {
                    cursor = prev->down;
                    while (cursor->right) {
                        cursor = cursor->right;
                    }
                    cursor->right = nullptr;
                    delete prev;
                }
                else {
                    return;
                }
            }
            else {
                if (prev->left) {
                    cursor->left = prev->left;
                    prev->left->right = cursor;
                }
                else {
                    cursor->left = nullptr;
                    head = cursor;
                }
                delete prev;
            }
        }
    }

    void moveCursorRight() {
        if (cursor && cursor->right) {
            cursor = cursor->right;
        }
    }

    void moveCursorLeft() {
        if (cursor && cursor->left) {
            cursor = cursor->left;
        }
    }

    void moveCursorUp() {
        if (cursor && cursor->up) {
            cursor = cursor->up;
            while (cursor->right) {
                cursor = cursor->right;
            }
        }
    }

    void moveCursorDown() {
        if (cursor && cursor->down) {
            cursor = cursor->down;
            while (cursor->right) {
                cursor = cursor->right;
            }
        }
    }

    void display() {
        system("cls");
        Node* currentRow = head;
        while (currentRow) {
            Node* currentNode = currentRow;
            while (currentNode) {
                if (currentNode == cursor) {
                    cout << '!';
                }
                cout << currentNode->data;
                currentNode = currentNode->right;
            }
            cout << endl;
            currentRow = currentRow->down;
        }
    }

    void saveToFile(const char* filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file." << endl;
            return;
        }

        Node* currentRow = head;
        while (currentRow) {
            Node* currentNode = currentRow;
            while (currentNode) {
                file << currentNode->data;
                currentNode = currentNode->right;
            }
            file << endl;
            currentRow = currentRow->down;
        }

        file.close();
    }

    void loadFromFile(const char* filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file." << endl;
            return;
        }

        string line;
        Node* lastNode = nullptr;
        Node* currentLine = nullptr;

        while (getline(file, line)) {
            Node* rowCursor = nullptr;

            for (char c : line) {
                Node* newNode = new Node(c);
                if (!head) {
                    head = newNode;
                    rowCursor = head;
                }
                else {
                    rowCursor->right = newNode;
                    newNode->left = rowCursor;
                    rowCursor = newNode;
                }
                if (!lastNode) {
                    cursor = newNode;
                }
                lastNode = newNode;
            }

            Node* newline = new Node('\n');
            if (currentLine) {
                currentLine->down = newline;
                newline->up = currentLine;
            }
            currentLine = newline;

            if (file.peek() != EOF) {
                cursor = newline;
                lastNode = nullptr;
            }
        }

        file.close();
    }

    void clear() {
        Node* currentRow = head;
        while (currentRow) {
            Node* currentNode = currentRow;
            while (currentNode) {
                Node* temp = currentNode;
                currentNode = currentNode->right;
                delete temp;
            }
            Node* tempRow = currentRow;
            currentRow = currentRow->down;
            delete tempRow;
        }
        head = nullptr;
        cursor = nullptr;
    }
};

void drawLineAt75Percent() {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(console, &csbi);
    int screenWidth = csbi.dwSize.X;
    int screenHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    int linePosition = static_cast<int>(screenHeight * 0.75);

    for (int i = 0; i < screenWidth; i++) {
        COORD cursor;
        cursor.X = i;
        cursor.Y = linePosition;
        SetConsoleCursorPosition(console, cursor);
        cout << '-';
    }
}

vector<string> dictionary;

void loadDictionary(const char* dictionaryFile) {
    ifstream file(dictionaryFile);
    if (!file.is_open()) {
        cerr << "Error opening dictionary file." << endl;
        return;
    }

    string word;
    while (file >> word) {
        dictionary.push_back(word);
    }

    file.close();
}

bool isWordInDictionary(const string& word) {
    for (const string& dictWord : dictionary) {
        if (dictWord == word) {
            return true;
        }
    }
    return false;
}

vector<string> letterSubstitution(const string& word) {
    vector<string> suggestions;
    const string alphabet = "abcdefghijklmnopqrstuvwxyz";

    for (size_t i = 0; i < word.length(); i++) {
        for (char newChar : alphabet) {
            string modifiedWord = word;
            modifiedWord[i] = newChar;

            if (isWordInDictionary(modifiedWord)) {
                suggestions.push_back(modifiedWord);
            }
        }
    }

    return suggestions;
}

vector<string> letterOmission(const string& word) {
    vector<string> suggestions;

    for (size_t i = 0; i < word.length(); i++) {
        string modifiedWord = word;
        modifiedWord.erase(i, 1);

        if (isWordInDictionary(modifiedWord)) {
            suggestions.push_back(modifiedWord);
        }
    }

    return suggestions;
}

vector<string> letterInsertion(const string& word) {
    vector<string> suggestions;
    const string alphabet = "abcdefghijklmnopqrstuvwxyz";

    for (size_t i = 0; i <= word.length(); i++) {
        for (char newChar : alphabet) {
            string modifiedWord = word;
            modifiedWord.insert(i, 1, newChar);

            if (isWordInDictionary(modifiedWord)) {
                suggestions.push_back(modifiedWord);
            }
        }
    }

    return suggestions;
}

vector<string> letterReversal(const string& word) {
    vector<string> suggestions;

    for (size_t i = 0; i < word.length() - 1; i++) {
        string modifiedWord = word;
        swap(modifiedWord[i], modifiedWord[i + 1]);

        if (isWordInDictionary(modifiedWord)) {
            suggestions.push_back(modifiedWord);
        }
    }

    return suggestions;
}

int main() {
    TwoDLinkedList notepad;

    string currentWord;

    while (true) {
        notepad.display();

        int input = _getch();

        if (input == 27)
            break;
        else if (input == 32) {
            if (!currentWord.empty()) {
                if (!isWordInDictionary(currentWord)) {
                    cout << "Word not found in the dictionary: " << currentWord << endl;

                    vector<string> suggestions = letterSubstitution(currentWord);
                    suggestions.insert(suggestions.end(), letterOmission(currentWord).begin(), letterOmission(currentWord).end());
                    suggestions.insert(suggestions.end(), letterInsertion(currentWord).begin(), letterInsertion(currentWord).end());
                    suggestions.insert(suggestions.end(), letterReversal(currentWord).begin(), letterReversal(currentWord).end());

                    if (!suggestions.empty()) {
                        cout << "Possible correct spellings:";
                        for (const string& suggestion : suggestions) {
                            cout << " " << suggestion;
                        }
                        cout << endl;
                    }
                }
                currentWord = "";
            }
            notepad.insertChar(' ');
        }
        else if (input == 13 || input == '\n') {
            if (!currentWord.empty()) {
                if (!isWordInDictionary(currentWord)) {
                    cout << "Word not found in the dictionary: " << currentWord << endl;

                    vector<string> suggestions = letterSubstitution(currentWord);
                    suggestions.insert(suggestions.end(), letterOmission(currentWord).begin(), letterOmission(currentWord).end());
                    suggestions.insert(suggestions.end(), letterInsertion(currentWord).begin(), letterInsertion(currentWord).end());
                    suggestions.insert(suggestions.end(), letterReversal(currentWord).begin(), letterReversal(currentWord).end());

                    if (!suggestions.empty()) {
                        cout << "Possible correct spellings:";
                        for (const string& suggestion : suggestions) {
                            cout << " " << suggestion;
                        }
                        cout << endl;
                    }
                }
                currentWord = "";
            }
            notepad.insertNewline();
        }
        else if (input == 8) {
            if (currentWord.length() > 0) {
                currentWord.pop_back();
            }
            notepad.deleteBackspace();
        }
        else if (input == 77)
            notepad.moveCursorRight();
        else if (input == 75)
            notepad.moveCursorLeft();
        else if (input == 72)
            notepad.moveCursorUp();
        else if (input == 80)
            notepad.moveCursorDown();
        else if (input >= 32 && input <= 126) {
            char printableChar = static_cast<char>(input);
            currentWord += printableChar;
            notepad.insertChar(printableChar);
        }
        else if (input == 19)
            notepad.saveToFile("notepad.txt");
        else if (input == 12)
            notepad.loadFromFile("notepad.txt");

        drawLineAt75Percent();
    }

    if (!currentWord.empty()) {
        if (!isWordInDictionary(currentWord)) {
            cout << "Word not found in the dictionary: " << currentWord << endl;

            vector<string> suggestions = letterSubstitution(currentWord);
            suggestions.insert(suggestions.end(), letterOmission(currentWord).begin(), letterOmission(currentWord).end());
            suggestions.insert(suggestions.end(), letterInsertion(currentWord).begin(), letterInsertion(currentWord).end());
            suggestions.insert(suggestions.end(), letterReversal(currentWord).begin(), letterReversal(currentWord).end());

            if (!suggestions.empty()) {
                cout << "Possible correct spellings:";
                for (const string& suggestion : suggestions) {
                    cout << " " << suggestion;
                }
                cout << endl;
            }
        }
    }

    notepad.display();
    notepad.saveToFile("notepad.txt");
    notepad.clear();
    return 0;
}

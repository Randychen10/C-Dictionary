#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <cctype>

using namespace std;

// Struct definition for dictionary entries
struct Entry {
    string key;   // Keyword
    string nva;   // Part of speech (noun, verb, adjective, etc.)
    string value; // Definition or value
};

// Function to trim leading and trailing whitespace from a string
string trim(const string& str) {
    size_t front = str.find_first_not_of(" \t\n\r\f\v");
    size_t back = str.find_last_not_of(" \t\n\r\f\v");

    if (front == string::npos || back == string::npos) {
        return "";
    }

    return str.substr(front, back - front + 1);
}

// Function to convert a string to lowercase
string toLower(const string& str) {
    string result = str;
    transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Function to capitalize the first letter of a word and fully capitalize "CSC" keywords
string capitalize(const string& str) {
    if (str.empty()) return str;
    string result = str;
    if (str.size() >= 3 && toLower(str.substr(0, 3)) == "csc") {
        transform(result.begin(), result.end(), result.begin(), ::toupper);
    } else {
        result[0] = toupper(result[0]);
    }
    return result;
}

// Function to print help message
void printHelpMessage() {
    cout << "     |" << endl;
    cout << "      PARAMETER HOW-TO, please enter:" << endl;
    cout << "      1. A search key -then 2. An optional part of speech -then" << endl;
    cout << "      3. An optional 'distinct' -then 4. An optional 'reverse'" << endl;
    cout << "     |" << endl;
}

// Function to get unique keywords from the file
unordered_set<string> getUniqueKeys(const string& filename) {
    fstream ioFile;
    ioFile.open(filename, ios::in);

    unordered_set<string> uniqueKeys;
    string line;

    while (getline(ioFile, line)) {
        uniqueKeys.insert(trim(line));
    }

    ioFile.close();
    return uniqueKeys;
}

// Function to count definitions in the file
list<string> countDefinitions(const string& filename) {
    fstream ioFile;
    ioFile.open(filename, ios::in);

    list<string> definitions;
    string line;

    while (getline(ioFile, line)) {
        // Trim leading and trailing whitespace
        line = trim(line);

        // Check if the line contains the arrow "-=>>"
        size_t arrowPos = line.find("-=>>");

        if (arrowPos != string::npos) {
            // Split the line into definitions by "|"
            stringstream ss(line);
            string item;
            while (getline(ss, item, '|')) {
                // Check if the item contains the arrow "-=>>"
                size_t arrowPosItem = item.find("-=>>");
                if (arrowPosItem != string::npos) {
                    // Add the item to definitions list
                    definitions.push_back(trim(item.substr(arrowPosItem + 4)));  // +4 to skip "-=>>"
                }
            }
        }
    }

    ioFile.close();
    return definitions;
}

// Function to print introductory message
void intro(const unordered_set<string>& uniqueKeys, const list<string>& definitions) {
    cout << "====== DICTIONARY 340 C++ ======" << endl;
    cout << "------ Keywords: " << uniqueKeys.size() << endl;
    cout << "------ Definitions: " << definitions.size() << endl;
}

// Function to create dictionary from file
multimap<string, pair<string, string>> createDictionary(const string& filename) {
    fstream ioFile;
    ioFile.open(filename, ios::in);

    multimap<string, pair<string, string>> dictionary;
    string line;

    while (getline(ioFile, line)) {
        string keyword, rest;
        stringstream ss(line);
        getline(ss, keyword, '|');
        keyword = capitalize(trim(keyword));
        while (getline(ss, rest, '|')) {
            size_t arrowPos = rest.find("-=>>");
            if (arrowPos != string::npos) {
                string pos = trim(rest.substr(0, arrowPos));
                string def = trim(rest.substr(arrowPos + 4));  // +4 to skip "-=>>"
                dictionary.insert({keyword, {pos, def}});
            }
        }
    }

    ioFile.close();
    return dictionary;
}

// Function to filter entries by part of speech
vector<Entry> filterEntriesByPartOfSpeech(const vector<Entry>& entries, const string& partOfSpeech) {
    vector<Entry> filteredEntries;
    for (const auto& entry : entries) {
        if (entry.nva == partOfSpeech) {
            filteredEntries.push_back(entry);
        }
    }
    return filteredEntries;
}

// Function to reverse the order of entries
vector<Entry> reverseEntries(const vector<Entry>& entries) {
    vector<Entry> reversedEntries = entries;
    reverse(reversedEntries.begin(), reversedEntries.end());
    return reversedEntries;
}

// Function to get distinct entries based on value and part of speech
vector<Entry> distinctEntries(vector<Entry> entries) {
    unordered_set<string> seen;
    vector<Entry> distinctEntries;

    for (auto& entry : entries) {
        // Capitalize the first letter of the value
        if (!entry.value.empty()) {
            entry.value[0] = toupper(entry.value[0]);
        }

        // Construct the key from the trimmed value and part of speech
        string key = trim(entry.value) + "_" + trim(entry.nva);

        if (seen.find(key) == seen.end()) {
            seen.insert(key);
            distinctEntries.push_back(entry);
        }
    }

    return distinctEntries;
}

// Function to apply commands (distinct, reverse) to entries
vector<Entry> applyCommands(vector<Entry> entries, const vector<string>& commands) {
    if (find(commands.begin(), commands.end(), "distinct") != commands.end()) {
        entries = distinctEntries(entries);
    }
    if (find(commands.begin(), commands.end(), "reverse") != commands.end()) {
        entries = reverseEntries(entries);
    }
    return entries;
}

// Function to get ordinal suffix for numbers (e.g., 1st, 2nd, 3rd, ...)
string getOrdinal(int number) {
    int m100 = number % 100;
    int m10 = number % 10;
    if (m10 == 1 && m100 != 11) {
        return to_string(number) + "st";
    } else if (m10 == 2 && m100 != 12) {
        return to_string(number) + "nd";
    } else if (m10 == 3 && m100 != 13) {
        return to_string(number) + "rd";
    } else {
        return to_string(number) + "th";
    }
}

// Function to search dictionary and handle commands
void dictSearch(const multimap<string, pair<string, string>>& dictionary) {
    int searchCount = 1;
    string userInput;

    cout << endl;
    while (true) {
        cout << "Search [" << searchCount << "]: ";
        getline(cin, userInput);
        userInput = toLower(trim(userInput));

        if (userInput == "!q") {
            cout << endl;
            cout << "-----THANK YOU-----";
            break;
        } else if (userInput == "!help" || userInput.empty()) {
            printHelpMessage();
        } else {
            stringstream ss(userInput);
            string keyword, pos, option;
            ss >> keyword;
            keyword = capitalize(trim(keyword));

            vector<Entry> entries;
            auto range = dictionary.equal_range(keyword);
            for (auto it = range.first; it != range.second; ++it) {
                entries.push_back({keyword, it->second.first, it->second.second});
            }

            vector<string> commands;
            vector<string> validPartsOfSpeech = {"noun", "verb", "adjective", "conjunction", "adverb", "interjection", "preposistion", "pronoun"};
            vector<string> validCommands = {"distinct", "reverse"};

            int paramIndex = 2;
            while (ss >> option) {
                string trimmedOption = toLower(trim(option));
                if (find(validPartsOfSpeech.begin(), validPartsOfSpeech.end(), trimmedOption) != validPartsOfSpeech.end()) {
                    if (pos.empty()) {
                        pos = trimmedOption;
                    } else {
                        // Error message for duplicate part of speech
                        cout << "     |" << endl;
                        cout << "      <The entered " << getOrdinal(paramIndex) << " parameter '" << trimmedOption << "' is NOT 'distinct' or 'reverse'.>" << endl;
                        cout << "     |" << endl;
                    }
                } else if (find(validCommands.begin(), validCommands.end(), trimmedOption) != validCommands.end()) {
                    commands.push_back(trimmedOption);
                } else {
                    // Error message for invalid parameter
                    cout << "     |" << endl;
                    cout << "      <The entered " << getOrdinal(paramIndex) << " parameter '" << trimmedOption << "' is NOT a part of speech.>" << endl;
                    cout << "      <The entered " << getOrdinal(paramIndex) << " parameter '" << trimmedOption << "' is NOT 'distinct'.>" << endl;
                    cout << "      <The entered " << getOrdinal(paramIndex) << " parameter '" << trimmedOption << "' is NOT 'reverse'.>" << endl;
                    cout << "      <The entered " << getOrdinal(paramIndex) << " parameter '" << trimmedOption << "' was disregarded.>" << endl;
                    cout << "      <The " << getOrdinal(paramIndex) << " parameter should be a part of speech or 'distinct' or 'reverse'.>" << endl;
                    cout << "     |" << endl;
                }
                paramIndex++;
            }

            if (!pos.empty()) {
                entries = filterEntriesByPartOfSpeech(entries, pos);
            }

            entries = applyCommands(entries, commands);

            // Sort entries by part of speech (nva)
            sort(entries.begin(), entries.end(), [](const Entry& a, const Entry& b) {
                return a.nva < b.nva;
            });

            if (!entries.empty()) {
                cout << "    |" << endl;
                for (const auto& entry : entries) {
                    cout << "      " << entry.key << " [" << entry.nva << "] : " << entry.value << endl;
                }
                cout << "     |" << endl;
            } else {
                cout << "   |" << endl;
                cout << "      <NOT FOUND> To be considered for next release. Thank you." << endl;
                cout << "    |" << endl;
            }
        }
        searchCount++;
    }
}

// Main function
int main() {
    // Loading message
    string filename = "C:\\Users\\MickeyMouse\\AbsolutePath\\DB\\Data.CS.SFSU.txt";
    ifstream file;
    bool correctFile = false;

    cout << "! Opening data file... " << filename << endl;

    while (!correctFile) {
        file.open(filename);
        if (!file.is_open()) {
            cout << "<!>ERROR<!> ===> File could not be opened." << endl;
            cout << "<!>ERROR<!> ===> Provided file path: " << filename << endl;
            cout << "<!>Enter the CORRECT data file path: ";
            getline(cin, filename);
        } else {
            correctFile = true;
        }
    }

    cout << "! Loading data..." << endl;
    cout << "! Loading completed..." << endl;
    cout << "! Closing data file... " << filename << endl;

    cout << "\n";

    unordered_set<string> uniqueKeys = getUniqueKeys(filename);
    list<string> definitions = countDefinitions(filename);
    multimap<string, pair<string, string>> dictionary = createDictionary(filename);

    intro(uniqueKeys, definitions);
    dictSearch(dictionary);

    return 0;
}

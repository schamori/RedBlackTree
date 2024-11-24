#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <regex>
#include <algorithm>
#include "RedBlackTree.h"
RedBlackTree extractUniqueWords(const std::string& filename) {
    // Open the file
    RedBlackTree tree;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
    }

    // Define a set to store unique words
    std::vector<std::string> uniqueWords;

    // Define a regex to match valid words (letters only)
    std::regex wordRegex("[a-zA-Z]+");

    std::string line;
    while (std::getline(file, line)) {
        // Convert the line to lowercase
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);

        // Use regex to extract words
        auto wordsBegin = std::sregex_iterator(line.begin(), line.end(), wordRegex);
        auto wordsEnd = std::sregex_iterator();

        for (auto it = wordsBegin; it != wordsEnd; ++it) {
            tree.insert(it->str()); // Insert word into the set
        }
    }

    file.close();

    return tree;
}

int main() {
    // File name
    std::string filename = "C:\\Users\\morit\\Documents\\Technikum\\5 Semester\\funktionales Programmierung\\Projekt\\war_and_peace.txt";

    // Extract and print unique words
    auto tree = extractUniqueWords(filename);

    tree.printTree();

    return 0;
}

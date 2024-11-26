#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>
#include <sstream>
#include "RedBlackTree.h"

RedBlackTree extractUniqueWords(const std::string& filename) {
    // Open the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
    }

    RedBlackTree tree;

    // Verwende einen Stream-Iterator, um den gesamten Dateiinhalt einzulesen
    std::istreambuf_iterator<char> it(file), end;
    std::string content(it, end);

    // Konvertiere den gesamten Inhalt in Kleinbuchstaben
    std::transform(content.begin(), content.end(), content.begin(), ::tolower);

    // Verwende Regex, um alle Wörter zu finden
    std::regex wordRegex("[a-zA-Z]+");
    auto wordsBegin = std::sregex_iterator(content.begin(), content.end(), wordRegex);
    auto wordsEnd = std::sregex_iterator();

    // Füge jedes Wort in den Baum ein
    for (auto it = wordsBegin; it != wordsEnd; ++it) {
        tree.insert(it->str());
    }

    return tree;
}

int main() {
    // File name
    doctest::Context context;

    int res = context.run();

    if (context.shouldExit()) {
        return res;
    }

    std::string filename = "D:\\FH\\5.Semester\\FPROG\\Projekt\\RedBlackTree\\war_and_peace.txt";

    // Extract and print unique words
    auto tree = extractUniqueWords(filename);

    std::ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open output.txt for writing." << std::endl;
        return 1;
    }

    tree.printTree(outputFile);
    outputFile.close();
    return 0;
}

// Testfälle
TEST_CASE("RedBlackTree Funktionen") {
    RedBlackTree tree;

    SUBCASE("Einfügen eines einzelnen Elements") {
        tree.insert("apple");
        CHECK(tree.find("apple") == true);
        CHECK(tree.getCount("apple") == 1);
    }

    SUBCASE("Einfügen mehrerer eindeutiger Elemente") {
        tree.insert("apple");
        tree.insert("banana");
        tree.insert("cherry");

        CHECK(tree.find("apple") == true);
        CHECK(tree.find("banana") == true);
        CHECK(tree.find("cherry") == true);
        CHECK(tree.find("date") == false);

        CHECK(tree.getCount("apple") == 1);
        CHECK(tree.getCount("banana") == 1);
        CHECK(tree.getCount("cherry") == 1);
    }

    SUBCASE("Einfügen von Duplikaten und Überprüfung der Zähler") {
        tree.insert("apple");
        tree.insert("banana");
        tree.insert("apple");
        tree.insert("apple");
        tree.insert("banana");

        CHECK(tree.find("apple") == true);
        CHECK(tree.find("banana") == true);

        CHECK(tree.getCount("apple") == 3);
        CHECK(tree.getCount("banana") == 2);
    }

    SUBCASE("Suche nach nicht vorhandenen Elementen") {
        tree.insert("apple");
        tree.insert("banana");

        CHECK(tree.find("cherry") == false);
        CHECK(tree.find("date") == false);
        CHECK(tree.getCount("cherry") == 0);
        CHECK(tree.getCount("date") == 0);
    }

    SUBCASE("Überprüfung der Unveränderlichkeit") {
        // Speichern des aktuellen Zustands des Baums
        tree.insert("apple");
        RedBlackTree treeVersion1 = tree; // Kopie des aktuellen Baums

        // Ein weiteres Element einfügen
        tree.insert("banana");
        RedBlackTree treeVersion2 = tree; // Kopie nach weiterer Einfügung

        // Überprüfen, dass treeVersion1 unverändert ist
        CHECK(treeVersion1.find("apple") == true);
        CHECK(treeVersion1.find("banana") == false);

        // Überprüfen, dass treeVersion2 beide Elemente enthält
        CHECK(treeVersion2.find("apple") == true);
        CHECK(treeVersion2.find("banana") == true);
    }

    SUBCASE("Testen der Traversierung und Sortierung der Ausgabe") {
        tree.insert("banana");
        tree.insert("apple");
        tree.insert("cherry");

        // Abfangen der Ausgabe von printTree
        std::stringstream output;
        tree.printTree(output);

        // Erwartete Ausgabe
        std::string expectedOutput =
                "apple count: 1\n"
                "banana count: 1\n"
                "cherry count: 1\n";

        CHECK(output.str() == expectedOutput);
    }

    SUBCASE("Einfügen in absteigender Reihenfolge") {
        tree.insert("d");
        tree.insert("c");
        tree.insert("b");
        tree.insert("a");

        CHECK(tree.find("a") == true);
        CHECK(tree.find("b") == true);
        CHECK(tree.find("c") == true);
        CHECK(tree.find("d") == true);

        // Überprüfen der Zähler
        CHECK(tree.getCount("a") == 1);
        CHECK(tree.getCount("b") == 1);
        CHECK(tree.getCount("c") == 1);
        CHECK(tree.getCount("d") == 1);

        // Abfangen der Ausgabe
        std::stringstream output;
        tree.printTree(output);

        // Erwartete Ausgabe
        std::string expectedOutput =
                "a count: 1\n"
                "b count: 1\n"
                "c count: 1\n"
                "d count: 1\n";

        CHECK(output.str() == expectedOutput);
    }

    SUBCASE("Einfügen in aufsteigender Reihenfolge") {
        tree.insert("a");
        tree.insert("b");
        tree.insert("c");
        tree.insert("d");

        CHECK(tree.find("a") == true);
        CHECK(tree.find("b") == true);
        CHECK(tree.find("c") == true);
        CHECK(tree.find("d") == true);

        // Überprüfen der Zähler
        CHECK(tree.getCount("a") == 1);
        CHECK(tree.getCount("b") == 1);
        CHECK(tree.getCount("c") == 1);
        CHECK(tree.getCount("d") == 1);

        // Abfangen der Ausgabe
        std::stringstream output;
        tree.printTree(output);

        // Erwartete Ausgabe
        std::string expectedOutput =
                "a count: 1\n"
                "b count: 1\n"
                "c count: 1\n"
                "d count: 1\n";

        CHECK(output.str() == expectedOutput);
    }

    SUBCASE("Große Anzahl von Einfügungen") {
        for (int i = 0; i < 1000; ++i) {
            tree.insert("key" + std::to_string(i));
        }

        // Überprüfen einiger Stichproben
        CHECK(tree.find("key0") == true);
        CHECK(tree.find("key500") == true);
        CHECK(tree.find("key999") == true);
        CHECK(tree.find("key1000") == false);

        // Überprüfen der Zähler
        CHECK(tree.getCount("key0") == 1);
        CHECK(tree.getCount("key500") == 1);
        CHECK(tree.getCount("key999") == 1);
    }
}

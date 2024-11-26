#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>
#include <sstream>
#include "RedBlackTree.h"

// Function to extract unique words and build the tree
std::shared_ptr<const Node> extractUniqueWords(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return nullptr;
    }

    std::istreambuf_iterator<char> it(file), end;
    std::string content(it, end);

    std::transform(content.begin(), content.end(), content.begin(), ::tolower);

    std::regex wordRegex("[a-zA-Z]+");
    auto wordsBegin = std::sregex_iterator(content.begin(), content.end(), wordRegex);
    auto wordsEnd = std::sregex_iterator();

    std::shared_ptr<const Node> root = nullptr;



    std::for_each(wordsBegin, wordsEnd, [&](const std::smatch& match) {
        bool increased = false;
        root = insert(root, match.str(), increased);
    });
    return root;
}

int main() {
    // File name
    doctest::Context context;

    int res = context.run();

    if (context.shouldExit()) {
        return res;
    }

    std::string filename = "war_and_peace.txt";

    auto tree = extractUniqueWords(filename);
    if (!tree) {
        return 1;
    }

    // Write the tree to an output file
    std::ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open output.txt for writing." << std::endl;
        return 1;
    }

    auto printTreeToFile = [&](const std::shared_ptr<const Node>& tree, std::ostream& output) {
        if (tree) {
            printTree(tree, output);
        }
    };

    printTreeToFile(tree, outputFile);
    outputFile.close();

    return 0;
}

// Testfälle
TEST_CASE("RedBlackTree Functions") {


    // Initialize the tree root
    std::shared_ptr<const Node> root = nullptr;

    SUBCASE("Insert a single element") {
        bool increased = false;
        root = insert(root, "apple", increased);

        CHECK(find(root, "apple") == true);
        CHECK(getCount(root, "apple") == 1);
    }

    SUBCASE("Insert multiple unique elements") {
        bool increased = false;
        root = insert(root, "apple", increased);
        root = insert(root, "banana", increased);
        root = insert(root, "cherry", increased);

        CHECK(find(root, "apple") == true);
        CHECK(find(root, "banana") == true);
        CHECK(find(root, "cherry") == true);
        CHECK(find(root, "date") == false);

        CHECK(getCount(root, "apple") == 1);
        CHECK(getCount(root, "banana") == 1);
        CHECK(getCount(root, "cherry") == 1);
    }

    SUBCASE("Insert duplicates and verify counts") {
        bool increased = false;
        root = insert(root, "apple", increased);
        root = insert(root, "banana", increased);
        root = insert(root, "apple", increased);
        root = insert(root, "apple", increased);
        root = insert(root, "banana", increased);

        CHECK(find(root, "apple") == true);
        CHECK(find(root, "banana") == true);

        CHECK(getCount(root, "apple") == 3);
        CHECK(getCount(root, "banana") == 2);
    }

    SUBCASE("Search for non-existing elements") {
        bool increased = false;
        root = insert(root, "apple", increased);
        root = insert(root, "banana", increased);

        CHECK(find(root, "cherry") == false);
        CHECK(find(root, "date") == false);
        CHECK(getCount(root, "cherry") == 0);
        CHECK(getCount(root, "date") == 0);
    }

    SUBCASE("Test traversal and sorted output") {
        bool increased = false;
        root = insert(root, "banana", increased);
        root = insert(root, "apple", increased);
        root = insert(root, "cherry", increased);

        std::stringstream output;
        printTree(root, output);

        std::string expectedOutput =
                "apple count: 1\n"
                "banana count: 1\n"
                "cherry count: 1\n";

        CHECK(output.str() == expectedOutput);
    }

    SUBCASE("Insert in descending order") {
        bool increased = false;
        root = insert(root, "d", increased);
        root = insert(root, "c", increased);
        root = insert(root, "b", increased);
        root = insert(root, "a", increased);

        CHECK(find(root, "a") == true);
        CHECK(find(root, "b") == true);
        CHECK(find(root, "c") == true);
        CHECK(find(root, "d") == true);

        CHECK(getCount(root, "a") == 1);
        CHECK(getCount(root, "b") == 1);
        CHECK(getCount(root, "c") == 1);
        CHECK(getCount(root, "d") == 1);

        std::stringstream output;
        printTree(root, output);

        std::string expectedOutput =
                "a count: 1\n"
                "b count: 1\n"
                "c count: 1\n"
                "d count: 1\n";

        CHECK(output.str() == expectedOutput);
    }

    SUBCASE("Insert in ascending order") {
        bool increased = false;
        root = insert(root, "a", increased);
        root = insert(root, "b", increased);
        root = insert(root, "c", increased);
        root = insert(root, "d", increased);

        CHECK(find(root, "a") == true);
        CHECK(find(root, "b") == true);
        CHECK(find(root, "c") == true);
        CHECK(find(root, "d") == true);

        CHECK(getCount(root, "a") == 1);
        CHECK(getCount(root, "b") == 1);
        CHECK(getCount(root, "c") == 1);
        CHECK(getCount(root, "d") == 1);

        std::stringstream output;
        printTree(root, output);

        std::string expectedOutput =
                "a count: 1\n"
                "b count: 1\n"
                "c count: 1\n"
                "d count: 1\n";

        CHECK(output.str() == expectedOutput);
    }

    SUBCASE("Large number of insertions") {
        bool increased = false;
        for (int i = 0; i < 1000; ++i) {
            root = insert(root, "key" + std::to_string(i), increased);
        }

        CHECK(find(root, "key0") == true);
        CHECK(find(root, "key500") == true);
        CHECK(find(root, "key999") == true);
        CHECK(find(root, "key1000") == false);

        CHECK(getCount(root, "key0") == 1);
        CHECK(getCount(root, "key500") == 1);
        CHECK(getCount(root, "key999") == 1);
    }
}

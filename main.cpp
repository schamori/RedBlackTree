#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <algorithm>
#include <sstream>
#include "RedBlackTree.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <memory>
#include <thread>
#include <mutex>
#include <algorithm>
#include <functional>
#include <iterator>

// Helper function for threading
std::shared_ptr<const Node> mergeTrees(const std::shared_ptr<const Node> root1, const std::shared_ptr<const Node> root2) {
    if (!root2) return root1;
    bool increased = false;
    std::shared_ptr<const Node> newRoot = nullptr;
    newRoot = insert(root1, root2->key, increased);
    newRoot = mergeTrees(newRoot, root2->left);
    newRoot = mergeTrees(newRoot, root2->right);
    return newRoot;
}

std::shared_ptr<const Node> extractUniqueWords(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return nullptr;
    }
    std::istreambuf_iterator<char> it(file), end;
    std::string content(it, end);

    auto transformString = [](const std::string& input, const std::function<char(char)>& transformer) -> std::string {
        std::string result(input.size(), '\0');
        std::transform(input.begin(), input.end(), result.begin(), transformer);
        return result;
    };

    std::string lowerContent = transformString(content, [](char c) { return std::tolower(c); });

    size_t numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 2; // Ensure at least 2 threads
    size_t contentSize = lowerContent.size();
    size_t chunkSize = contentSize / numThreads;

    std::vector<std::thread> threads;
    std::vector<std::shared_ptr<const Node>> localTrees(numThreads);

    std::regex wordRegex("[a-zA-Z]+");

    auto processChunk = [&](size_t index, const size_t start, const  size_t end) {
        size_t newEnd = end;
        if (newEnd < contentSize) {
            while (newEnd < contentSize && std::isalpha(lowerContent[newEnd])) {
                ++newEnd;
            }
        } else {
            newEnd = contentSize;
        }

        std::string chunk = lowerContent.substr(start, newEnd - start);

        std::sregex_iterator wordsBegin(chunk.begin(), chunk.end(), wordRegex);
        std::sregex_iterator wordsEnd;

        std::shared_ptr<const Node> localRoot = nullptr;
        for (auto it = wordsBegin; it != wordsEnd; ++it) {
            bool increased = false;
            localRoot = insert(localRoot, it->str(), increased);
        }

        localTrees[index] = localRoot;
    };

    for (size_t i = 0; i < numThreads; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? contentSize : (i + 1) * chunkSize;
        threads.emplace_back(processChunk, i, start, end);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::shared_ptr<const Node> globalRoot = nullptr;
    for (const auto& localRoot : localTrees) {
        globalRoot = mergeTrees(globalRoot, localRoot);
    }

    return globalRoot;
}


int main() {

    // File name
    doctest::Context context;

    int res = context.run();

    if (context.shouldExit()) {
        return res;
    }

    std::string filename = "C:\\Users\\morit\\Documents\\Technikum\\5 Semester\\funktionales Programmierung\\RedBlackTree\\war_and_peace.txt";

    auto startTime = std::chrono::high_resolution_clock::now();

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
    auto endTime = std::chrono::high_resolution_clock::now();
    auto totalDuration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "Total execution time: " << totalDuration << " ms" << std::endl;


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

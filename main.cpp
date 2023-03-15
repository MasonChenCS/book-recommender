#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include "ListStructure.h"
#include "RedBlackTree.h"
#include "BTree.h"
using namespace std::literals::chrono_literals;


std::string pad(const std::string message, const unsigned int displayWidth) {
    std::string padding = "";
    int numSpaces = displayWidth / 2 - message.size() / 2;
    if (numSpaces < 0)
        numSpaces = 0;
    for (unsigned int i = 0; i < numSpaces; i++)
        padding += " ";
    return padding;
}


std::string center(std::string message, const unsigned displayWidth = 100) {
    return pad(message, displayWidth) + message;
}


std::string centerBlock(std::string message, const unsigned displayWidth = 100) {
    std::string result;
    std::string line;
    std::istringstream inSS(message);
    while (getline(inSS, line)) {
        result += center(line, displayWidth) + '\n';
    }
    return result.substr(0, result.length()-1);
}


static bool isNonNegativeInteger(const std::string& number) {
    return number.find_first_not_of("0123456789") == std::string::npos;
}


std::chrono::time_point<std::chrono::high_resolution_clock> startTimer() {
    return std::chrono::high_resolution_clock::now();
}


float endTimer(const std::chrono::time_point<std::chrono::high_resolution_clock>& startTime, const std::string& message) {
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = endTime - startTime;
    std::cout << message << ": " << duration.count()*1000.0f << " ms" << std::endl;
    return duration.count()*1000.0f;
}


int main() {
    // Initialize data structures
    ListStructure listStruct;
    std::vector<RedBlackTree> rbTrees(5, RedBlackTree());
    std::vector<BTree> bTrees(5, BTree());

    // Parse data
    bool SKIP_HEADER_LINE = true;
    std::ifstream file("data.csv");
    if (file.is_open())
        std::cout << "File was successfully opened." << std::endl;
    else {
        std::cout << "File could not be opened." << std::endl;
        return 0;
    }
    std::string line;
    std::string token;
    std::string emptyString;
    auto startTime = startTimer();
    while (getline(file, line)) { // For every line
        if (SKIP_HEADER_LINE) {
            SKIP_HEADER_LINE = false;
            continue;
        }
        std::istringstream inSS(line);
        Book tempBook;
        try {
            getline(inSS, token, ',');
            int tempId = std::stoi(token);
            getline(inSS, token, ',');
            tempBook.title = token;
            getline(inSS, token, ',');
            tempBook.author = token;
            getline(inSS, token, ',');
            tempBook.rating = std::stof(token);
            getline(inSS, token, ',');
            tempBook.isbn = token;
            getline(inSS, token, ',');
            long long int tempIsbn13 = std::stoll(token);
            getline(inSS, token, ',');
            tempBook.language = token;
            getline(inSS, token, ',');
            int tempPageCount = std::stoi(token);
            getline(inSS, token, ',');
            int tempRatingCount = std::stoi(token);
            getline(inSS, token); // There is no comma delimiter at the end of the line
            int tempReviewCount = std::stoi(token);
            if (tempId < 0 && tempIsbn13 < 0 && tempPageCount < 0 && tempRatingCount < 0 && tempReviewCount < 0)
                throw std::invalid_argument("Negative number detected. Input numbers must be positive.");
            tempBook.id = tempId;
            tempBook.isbn13 = tempIsbn13;
            tempBook.pageCount = tempPageCount;
            tempBook.ratingCount = tempRatingCount;
            tempBook.reviewCount = tempReviewCount;
        }
        catch (std::invalid_argument e) {
            std::cout << e.what() << std::endl;
        }
        catch (...) {
            std::cout << "An unknown error occurred while parsing the data." << std::endl;
        }
        listStruct.insert(tempBook);
    }
    file.close();
    endTimer(startTime, "Loading Data from File");
    std::cout << std::endl;

    std::vector<std::string> treeTypes = {"Rating", "Page Count", "ID", "ISBN", "ISBN13"};
    float insertionTime = 0;
    for (unsigned int i = 0; i < treeTypes.size(); i++) {
        startTime = startTimer();
        for (unsigned int j = 0; j < listStruct.list.size(); j++) {
            rbTrees[i].insert(listStruct.list[j]->book, treeTypes[i]);
        }
        insertionTime += endTimer(startTime, "Inserting into Red Black Tree based on " + treeTypes[i]);
    }
    std::cout << "Average insertion time for Red Black Tree: " << insertionTime/treeTypes.size() << " ms\n" << std::endl;

    insertionTime = 0;
    for (unsigned int i = 0; i < treeTypes.size(); i++) {
        startTime = startTimer();
        for (unsigned int j = 0; j < listStruct.list.size(); j++) {
            bTrees[i].insert(listStruct.list[j]->book, treeTypes[i]);
        }
        insertionTime += endTimer(startTime, "Inserting into B Tree based on " + treeTypes[i]);
    }
    std::cout << "Average insertion time for B Tree: " << insertionTime/treeTypes.size() << " ms\n" << std::endl;

    std::string introMessage = "O ======================================================= O\nHello, welcome to\nBOOK RECOMMENDER\ncreated by Mason Chen, Hailin Zeng, and Jie Ting Li Lu\n\nWhat would you like to filter by?\nEnter the numerical selection below or [0] to exit:\n|----------------------------------------------------------------------------------------------------|\n[1] Title Keyword [2] Author Keyword [3] Average Rating [4] Page Count [5] Book ID [6] ISBN [7] ISBN13\n|----------------------------------------------------------------------------------------------------|\nO ======================================================= O";
    std::cout << centerBlock(introMessage) << std::endl;
    introMessage = "O ======================================================= O\nWhat would you like to filter by?\nEnter the numerical selection below or [0] to exit:\n|----------------------------------------------------------------------------------------------------|\n[1] Title Keyword [2] Author Keyword [3] Average Rating [4] Page Count [5] Book ID [6] ISBN [7] ISBN13\n|----------------------------------------------------------------------------------------------------|\nO ======================================================= O";
    std::string userInput = "";
    bool displaySmallMenu = false;
    while (true) {
        if (displaySmallMenu)
            std::cout << centerBlock(introMessage) << std::endl;
        std::cin >> userInput;
        if (userInput == "0")
            break;
        else if (isNonNegativeInteger(userInput) && std::stoi(userInput) > 0 && std::stoi(userInput) <= 7) {
            std::vector<Book*> listResults = {};
            std::vector<Book*> rbTreeResults = {};
            std::vector<Book*> bTreeResults = {};
            if (userInput == "1" || userInput == "2") {
                std::cout << center("Enter search keywords (separate by spaces for multiple queries):") << std::endl;
                std::string token;
                std::vector<std::string> keywords;
                std::cin.ignore();
                getline(std::cin, emptyString);
                std::istringstream inSS(emptyString + ' '); // Add a space as a delimiter at the end
                while (getline(inSS, token, ' ')) {
                    if (token.length() > 0)
                        keywords.push_back(Book::lowercase(token));
                }
                std::string searchMode;
                while (true) {
                    std::cout << center("Enter [1] for tokenized search or [2] for exact search:") << std::endl;
                    std::cin >> searchMode;
                    if (searchMode == "1" || searchMode == "2")
                        break;
                }
                if (userInput == "1") {
                    startTime = startTimer();
                    listResults = listStruct.searchKeywords(keywords, "title", searchMode == "1" ? "tokenized" : "exact");
                    endTimer(startTime, "Title Search with List");

                    startTime = startTimer();
                    rbTreeResults = rbTrees[0].searchKeywords(keywords, "title", searchMode == "1" ? "tokenized" : "exact");
                    endTimer(startTime, "Title Search with Red Black Tree");

                    startTime = startTimer();
                    bTreeResults = bTrees[0].searchKeywords(keywords, "title", searchMode == "1" ? "tokenized" : "exact");
                    endTimer(startTime, "Title Search with B Tree");
                    std::cout << std::endl;
                }
                else {
                    startTime = startTimer();
                    listResults = listStruct.searchKeywords(keywords, "author", searchMode == "1" ? "tokenized" : "exact");
                    endTimer(startTime, "Author Search with List");

                    startTime = startTimer();
                    rbTreeResults = listStruct.searchKeywords(keywords, "author", searchMode == "1" ? "tokenized" : "exact");
                    endTimer(startTime, "Author Search with Red Black Tree");

                    startTime = startTimer();
                    bTreeResults = listStruct.searchKeywords(keywords, "author", searchMode == "1" ? "tokenized" : "exact");
                    endTimer(startTime, "Author Search with B Tree");
                    std::cout << std::endl;
                }
                std::cin.ignore();
            }
            else if (userInput == "3" || userInput == "4") {
                std::cout << center(userInput == "3" ? "Enter average rating:" : "Enter page count:") << std::endl;
                float key = 0;
                std::string keyString;
                while (true) {
                    try {
                        std::cin >> keyString;
                        key = std::stof(keyString);
                        if ((userInput == "3" && (key < 0.00 || key > 5.00)) || (userInput == "4" && key < 0))
                            throw std::invalid_argument("");
                        break;
                    }
                    catch (...) {
                        std::string suffix = (userInput == "3" ? "rating below 5" : "page count");
                        std::cout << center("Please enter a non-negative " + suffix) << std::endl;
                    }
                }

                std::cout << center("Enter maximum number of results:") << std::endl;
                int capacity = 0;
                std::string capacityString;
                while (true) {
                    try {
                        std::cin >> capacityString;
                        capacity = std::stoi(capacityString);
                        if (capacity < 0 || capacity > listStruct.list.size())
                            throw std::invalid_argument("");
                        break;
                    }
                    catch (...) {
                        std::cout << center("Please enter a number between 0 and " + std::to_string(listStruct.list.size())) << std::endl;
                    }
                }

                unsigned int index = stoi(userInput) - 3;
                startTime = startTimer();
                listResults = listStruct.searchApproximate(key, treeTypes[index], capacity);
                endTimer(startTime, treeTypes[index] + " Search with List");

                startTime = startTimer();
                rbTreeResults = rbTrees[index].searchApproximate(key, treeTypes[index], capacity);
                endTimer(startTime, treeTypes[index] + " Search with Red Black Tree");

                startTime = startTimer();
                bTreeResults = bTrees[index].searchApproximate(key, treeTypes[index], capacity);
                endTimer(startTime, treeTypes[index] + " Search with B Tree");
                std::cout << std::endl;

                std::cin.ignore();
            }
            else if (userInput == "5" || userInput == "6" || userInput == "7") {
                std::string searchMode = treeTypes[stoi(userInput) - 3];
                std::cout << center("Enter an existing book " + Book::uppercase(searchMode)) << std::endl;
                std::cin.ignore();
                getline(std::cin, emptyString);

                startTime = startTimer();
                Book* listResult = listStruct.searchExact(emptyString, searchMode);
                endTimer(startTime, searchMode + " Search with List");

                startTime = startTimer();
                Book* rbTreeResult = rbTrees[stoi(userInput) - 3].searchExact(emptyString, searchMode);
                endTimer(startTime, searchMode + " Search with Red Black Tree");

                startTime = startTimer();
                Book* bTreeResult = bTrees[stoi(userInput) - 3].searchExact(emptyString, searchMode);
                endTimer(startTime, searchMode + " Search with B Tree");
                std::cout << std::endl;

                if (listResult != nullptr)
                    listResults.push_back(listResult);
                if (rbTreeResult != nullptr)
                    rbTreeResults.push_back(rbTreeResult);
                if (bTreeResult != nullptr)
                    bTreeResults.push_back(bTreeResult);
            }

            if (listResults.size() == 0)
                std::cout << center("No results found in List.") << std::endl;
            else
                std::cout << center(std::to_string(listResults.size()) + " results returned in List.") << std::endl;
            for (unsigned int i = 0; i < listResults.size(); i++) {
                std::cout << "#" << i+1 << ": " << listResults[i]->printBookInfo() << "\n" << std::endl;
            }

            if (rbTreeResults.size() == 0)
                std::cout << center("No results found in Red Black Tree.") << std::endl;
            else
                std::cout << center(std::to_string(rbTreeResults.size()) + " results returned in Red Black Tree.") << std::endl;
            for (unsigned int i = 0; i < rbTreeResults.size(); i++) {
                std::cout << "#" << i+1 << ": " << rbTreeResults[i]->printBookInfo() << "\n" << std::endl;
            }

            if (bTreeResults.size() == 0)
                std::cout << center("No results found in B Tree.") << std::endl;
            else
                std::cout << center(std::to_string(bTreeResults.size()) + " results returned in B Tree.") << std::endl;
            for (unsigned int i = 0; i < bTreeResults.size(); i++) {
                std::cout << "#" << i+1 << ": " << bTreeResults[i]->printBookInfo() << "\n" << std::endl;
            }

            std::cout << center("Press ENTER to return to the main screen.") << std::endl;
            getline(std::cin, emptyString);
            displaySmallMenu = true;
        }
        else {
            std::cout << center("Please enter a valid option.") << std::endl;
            displaySmallMenu = false;
        }
    }
    std::cout << center("Thank you for using BOOK RECOMMENDER!") << std::endl;
    return 0;
}
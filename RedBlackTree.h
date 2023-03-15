#include "Book.h"


class RedBlackTree {
    struct Node {
        Book book;
        bool isRed;
        // 1. Add any necessary additional variables
        // 2. Implement any necessary private helper functions

        Node(Book book_) : book(book_) {}
        ~Node() {}
    };
public:
    void insert(Book book_, std::string balanceMode);
    std::vector<Book*> searchKeywords(std::vector<std::string> keywords, const std::string searchMode, const std::string matchMode);
    std::vector<Book*> searchApproximate(const float& key, const std::string searchMode, const unsigned int capacity);
    Book* searchExact(const std::string& key, const std::string searchMode);
    RedBlackTree();
    ~RedBlackTree();
};


RedBlackTree::RedBlackTree() {
}


RedBlackTree::~RedBlackTree() {
    // De-allocate memory of each Node in the data structure
}


void RedBlackTree::insert(Book book_, std::string balanceMode) {
    // 1. Insert a dynamically allocated Node (which contains a Book variable that you initialize using book_)
    // 2. Balance the data structure based on balanceMode, which can be "Rating", "Page Count", "ID", "ISBN", or "ISBN13"
    // Intended Time Complexity is O(log n)
}


std::vector<Book*> RedBlackTree::searchKeywords(std::vector<std::string> keywords, const std::string searchMode, const std::string matchMode) {
    std::vector<Book*> results = {};

    // Iterate through each Node in your data structure
    //     a. For each Node, loop through the vector "keywords" to check for matches. Keep the below conditions in mind:
    //     b. matchMode can be "exact" or "tokenized"
    //     c. searchMode can be "title" or "author"
    // Intended Time Complexity is O(n)

    // Uncomment and execute the code below for EACH node:
    /*
    for (unsigned int j = 0; j < keywords.size(); j++) {
        if (matchMode == "exact") {
            Book* tempBook = nullptr; // replace nullptr with the Book pointer that you access with your data structure
            if (Book::lowercase(searchMode == "title" ? tempBook->title : tempBook->author).find(keywords[j]) != std::string::npos) {
                results.push_back(tempBook);
                break;
            }
        }
        else if (matchMode == "tokenized") {
            Book* tempBook = nullptr; // replace nullptr with the Book pointer that you access with your data structure
            std::vector<std::string> book = tempBook->splitText(searchMode == "title" ? tempBook->title : tempBook->author);
            if (std::find(book.begin(), book.end(), keywords[j]) != book.end()) {
                results.push_back(tempBook);
                break;
            }
        }
    }
    */
    // You may refer to the searchKeywords function in ListStructure.h for an idea of the implementation

    return results;
}


std::vector<Book*> RedBlackTree::searchApproximate(const float& key, const std::string searchMode, const unsigned int capacity) {
    std::vector<Book*> results = {};

    // The algorithm idea is that you will return the nodes you visit as you travel closer to the key in your data structure. Steps:

    // 1. Search for the key. The key does not actually have to exist within the data structure. Time Complexity is O(log n)
    //    searchMode can be "Rating" or "Page Count"
    // 2. As you search for the key, keep the recently visited nodes in the "results" vector. Maximum size of the vector should be "capacity"
    //    If you locate the key really quickly but your "results" vector is not at maximum size yet, that is okay. You can return it anyway
    //    For example, if your capacity is 10, but you have already found the key after visiting 2 nodes, you can just return the 2 nodes

    return {};
}


Book* RedBlackTree::searchExact(const std::string& key, const std::string searchMode) {
    // Search the data structure for a Book with a specific string matching the key. Return a pointer to the Book. Time Complexity is O(log n)
    // If the key does not exist, return nullptr
    // searchMode can be "ID", "ISBN", or "ISBN13"
    return nullptr;
}
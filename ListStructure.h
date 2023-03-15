#include <queue>
#include "Book.h"


class ListStructure {
    struct Node {
        Book book;
        float key;
        std::string searchMode;
        bool operator<(const Node& n) const {
            if (searchMode == "Rating")
                return fabs(this->book.rating - key) < fabs(n.book.rating - key);
            else
                return fabs((float)this->book.pageCount - key) < fabs((float)n.book.pageCount - key);
        }
        Node(Book book_) : book(book_) {}
        ~Node() {}
    };
public:
    std::vector<Node*> list = {};
    void insert(Book book_);
    std::vector<Book*> searchKeywords(std::vector<std::string> keywords, const std::string searchMode, const std::string matchMode);
    std::vector<Book*> searchApproximate(const float& key, const std::string searchMode, const unsigned int capacity);
    Book* searchExact(const std::string& key, const std::string searchMode);
    ListStructure();
    ~ListStructure();
    struct nodeComparator {
        bool operator() (Node* n1, Node* n2) {
            return *n1 < *n2;
        }
    };
};


ListStructure::ListStructure() {
}


ListStructure::~ListStructure() {
    while (!list.empty()) {
        delete list.back();
        list.pop_back();
    }
}


void ListStructure::insert(Book book_) {
    list.push_back(new Node(book_));
}


std::vector<Book*> ListStructure::searchKeywords(std::vector<std::string> keywords, const std::string searchMode, const std::string matchMode) {
    std::vector<Book*> results = {};
    for (unsigned int i = 0; i < list.size(); i++) {
        for (unsigned int j = 0; j < keywords.size(); j++) {
            if (matchMode == "exact") {
                Book* tempBook = &list[i]->book;
                if (Book::lowercase(searchMode == "title" ? tempBook->title : tempBook->author).find(keywords[j]) != std::string::npos) {
                    results.push_back(tempBook);
                    break;
                }
            }
            else if (matchMode == "tokenized") {
                Book* tempBook = &list[i]->book;
                std::vector<std::string> book = tempBook->splitText(searchMode == "title" ? tempBook->title : tempBook->author);
                if (std::find(book.begin(), book.end(), keywords[j]) != book.end()) {
                    results.push_back(tempBook);
                    break;
                }
            }
        }
    }
    return results;
}


std::vector<Book*> ListStructure::searchApproximate(const float& key, const std::string searchMode, const unsigned int capacity) {
    std::vector<Book*> results = {};
    std::priority_queue<Node*, std::vector<Node*>, nodeComparator> pq;
    for (unsigned int i = 0; i < list.size(); i++) {
        list[i]->searchMode = searchMode;
        list[i]->key = key;
        pq.push(list[i]);
        if (pq.size() > capacity)
            pq.pop();
    }
    while (!pq.empty()) {
        results.push_back(&pq.top()->book);
        pq.pop();
    }
    return results;
}


Book* ListStructure::searchExact(const std::string& key, const std::string searchMode) {
    for (unsigned int i = 0; i < list.size(); i++) {
        if ((searchMode == "ID" && std::to_string(list[i]->book.id) == key) ||
            (searchMode == "ISBN" && Book::lowercase(list[i]->book.isbn) == Book::lowercase(key)) ||
            (searchMode == "ISBN13" && std::to_string(list[i]->book.isbn13) == key))
            return &list[i]->book;
    }
    return nullptr;
}
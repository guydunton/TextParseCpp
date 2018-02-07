#include "catch.hpp"
#include "../include/ParserBuilder.hpp"

void handleTitle(const std::string& title, std::string& output) {
    output = title;
}

TEST_CASE("Invoke works with function pointer") {
    const auto text = "book Dune";
    const auto parser = createParser<std::string>()
        .matching("book")
        .selecting(1)
        .invoking(&handleTitle)
        .finalize();
    
    std::string output;
    parser.parse(text, &output);
    REQUIRE(output == "Dune");
}

namespace {
    struct Book {
        std::string title;
        void setAuthor(const std::string& author) {
            this->author = author;
        }
        std::string author;
    };
}

TEST_CASE("Invoke works with a member function ptr") {
    const auto text = "author Tolkien";
    const auto parser = createParser<Book>()
        .matching("author")
        .selecting(1)
        .invoking(&Book::setAuthor)
        .finalize();
    
    Book book;
    parser.parse(text, &book);
    REQUIRE(book.author == "Tolkien");
}

TEST_CASE("Invoke works with a member object pointer")
{
    const auto text = "book Watchmen";
    const auto parser = createParser<Book>()
        .matching("book")
        .selecting(1)
        .invoking(&Book::title)
        .finalize();
    
    Book book;
    parser.parse(text, &book);
    REQUIRE(book.title == "Watchmen");
}

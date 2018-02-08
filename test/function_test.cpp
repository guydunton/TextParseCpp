#include "catch.hpp"
#include <parser/utility/function.hpp>

TEST_CASE("Function works with lambda")
{
    ParserSpace::Function<int(int&)> lambdaFunction{ [](int& i) {
        i *= 2;
        return 3;
        
    } };
    int i = 1;
    int result = lambdaFunction(i);
    
    REQUIRE(i == 2);
    REQUIRE(result == 3);
}

namespace {
    std::string createText(bool i) {
        return (i ? "true" : "false");
    }
}

TEST_CASE("Function works with function pointer")
{
    ParserSpace::Function<std::string(bool)> functionPtr{ &createText };
    
    std::string result1 = functionPtr(true);
    std::string result2 = functionPtr(false);
    
    REQUIRE(result1 == "true");
    REQUIRE(result2 == "false");
}

namespace {
    struct Book {
        int version = 0;
        bool isInPrint(int i) const {
            return i > 3;
        }
        void setVersion(int v) {
            version = v;
        }
        std::string title;
    };
}

TEST_CASE("Function works with member function pointer")
{
    ParserSpace::Function<void(int, Book&)> memberFunc{ &Book::setVersion };
    Book book;
    memberFunc(3, book);
    REQUIRE(book.version == 3);
}

TEST_CASE("Function works with const member function pointer")
{
    ParserSpace::Function<bool(int, Book&)> memberFunc{ &Book::isInPrint };
    Book book;
    bool result = memberFunc(4, book);
    REQUIRE(result == true);
}

TEST_CASE("Function works with member data pointer")
{
    ParserSpace::Function<void(int, Book&)> memberObj{ &Book::version };
    Book book;
    memberObj(3, book);
    REQUIRE(book.version == 3);
}

TEST_CASE("Function works with member data pointer and passing in const ref")
{
    ParserSpace::Function<void(const std::string&, Book&)> memberObj{ &Book::title };
}
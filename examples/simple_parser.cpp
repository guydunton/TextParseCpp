
/*
 * This is the text we want to process:
 * 
 * Book: Dune
 * author: F.Herbert
 * Published: 01/08/1965
 * Pages: 412
 */

#include <string>
#include <parser/utility/text.hpp>
#include <parser/parser_builder.hpp>

struct Date {
	int day = 0;
	int month = 0;
	int year = 0;
};

struct Book {
	std::string title;
	std::string author;
	Date publicationDate;
	int pages = 0;
};

Parser<Book> createBookParser() {
	const auto title = createParser<Book>()
		.matching("Book:")
		.selecting(1)
		.invoking(&Book::title)
		.finalize();

	const auto author = createParser<Book>()
		.matching("author:")
		.selecting(1)
		.invoking(&Book::author)
		.finalize();

	const auto publicationDate = createParser<Book>()
		.matching("Published:")
		.selecting(1)
		.invoking([](const std::string& dateString, Book& book){
			const auto splitDate = Text::splitString(dateString, '/');
			book.publicationDate.day = stoi(splitDate.at(0));
			book.publicationDate.month = stoi(splitDate.at(1));
			book.publicationDate.year = stoi(splitDate.at(2));
		}).finalize();

	const auto pages = createParser<Book>()
		.matching("Pages:")
		.selecting(1)
		.invoking([](const std::string& pagesString, Book& book){
			book.pages = stoi(pagesString);
		}).finalize();

	return (title & author & publicationDate & pages);
}

int main() {
	const auto bookParser = createBookParser();

	const auto text = 
		"Book: Dune\n"
		"author: F.Herbert\n"
		"Published: 01/08/1965\n"
		"Pages: 412";

	Book book;
	bookParser.parse(text, &book);

	return 0;
}
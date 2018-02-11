# TextMatch

[![Build Status](https://travis-ci.org/gdunton/TextParseCpp.svg?branch=master)](https://travis-ci.org/gdunton/TextParseCpp)
[![Build status](https://ci.appveyor.com/api/projects/status/wlcba6c1b9vsrdqq?svg=true)](https://ci.appveyor.com/project/gdunton/textmatch)
[![codecov](https://codecov.io/gh/gdunton/TextParseCpp/branch/master/graph/badge.svg)](https://codecov.io/gh/gdunton/TextParseCpp)

TextParseCpp is a library to make parsing text into objects a little easier.

This library is an exploration at a replacement to the sort of code I'm used to seeing for parsing text. 

## Example

To parse this:
```
book-name Dune
```

I would see:
```cpp
auto text = "book-name Dune";
auto currentLineParts = splitString(text);
auto bookStore = BookStore{};

if(currentLineParts.at(0) == "book-name") {
	// Grab some widely available object
	bookStore.addBook(currentLineParts.at(1));
}
```

The problems with this are:
- The business logic here is obscured behind all the boiler plate
- There is no type checking or automation of conversions
- The code favours larger functions and make reuse harder

To look into solving this I'm proposing this architecture:

```cpp
const auto parser = makeParser<BookStore>()
  .matching("book-name")
  .selecting(1)
  .invoking(&BookStore::addBook)
  .finalize();
  
  auto bookStore = BookStore{};
  parser.parse("book-name Dune", &bookStore);
```

The goal is to make parsing simpler.

## Current Status

At the moment the method invoking requires a lambda which takes a string and puts it into the output e.g.
```cpp
// The reset of the parser
.invoking([](const string& name, BookStore& output){
	output.addBook(name);
}).finalize();
```

The code can currently also type check the number of selection calls against the lambda

Parsers can be combined using the & operator e.g.
```cpp
Parser<Book> combined = parser1 & parser2 & parser3;
```

We can now also use pointers to member functions and pointers to member data in the invoke function. e.g.
```cpp
struct Book {
	std::string title;
	void setTitle(const std::string& title);
};

const auto p1 = createParser<Book>()
	// First parts of the parser
	.invoking(&Book::title).finalize();

const auto p2 = createParser<Book>()
	// First part of the parser
	.invoking(&Book::setTitle).finalize();
```
The parer will do the correct thing. 
*Pointers to member functions must have the same number of arguments as the number of select calls.
*Pointers to member data must always take a single argument so only 1 select can be in the parser.

## Future plans
- [x] Combine parsers together to form a new combines parser.
- [x] Pass a pointer to member object into invoking.
- [ ] Allow parsers to have submatchers to parse more complex lines.
- [ ] Allow chaining parsers so that returns from a lambda are passed into sub-parsers.
- [ ] Allow a parser to span multiple lines by calling an until functions.
- [ ] The matching function can be more complex to allow match tokens to be "any" or "not".

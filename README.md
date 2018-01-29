# TextMatch

Text match is a library that makes it simpler to search through a piece text to find desired parts.

e.g.
book-name Dune 

will be matched by the C++ code:

```cpp
const auto matcher = createMatcher<std::string>()
  .matching("book-name")
  .grab(1)
  .calling([](const std::string& name, std::string& output){
    output = name;
  });
```




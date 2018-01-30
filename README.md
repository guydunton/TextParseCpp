# TextMatch

[![Build Status](https://travis-ci.org/gdunton/TextMatch.svg?branch=master)](https://travis-ci.org/gdunton/TextMatch)
[![Build status](https://ci.appveyor.com/api/projects/status/wlcba6c1b9vsrdqq?svg=true)](https://ci.appveyor.com/project/gdunton/textmatch)
[![codecov](https://codecov.io/gh/gdunton/TextMatch/branch/master/graph/badge.svg)](https://codecov.io/gh/gdunton/TextMatch)

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




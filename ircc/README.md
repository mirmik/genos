# Simple c/c++ resource compiler

The `ircc` program packages the target files into a key-value store that can be connected to a C++ and C program.

## Installation
```bash
cmake .
cmake --build . 
cmake --install .
```
It install `ircc` binary and `ircc/ircc.h` header. Header contains access and list methods for c and c++ projects.

## Example 

hello.txt: 
```
HelloWorld
```

foo.txt: 
```
HelloUnderWorld
```

resources.txt: 
```bash
/hello ./helloworld.txt
another_key ./foo.txt
```

If you have files `hello.txt`, `foo.txt`, `resources.txt` then the `ircc` command give you cpp file:
```bash
ircc resources.txt -o ircc_resources.gen.cpp
``` 
```c++
/// ... headers

const char* const IRCC_RESOURCES_0 = 
		"\x48\x65\x6C\x6C\x6F\x57\x6F\x72\x6C\x64";

const char* const IRCC_RESOURCES_1 = 
		"\x48\x65\x6C\x6C\x6F\x55\x6E\x64\x65\x72\x57\x6F\x72\x6C\x64";

/// ... key_value_size type

struct key_value_size IRCC_RESOURCES_[] = {
	{"/hello", IRCC_RESOURCES_0, 10},
	{"another_key", IRCC_RESOURCES_1, 15},
	{"/image", IRCC_RESOURCES_2, 38905},
	{NULL, NULL, 0}};

/// ... helper functions 
```

It can be used in program:
```c++
#include <iostream>
#include <string>

extern std::string ircc_string(const char *key);

int main()
{
    std::string resource = ircc_string("/hello");
    std::cout << hello << std::endl;
    return 0;
}
``` 

## List of access methods:
```c++
extern std::string ircc_string(const char *key);
extern std::vector<uint8_t> ircc_vector(const char *key);
extern std::pair<const char*, size_t> ircc_pair(const char *key);
extern "C" const char *ircc_c_string(const char *key, size_t *sizeptr);
```

## Keys iteration methods:
```c++
extern std::vector<std::string> ircc_keys();
extern "C" const char *ircc_name_by_no(size_t no);
```

## C style.
`ircc` can create c-style files with `c_only` key (Only c-style functions will be added):
```bash
ircc resources.txt -o ircc_resources.gen.c --c_only 
```

## CMake example:
It can be used with cmake with `add_custom_command`:

```cmake
project(ircc)
cmake_minimum_required(VERSION 3.0)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(SOURCES 
	main.cpp
    ircc_resources.gen.cpp)

add_executable(cmake_runtest ${SOURCES})

execute_process(COMMAND ircc resources.txt -o ircc_resources.gen.cpp --sources-cmake
                OUTPUT_VARIABLE RESOURCE_LIST)
add_custom_command(OUTPUT ircc_resources.gen.cpp
    COMMAND ircc resources.txt -o ircc_resources.gen.cpp
    DEPENDS ${RESOURCE_LIST} # for rebuild on resourses change
)
```

## Comments and directory syntax
If you have project tree like
```bash
/
	web/
		index.html
		foo.json
		bar.json
	resources.txt
	...
```
then script resources.txt 
```bash
# It is directory syntax
/web/ ./web/
```
will have the same effect as
```bash
/web/index.html ./web/index.html
/web/foo.json ./web/foo.json
/web/bar.json ./web/bar.json
```
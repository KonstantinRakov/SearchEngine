# **Local search engine for JSON files**
_______


## **Technology Stack**

* Assembly sources and build with ["CMake"](https://cmake.org/)
* ["MinGW"](https://www.mingw-w64.org/) is used for compilation  
* The ["nlohmann/json"](https://github.com/nlohmann/json) library is used to work with JSON files
* The ["googletests"](https://github.com/google/googletest/) library is used for Unit tests 

## **Manual**
After building the project, for the program to work correctly, copy the following elements to the /build/src folder:
* config.json - The file with the search configuration. It is necessary to specify the path to the files on which it will be produced
* requests.json - File with incoming requests 
* answers.json - The file with the search results
* resources/ - Folder with files to search


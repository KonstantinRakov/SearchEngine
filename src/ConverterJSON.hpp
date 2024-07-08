#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

class ConverterJSON
{
private:
    ConverterJSON() = default; // private constructor for singleton realization
    static ConverterJSON* instance;
    const std::string ANSWERS_FILE_PATH = "answers.json";
    std::string applicationName;
    std::string applicationVersion;
    int maxResponses{ 5 };
    std::vector<std::string> resourcesPaths;
    std::vector<std::string> textDocuments;
    std::vector<std::string> requests;

public:

    /**
    * Get the files content in string vector form 
    * @return content of files listed in config.json 
    */
    std::vector<std::string> getTextDocuments();

    /**
     * Get the max responses per request
     * @return max responses count
     */
    int getResponsesLimit() const;

    /**
     * Get the Requests from requests.json 
     * @return requests vector
     */
    std::vector<std::string> getRequests();

    /**
     * Put requests results into answers.json file 
     * @param[in] answers - vector of answers to be placed into answers.json file
     */
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);

    /**
    * Get the Instance object of ConverterJSON class
    * @return instance of the singleton
    */
    static ConverterJSON* getInstance();

    /**
     * Read config file specified in CONFIG_FILE_PATH
     */
    void readConfigFile(std::string path = "config.json");

    /**
     * Read request file specified in REQUEST_FILE_PATH
     */
    void readRequestFile(std::string path = "requests.json");

    /**
     * Get maximal responses quantity, which can be returned be Search Server
     * @return maximal responses quantity
     */
    int getMaxResponses() const;
};
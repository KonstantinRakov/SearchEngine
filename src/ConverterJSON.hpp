#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

class ConverterJSON
{
private:
    const std::string REQUESTS_FILE_PATH = "requests.json";
    const std::string FILE_REQUESTS_FOUND = "Request file has been found. Number of queries found: ";
    const std::string FILE_REQUESTS_NOT_FOUND = "Error, request file was not found. The path to the file: ";
    const std::string CONFIG_FILE_PATH = "config.json";
    const std::string FILE_CONFIG_FOUND = "Config file has been found.\nConfig list:\n";
    const std::string FILE_CONFIG_NOT_FOUND = "Error, config file was not found. The path to the file: ";
    const std::string ANSWERS_FILE_PATH = "answers.json";
    const std::string FILE_ANSWERS_FOUND = "Answers file has been found.\nConfig list:\n";
    const std::string FILE_ANSWERS_NOT_FOUND = "Error, answers file was not found. The path to the file: ";
    const std::string ANSWERS_EMPTY = "No answers for recording.\n";
    const std::string ANSWERS_COMPLITE = "The answers have been prepared and uploaded to a file\n";    
    std::string applicationName;
    std::string applicationVersion;
    int maxResponses{ 5 };
    std::vector<std::string> resourcesPaths;
    std::vector<std::string> textDocuments;
    std::vector<std::string> requests;

public:

    ConverterJSON() = default;
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
     * Read config file specified in CONFIG_FILE_PATH
     */
    void readConfigFile();

    /**
     * Read request file specified in REQUEST_FILE_PATH
     */
    void readRequestFile();
};
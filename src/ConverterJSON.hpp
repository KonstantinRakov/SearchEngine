#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <exception>

class ConverterJSON
{
private:
    const std::string REQUESTS_FILE_PATH = "requests.json";
    const std::string FILE_REQUESTS_FOUND = "Request file has been found.\nNumber of requests: ";
    const std::string CONFIG_FILE_PATH = "config.json";
    const std::string FILE_CONFIG_FOUND = "Config file has been found.\nConfig list:\n";
    const std::string ANSWERS_FILE_PATH = "answers.json";
    const std::string FILE_ANSWERS_FOUND = "Answers file has been found.\nConfig list:\n";
    const std::string FILE_ANSWERS_NOT_FOUND = "Error, answers file was not found. The path to the file: ";
    const std::string ANSWERS_EMPTY = "No answers for recording.\n";
    const std::string ANSWERS_COMPLITE = "The answers have been prepared and uploaded to a file\n";


    const std::string ERROR_FILE_REQUESTS_NOT_FOUND = "request file is missing. The path to the file: ";
    const std::string ERROR_REQUESTS_EMPTY = "requests file is empty\n";
    const std::string ERROR_REQUESTS_COUNT = "incorrect number of requests\n";

    const std::string ERROR_FILE_CONFIG_NOT_FOUND = "config file is missing. The path to the file: ";
    const std::string ERROR_CONFIG_EMPTY = "config file is empty\n";
    const std::string ERROR_CONFIG_NOT_NAME = "error there is no \"name\" field in the configuration file\n";
    const std::string ERROR_CONFIG_NOT_VERSION = "error there is no \"version\" field in the configuration file\n";
    const std::string ERROR_CONFIG_NOT_MAX_RESPONSES = "error there is no \"max_responses\" field in the configuration file\n";

    std::string applicationName;
    std::string applicationVersion;
    int maxResponses{ 5 };
    std::vector<std::string> resourcesPaths;
    std::vector<std::string> textDocuments;
    std::vector<std::string> requests;

    bool invalidConfig (const nlohmann::json &configDictionary) const;

    bool inValidReuests(const nlohmann::json &requestsDictionary) const;

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
    std::vector<std::string> getRequests() const;

    /**
     * Put requests results into answers.json file 
     * @param[in] answers - vector of answers to be placed into answers.json file
     */
    void putAnswers(std::vector<std::vector<std::pair<int, float>>>& answers) const;

    /**
     * Read config file specified in CONFIG_FILE_PATH
     */
    void readConfigFile();

    /**
     * Read request file specified in REQUEST_FILE_PATH
     */
    void readRequestFile();
};
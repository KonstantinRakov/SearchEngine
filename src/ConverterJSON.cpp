#include "ConverterJSON.hpp"

bool ConverterJSON::invalidConfig (const nlohmann::json &configDictionary) const {
    if (configDictionary.contains("config") && configDictionary.contains("files") ) {
        if( configDictionary["config"].find("name") == configDictionary["config"].end()){
            throw std::invalid_argument(ERROR_CONFIG_NOT_NAME);
        }
        if( configDictionary["config"].find("version") == configDictionary["config"].end()){
            throw std::invalid_argument(ERROR_CONFIG_NOT_VERSION);
        }
        if( configDictionary["config"].find("max_responses") == configDictionary["config"].end()){
            throw std::invalid_argument(ERROR_CONFIG_NOT_MAX_RESPONSES);
        }
    }
    else {
        throw std::invalid_argument(ERROR_CONFIG_EMPTY); 
    }
    return true;
}

bool ConverterJSON::inValidReuests(const nlohmann::json &requestsDictionary) const {
    if (requestsDictionary.contains("requests")) {
        if (requestsDictionary["requests"].size() < 0 && requestsDictionary["requests"].size() > 1000) {
            throw std::invalid_argument(ERROR_REQUESTS_COUNT); 
        }        
    }else {
        throw std::invalid_argument(ERROR_CONFIG_EMPTY); 
    }
    return true;
}


std::vector<std::string> ConverterJSON::getTextDocuments(){
    textDocuments.clear();
    for (const auto& doc : resourcesPaths) {
        std::ifstream docReadingStream(doc);
        if (docReadingStream.is_open()) {
            std::string buffer;
            while (!docReadingStream.eof()) {
                std::string b;
                docReadingStream >> b;
                buffer += b;
                buffer += " ";
            }
            textDocuments.push_back(buffer);
            docReadingStream.close();
        }
        else {
            std::cerr << "File content reading:\t- file not found error " + doc << "\n";
        }
    }
    std::cout << "Input docs read success: " << resourcesPaths.size() << " files\n";
    return textDocuments;
}

int ConverterJSON::getResponsesLimit() const {
    return maxResponses;
}

std::vector<std::string> ConverterJSON::getRequests() const {
    return requests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>>& answers) const {
    nlohmann::json answersDict;
    if (!answers.empty()) {
        std::ofstream answersFile(ANSWERS_FILE_PATH, std::ios_base::trunc);
        if (answersFile.is_open()) {
            int requestCount{ 0 };
            nlohmann::json answerDictionary;
            for (auto request : answers) {
                answersDict["answers"]["request" + std::to_string(requestCount)]["result"] = !request.empty();
                if (request.size() == 1) {
                    answersDict["answers"]["request" + std::to_string(requestCount)]["docid"] = request[0].first;
                    answersDict["answers"]["request" + std::to_string(requestCount)]["rank"] = request[0].second;
                }
                else {
                    auto relevance_array = nlohmann::json::array();
                    int relevanceID{ 0 };
                    for (auto relevance : request) {
                        ++relevanceID;
                        if (relevanceID > maxResponses) break;

                        auto relevance_member = nlohmann::json::object();
                        relevance_member["docid"] = relevance.first;
                        relevance_member["rank"] = relevance.second;
                        relevance_array.push_back(relevance_member);
                    }
                    answersDict["answers"]["request" + std::to_string(requestCount)]["relevance"] = relevance_array;
                }
                ++requestCount;
            }
            answersFile << answersDict;
            answersFile.close();
            std::cout << ANSWERS_COMPLITE;
        }
        else {
            std::cerr << FILE_ANSWERS_NOT_FOUND + ANSWERS_FILE_PATH << "\n";
        }
    }
    else {
        std::cerr << ANSWERS_EMPTY;
    }
}

void ConverterJSON::readConfigFile() {
std::ifstream configFile(CONFIG_FILE_PATH);
    if (configFile.is_open()) {
        nlohmann::json configDictionary;
        configFile >> configDictionary;
        if(invalidConfig(configDictionary)){
            applicationName = configDictionary["config"]["name"];
            applicationVersion = configDictionary["config"]["version"];
            maxResponses = configDictionary["config"]["max_responses"];
            resourcesPaths.clear();
            for (auto f : configDictionary["files"]) {
                resourcesPaths.push_back(f);
            }
            std::cout << FILE_CONFIG_FOUND;
            std::cout << "Name: " << applicationName << "\n";
            std::cout << "Version: " << applicationVersion << "\n";
            std::cout << "Maximum number of responses: " << maxResponses << "\n";
            std::cout << "Files to search: " << resourcesPaths.size() << "\n";
            configFile.close();
        }
    }
    else {
        throw std::invalid_argument( ERROR_FILE_CONFIG_NOT_FOUND + CONFIG_FILE_PATH );
    }
}

void ConverterJSON::readRequestFile() {
    std::ifstream configFile (REQUESTS_FILE_PATH);
    if (configFile.is_open()) {
        nlohmann::json requestsDictionary;
        configFile >> requestsDictionary;
        if (inValidReuests(requestsDictionary)) {
            requests.clear();
            for (auto f : requestsDictionary["requests"]) {
                requests.push_back(f);
            }
            configFile.close();        
            std::cout << FILE_REQUESTS_FOUND << requests.size() << "\n";
        }
    }
    else {
        throw std::invalid_argument(ERROR_FILE_REQUESTS_NOT_FOUND + REQUESTS_FILE_PATH); 
    }
}


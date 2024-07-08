#include "ConverterJSON.hpp"

const int LINE_LENGTH = 46;
const int HEADER_SPACER = 15;

//static members initialization -------------------------
ConverterJSON* ConverterJSON::instance = nullptr;
ConverterJSON* ConverterJSON::getInstance()
{
    if (instance == nullptr)
    {
        instance = new ConverterJSON();
    }
    return instance;
}
//---------------------------------------------------------

std::vector<std::string> ConverterJSON::getTextDocuments()
{
    textDocuments.clear();
    for (const auto& doc : resourcesPaths)
    {
        std::ifstream docReadingStream(doc);
        if (docReadingStream.is_open())
        {
            std::string buffer;
            while (!docReadingStream.eof())
            {
                std::string b;
                docReadingStream >> b;
                buffer += b;
                buffer += " ";
            }
            textDocuments.push_back(buffer);
            docReadingStream.close();
        }
        else
        {
            std::cerr << "File content reading:\t- file not found error " + doc << "\n";
        }
    }
    std::cout << "Input docs read success: " << resourcesPaths.size() << " files\n";
    return textDocuments;
}

int ConverterJSON::getResponsesLimit() const
{
    return maxResponses;
}

std::vector<std::string> ConverterJSON::getRequests()
{
    return requests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
{
    nlohmann::json answersDict;
    if (!answers.empty())
    {
        std::cout << "Answers pushing... ";
        std::ofstream answersFile(ANSWERS_FILE_PATH, std::ios_base::trunc);
        if (answersFile.is_open())
        {
            int requestCount{ 0 };
            nlohmann::json answerDictionary;
            for (auto request : answers)
            {
                answersDict["answers"]["request" + std::to_string(requestCount)]["result"] = !request.empty();
                if (request.size() == 1)
                {
                    answersDict["answers"]["request" + std::to_string(requestCount)]["docid"] = request[0].first;
                    answersDict["answers"]["request" + std::to_string(requestCount)]["rank"] = request[0].second;
                }
                else
                {
                    auto relevance_array = nlohmann::json::array();
                    int relevanceID{ 0 };
                    for (auto relevance : request)
                    {
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
            std::cout << "done\n";
        }
        else
        {
            std::cout << "\t error - file not found: " + ANSWERS_FILE_PATH << "\n";
        }
    }
    else
    {
        std::cout << "No answers to push.\n";
    }
}

void ConverterJSON::readConfigFile(std::string path)
{
    std::ifstream configFile(path);
    if (configFile.is_open())
    {
        nlohmann::json configDictionary;
        configFile >> configDictionary;
        applicationName = configDictionary["config"]["name"];
        applicationVersion = configDictionary["config"]["version"];
        maxResponses = configDictionary["config"]["max_responses"];
        resourcesPaths.clear();
        for (auto f : configDictionary["files"])
        {
            resourcesPaths.push_back(f);
        }

        for (int i = 0; i < HEADER_SPACER; ++i) { std::cout << "="; }
        std::cout << "[Initialization]";
        for (int i = 0; i < HEADER_SPACER; ++i) { std::cout << "="; }
        std::cout << "\n" << applicationName << "\n";
        std::cout << "Version: " << applicationVersion << "\n";
        std::cout << "Max responses per request: " << maxResponses << "\n";
        std::cout << "Files library: " << resourcesPaths.size() << "\n";
        for (int i = 0; i < LINE_LENGTH; ++i) { std::cout << "-"; }
        std::cout << "\n";
        configFile.close();
    }
    else
    {
        std::cerr << "\t - file not found error: " + path << "\n";
    }
}

void ConverterJSON::readRequestFile(std::string path)
{
    std::cout << "Requests reading: ";
    std::ifstream configFile(path);
    if (configFile.is_open())
    {
        nlohmann::json requestsDictionary;
        configFile >> requestsDictionary;
        requests.clear();
        for (auto f : requestsDictionary["requests"])
        {
            requests.push_back(f);
        }
        configFile.close();
        std::string requestOrRequests = requests.size() == 1 ? " request is " : " requests are ";
        std::cout << requests.size() << requestOrRequests << "found\n";
    }
    else
    {
        std::cerr << "\t - file not found error: " + path << "\n";
    }
}

int ConverterJSON::getMaxResponses() const {
    return maxResponses;
}
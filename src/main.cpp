#include <iostream>
#include "ConverterJSON.hpp"
#include "InvertedIndex.hpp"
#include "SearchServer.hpp"

/**
 * Preparing the result for recording.
 */
std::vector<std::vector<std::pair<int, float>>> preparingAnswers(const std::vector<std::vector<RelativeIndex>>& allRequestsResults){
    std::vector<std::vector<std::pair<int, float>>> allRequestsResultsReadyForJSON;
    for (auto& requestResult : allRequestsResults)
    {
        std::vector<std::pair<int, float>> requestResultReadyForJSON;
        for (auto& pageRelevance : requestResult) {
            std::pair<int, float> relevancePair;
            relevancePair.first = (int)pageRelevance.doc_id;
            relevancePair.second = pageRelevance.rank;
            requestResultReadyForJSON.push_back(relevancePair);
        }
        allRequestsResultsReadyForJSON.push_back(requestResultReadyForJSON);
    }
    return allRequestsResultsReadyForJSON;
}

int main()
{
    //Initialization:    
    ConverterJSON::getInstance()->readConfigFile();
    ConverterJSON::getInstance()->readRequestFile();    
    std::vector<std::string> documents = ConverterJSON::getInstance()->getTextDocuments();
    auto* invertedIndex = new InvertedIndex();
    invertedIndex->updateDocumentBase(documents);

    //Search:
    std::cout << "Searching...\n";
    SearchServer searchServer(*invertedIndex);
    searchServer.setMaxResponses(ConverterJSON::getInstance()->getMaxResponses());    
    std::vector<std::vector<RelativeIndex>> allRequestsResults = searchServer.search(ConverterJSON::getInstance()->getRequests());
    ConverterJSON::getInstance()->putAnswers(preparingAnswers(allRequestsResults));
    system("pause");
    return 0;
}
#include <iostream>
#include "ConverterJSON.hpp"
#include "InvertedIndex.hpp"
#include "SearchServer.hpp"

int main()
{
    //Initialization:
    ConverterJSON converterJSON;
    converterJSON.readConfigFile();
    converterJSON.readRequestFile();    
    InvertedIndex invertedIndex;
    invertedIndex.updateDocumentBase(converterJSON.getTextDocuments());
    
    //Search:
    std::cout << "Searching...\n";
    SearchServer searchServer(invertedIndex);
    searchServer.setMaxResponses(converterJSON.getResponsesLimit());
    auto allRequestsResults = searchServer.search(converterJSON.getRequests());
    converterJSON.putAnswers(allRequestsResults);
    system("pause");
    return 0;
}
#include <iostream>
#include "ConverterJSON.hpp"
#include "InvertedIndex.hpp"
#include "SearchServer.hpp"
#include "ThreadPool.hpp"

int main()
{
    //Initialization:
    ConverterJSON converterJSON;
    InvertedIndex invertedIndex;
    try {
        converterJSON.readConfigFile();
        converterJSON.readRequestFile();
        invertedIndex.updateDocumentBase(converterJSON.getTextDocuments());
    }
    catch(const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << '\n';
    }
    
    
    SearchServer searchServer(invertedIndex);
    searchServer.setMaxResponses(converterJSON.getResponsesLimit());
    //Search:
    std::cout << "Searching...\n";
    try {
        auto allRequestsResults = searchServer.search(converterJSON.getRequests());
        converterJSON.putAnswers(allRequestsResults);
    }
    catch(const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << '\n';
    }

    system("pause");
    return 0;
}
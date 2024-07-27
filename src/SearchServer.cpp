#include "SearchServer.hpp"

size_t SearchServer::getAbsoluteRelevanceForDocument(size_t docId, std::set<std::string>& uniqueWords) {
    size_t absoluteRelevance{ 0 };
    for (const auto& word : uniqueWords) {
        size_t wordCountInDoc = _index.getWordCountInDoc(word, docId);
        absoluteRelevance += wordCountInDoc;
    }
    return absoluteRelevance;
}

std::vector<std::vector<std::pair<int, float>>> SearchServer::search(const std::vector<std::string>& queries_input){
    if (queries_input.empty()) {
        throw std::invalid_argument(ERROR_REQUESTS_EMPTY);
    }
    std::vector<std::vector<RelativeIndex>> resultRelativeIndexes{};

    for (const auto& query : queries_input) {
        // Get unique words from query
        std::set<std::string> uniqueWords;

        std::istringstream ist(query);
        for (std::string word; ist >> word; ) {
        //Convert symbols to lower case:
        std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { return std::tolower(c); });
        uniqueWords.emplace(word);
        }
        if (uniqueWords.empty()) {
            std::cerr << ERROR_BAD_REQUEST;
            continue;
        }

        // Get the entry count for each word
        std::vector<std::pair<std::string, size_t>> wordsEntries;
        for (const auto& word : uniqueWords) {
            auto wordEntries = _index.getWordCount(word);
            size_t wordEntriesSum = 0;
            for (auto wordEntry : wordEntries) {
                wordEntriesSum += wordEntry.count;
            }
            std::pair<std::string, size_t> wordAndEntry;
            wordAndEntry.first = word;
            wordAndEntry.second = wordEntriesSum;
            wordsEntries.push_back(wordAndEntry);
        }

        // Sort unique words according to entry count in ascending direction
        std::sort(wordsEntries.begin(), wordsEntries.end(), [](auto& left, auto& right) {
            return left.second < right.second;
        });

        // Get the document list of documents
        std::vector<size_t> documentIds{};
        // Getting entries and docIds:
        for (const auto& wordAndEntry : wordsEntries) {
            auto entries = _index.getWordCount(wordAndEntry.first);
            for (auto entry : entries) {
                documentIds.push_back(entry.doc_id);
            }
        }
        // Getting unique ids from docIds:
        std::set<size_t> uniqueDocIds(documentIds.begin(), documentIds.end());
        documentIds.clear();
        documentIds.assign(uniqueDocIds.begin(), uniqueDocIds.end());
        std::sort(documentIds.begin(), documentIds.end(), std::less<size_t>());

        // Get absolute relevance and maximal relevance:
        std::vector<RelativeIndex>* relativeIndexes = new std::vector<RelativeIndex>();
        size_t maxAbsoluteRelevance{ 0 };
        for (const auto& docId : documentIds) {
            size_t absoluteRelevance = getAbsoluteRelevanceForDocument(docId, uniqueWords);
            auto* relativeIndex = new RelativeIndex();
            relativeIndex->doc_id = docId;
            relativeIndex->absoluteIndex = absoluteRelevance;
            relativeIndexes->push_back(*relativeIndex);
            delete relativeIndex;
            if (absoluteRelevance > maxAbsoluteRelevance) maxAbsoluteRelevance = absoluteRelevance;
        }

        // Get relative relevance for each document:
        for (auto& relativeIndex : *relativeIndexes) {
            if (maxAbsoluteRelevance != 0) {
                float rank = (float)relativeIndex.absoluteIndex / (float)maxAbsoluteRelevance;
                int rounded = (int)std::round(rank * 100);
                rank = (float)rounded / 100;
                relativeIndex.rank = rank;
            }
            else relativeIndex.rank = 0;
        }

        // Sort the documents according to relevance (descending):
        std::sort(relativeIndexes->begin(), relativeIndexes->end(), [&relativeIndexes](RelativeIndex& left, RelativeIndex& right) {
                return (left.rank > right.rank || (left.rank == right.rank && left.doc_id < right.doc_id));
            });

        //Cut the result according to maxResponsesCount from InvertedIndex:
        if (relativeIndexes->size() > maxResponses) {
            relativeIndexes->erase(relativeIndexes->begin() + maxResponses, relativeIndexes->end());
        }
        // Push this vector to the result:
        resultRelativeIndexes.push_back(*relativeIndexes);
        delete relativeIndexes;
    }

    /**
     * Preparing the result for recording.
     */
    std::vector<std::vector<std::pair<int, float>>> result{};
    for (auto& requestResult : resultRelativeIndexes)
    {
        std::vector<std::pair<int, float>> requestResultReadyForJSON;
        for (auto& pageRelevance : requestResult) {
            std::pair<int, float> relevancePair;
            relevancePair.first = (int)pageRelevance.doc_id;
            relevancePair.second = pageRelevance.rank;
            requestResultReadyForJSON.push_back(relevancePair);
        }
        result.push_back(requestResultReadyForJSON);
    }
    return result;
}

void SearchServer::setMaxResponses(const int& newMaxResponses){
    this->maxResponses = newMaxResponses;
}

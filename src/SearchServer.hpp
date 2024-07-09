#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cmath>

#include "InvertedIndex.hpp"

struct RelativeIndex
{
    size_t doc_id;
    float rank;
    size_t absoluteIndex = 0; // Added for convenience.
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
    bool operator > (const RelativeIndex& other) const {
        return (rank > other.rank || (rank == other.rank && doc_id < other.doc_id));
    }
};

class SearchServer
{
private:
    InvertedIndex _index;
    int maxResponses{ 5 };
    /**
     * Getting unique words from request line
     * @param [in] request - separate string from request.json
     * @return set of unique words
     */
    std::set<std::string> getUniqueWords(const std::string& request);

    /**
     * Get the vector of entries for words set
     * @param [in] words - set of words
     * @return vector of entries
     */
    std::vector<std::pair<std::string, size_t>> getWordsEntries(const std::set<std::string>& words);

    /**
     * Sort the entries vector in ascending direction of entries count
     * @param [in/out] wordsEntries - entries vector
     */
    void sortWordsAscendingToEntries(std::vector<std::pair<std::string, size_t>>& wordsEntries);

    /**
     * Get the vector of documents where all words from request can be found
     * @param [in] words - vector of pairs "word - entry count"
     * @return the vector of document ids where all words can be found 
     */
    std::vector<size_t> getAllDocumentsWithWords(const std::vector<std::pair<std::string, size_t>>& words);

    /**
     * Calculate absolute relevance of the document for certain words
     * @param [in] docId - document id
     * @param [in] uniqueWords - request words vector
     * @return absolute relevance
     */
    size_t getAbsoluteRelevanceForDocument(size_t docId, std::set<std::string>& uniqueWords);

public:
    /**
    * @param idx - link to the InvertedIndex instance to 
    * let SearchServer know the word frequency in files.
    */
    SearchServer(InvertedIndex& idx) : _index(idx) { };

    /**
    * Process the search request
    * @param [in] queries_input - search requests from requests.json 
    * @return the sorted list of relevant answers 
    */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

    /**
    * Set maximal responses quantity from Search Server
    * @param [in] newMaxResponses - new value for maximal responses quantity
    */
    void setMaxResponses(const int& newMaxResponses);
};
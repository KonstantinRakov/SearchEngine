#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

struct Entry
{
    size_t doc_id, count;

    // Required for tests:
    bool operator ==(const Entry& other) const {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex
{
private:
    /**
     * Perform the indexing of the separate file
     * @param [in] fileContent - std::string with file content
     * @param [in] docId - id of the file
     */
    void indexTheFile(const std::string& fileContent, size_t docId);

    std::map<int, std::string> document_list; // map of documents (unique id and file name) for search in
    static std::mutex mutexIndexMap; // access to index map
    std::map<std::string, std::vector<Entry>> frequencyDictionary; // frequency dictionary for all files
    bool indexingIsOngoing;

public:

    InvertedIndex() = default;
    
    /**
    * Update or fill the documents base in which search will be performed
    * @param [in] texts_input - documents content
    */
    void updateDocumentBase(const std::vector<std::string>& input_docs);

    /**
    * Get the count of \'word\' entries in ready documents base
    * @param [in] word - word for which the entry frequency should be detected
    * @return vector of words with calculated entry frequency
    */
    std::vector<Entry> getWordCount(const std::string& word);

    /**
     * Get the word count in certain document
     * @param [in] word - word to counted in a document
     * @param [in] doc_id - document id for search
     * @return count of the word in certain document
     */
    size_t getWordCountInDoc(const std::string& word, const size_t doc_id) const; // ADDITIONAL METHOD

};
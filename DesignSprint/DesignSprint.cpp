/*

// Design Sprint  - Parallel Class
// Name Student: Minh Nguyen
// Student ID: 8723388

// Explain the method using

*/

// Include Libraries
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

// Test For file size
        // Test Input Files
        const std::string inputfile = "Large.txt";
        const std::string inputfile_small = "Hamlet.txt";
        const std::string inputfile_medium = "Hamlet.txt";
        const std::string inputfile_large = "Hamlet.txt";

// Define the variables input to test for each design performances

// Test Case I: Pattern.size() || numThreads > hardware::numthreads
        // Test Case 1: Pattern.size() = numThreads < hardware::numthreads
        std::vector<std::string> patterns6 = { "Horatio", "and", "Hamlet", "God"};
        const int numThreads6 = 4;
        //
        // Case Test 2 Pattern.size() > numThreads < hardware::numthreads
        std::vector<std::string> patterns1 = { "Horatio", "and", "Hamlet", "God", "question", "To", "sleep", "death", "King","or"};
        const int numThreads2 = 3;
        //
        // Test Case 3: Pattern.size() < numThreads < hardware::numthreads
        std::vector<std::string> patterns3 = { "Horatio", "and"};
        const int numThreads3 = 8;

// Test Case II: Pattern.size() || numThreads > hardware::numthreads
        // Test Case 4: Pattern.size(14) = numThreads > hardware::numthreads
        std::vector<std::string> patterns4 = { "Horatio", "and", "Hamlet", "God","question", "To", "sleep", "death", "King", "or", "nothing", "eyes", "speak","myself"};
        const int numThreads4 = 14;
        //
        // Case Test 5 Pattern.size(24) > numThreads > hardware::numthreads 
        std::vector<std::string> patterns5 = { "Horatio", "and", "Hamlet", "God", "question", "To", "sleep", "death", "King", "or", "that", "nothing", "eyes", "speak", "myself", "cock", "of", "Denmark", "ourselves", "funeral", "dirge", "marriage", "son", "him"};
        const int numThreads5 = 28;
        //Denmark,ourselves,funeral,dirge,marriage
        // Test Case 6: Pattern.size() < numThreads > hardware::numthreads
        std::vector<std::string> patterns = { "Horatio", "and", "Hamlet", "God", "question", "To", "sleep", "death", "King", "or", "that", "nothing", "eyes", "speak", "myself", "cock", "of", "Denmark", "ourselves", "funeral", "dirge", "marriage", "son", "him", "will", "again"};
        const int numThreads = 25;

//
// Define test mode to test for performance
        // ** Note ** : const bool MISD the Numthread always = patterns sizes
        std::vector<std::string> Design = { "SISD", "SIMD", "MISD", "MIMD"};
        const bool SISD = true;
        const bool SIMD = false;
        const bool MISD = false;
        const bool MIMD = false;
//

// Function to stream large file using type BufIt defined type alias as a char type to iterator over all character in ifstream in and using rdbuf() function to read the alias type
std::string Reading_Files(std::string const& filename) {
    using BufIt = std::istreambuf_iterator<char>;
    std::ifstream in(filename);
    return std::string(BufIt(in.rdbuf()), BufIt());
}

int countWords_SISD(const std::string& text) {
    std::istringstream iss(text);
    std::string word;
    int localCount = 0;
    while (iss >> word) {
        ++localCount;
    }
    return localCount;
}

void search_SISD(const std::string& text, const std::string& pattern, int& result) {
    int count = 0;
    int length_pattern = pattern.size();
    int lenth_text_input = text.size();

    std::vector<int> bad_char(256, -1);

    for (int j = 0; j < length_pattern; j++) {
        bad_char[static_cast<int>(pattern[j])] = j;
    }
    int shift_value = 0;
    while (shift_value <= lenth_text_input - length_pattern) {
        int j = length_pattern - 1;
        while (j >= 0 && pattern[j] == text[shift_value + j])
            --j;

        if (j < 0) {
            ++count;
            shift_value += (shift_value + length_pattern < lenth_text_input) ? length_pattern - bad_char[text[shift_value + length_pattern]] : 1;
        }
        else {
            shift_value += std::max(1, j - bad_char[text[shift_value + j]]);
        }
    }
    result = count;
}

// Function to calculate for all the words in the text and all the times specific words occured inside the text 
void countWordsInSegment_SIMD(const std::string& text, const std::vector<std::string>& patterns,
    int& result_count_all_words_in_segment, int*& result_count_for_each_pattern, int patternSize) {

    std::istringstream iss(text);
    std::string word;
    int localCount = 0;

    // using iss >> word to fast loop through all words
    while (iss >> word) {
        ++localCount;
    }
    
    // Store the number of total word inside this specific chunk text
    result_count_all_words_in_segment = localCount;

    // Allocated memory for the list to stores all the results is the count of times specific word in specific patterns list occured inside the text
    result_count_for_each_pattern = new int[patternSize];

    for (size_t i = 0; i < patterns.size(); i++) {
        int count = 0;
        int length_pattern = patterns[i].size();
        int lenth_text_input = text.size();

        std::vector<int> bad_char(256, -1);

        for (int j = 0; j < length_pattern; j++) {
            bad_char[static_cast<int>(patterns[i][j])] = j;
        }

        int shift_value = 0;
        while (shift_value <= lenth_text_input - length_pattern) {
            int j = length_pattern - 1;
            while (j >= 0 && patterns[i][j] == text[shift_value + j])
                --j;

            if (j < 0) {
                ++count;
                shift_value += (shift_value + length_pattern < lenth_text_input) ? length_pattern - bad_char[text[shift_value + length_pattern]] : 1;
            }
            else {
                shift_value += std::max(1, j - bad_char[text[shift_value + j]]);
            }
        }
        result_count_for_each_pattern[i] = count;
    }
}

void countWordsInSegment_MIMD(const std::string& text, const std::vector<std::string>& patterns,
    int& result_count_all_words_in_segment, int*& result_count_for_each_pattern, int patternSize) {

    std::istringstream iss(text);
    std::string word;
    int localCount = 0;

    // using iss >> word to fast loop through all words
    while (iss >> word) {
        ++localCount;
    }

    // Store the number of total words inside this specific chunk text
    result_count_all_words_in_segment = localCount;

    // Allocate memory for the list to store all the results is the count of times a specific word in specific patterns list occurred inside the text
    result_count_for_each_pattern = new int[patternSize];

    for (size_t i = 0; i < patterns.size(); i++) {
        int count = 0;
        int length_pattern = patterns[i].size();
        int length_text_input = text.size();

        std::vector<int> bad_char(256, -1);

        for (int j = 0; j < length_pattern; j++) {
            bad_char[static_cast<int>(patterns[i][j])] = j;
        }

        int shift_value = 0;
        while (shift_value <= length_text_input - length_pattern) {
            int j = length_pattern - 1;
            while (j >= 0 && patterns[i][j] == text[shift_value + j])
                --j;

            if (j < 0) {
                ++count;
                shift_value += (shift_value + length_pattern < length_text_input) ? length_pattern - bad_char[text[shift_value + length_pattern]] : 1;
            }
            else {
                shift_value += std::max(1, j - bad_char[text[shift_value + j]]);
            }
        }
        result_count_for_each_pattern[i] = count;
    }
}

int main()
{    
    std::string text = Reading_Files(inputfile);
    auto startTime = std::chrono::high_resolution_clock::now();

    if (SISD)
    {
        std::cout << "Start Process SISD Design ..." << std::endl;
        int totalwords = countWords_SISD(text);
        int result = 0;
        for (const std::string& pattern : patterns) {
            search_SISD(text, pattern, result);
            std::cout << result  << " : " << pattern << std::endl;
        }
        std::cout << "Total word count: " << totalwords << " words" << std::endl;
        std::cout << "End Process SISD Design" << std::endl;
    }
    if (SIMD)
    {
        std::cout << "Start Process SIMD Design ..." << std::endl;
        // Defined the list of threads and the results and the string input need to process and need to collect the results to recalculate later
        std::vector<std::thread> threads(numThreads);
        std::string* chunk_char_each_threads = new std::string[numThreads];
        int* results_count_all_words_in_segment = new int[numThreads]();
        int** results_count_for_each_pattern = new int* [numThreads];
        int totalCount = 0;
        int sizepatterns = patterns.size();


        // Allocate memory for the results count for each words have size equal to patterns.size() store the results in 2D array
        for (int i = 0; i < numThreads; i++) {
            results_count_for_each_pattern[i] = new int[sizepatterns];
        }

        // Loop through all the thread to split the original text to each chunks
        for (int i = 0; i < numThreads; i++) {
            // Divide the text into segments and check if there is remainder in the text input
            int segmentSize = text.size() / numThreads;
            int remainder = text.size() % numThreads;

            int start = i * (text.size() / numThreads);
            int end = (i == numThreads - 1) ? text.size() : (start + (text.size() / numThreads));
            if (i == numThreads - 1 && remainder > 0) {
                end += remainder;
            }
            // Store back input string for each chunk to the string array
            chunk_char_each_threads[i] = text.substr(start, end - start);
        }
        // Loop though all the threads to assign the task with their specific input - results location to write/read
        for (int i = 0; i < numThreads; i++) {
            threads[i] = std::thread([&, i]() {
                countWordsInSegment_SIMD(chunk_char_each_threads[i], patterns, results_count_all_words_in_segment[i], results_count_for_each_pattern[i], sizepatterns);
                });
        }
        // Loop though all the threads to call join method to make main threads wait for all threads finish their works
        for (auto& thread : threads) {
            thread.join();
        }

        // Loop though all the results to sum up for total words are counted inside each threads in results_count_all_words_in_segment int*
        for (int i = 0; i < numThreads; i++) {
            totalCount += results_count_all_words_in_segment[i];
        }

        // Loop though all the results corresponding to the pattern to sum up for total times that words are occured are counted inside each threads in results_count_for_each_pattern int**
        for (size_t i = 0; i < patterns.size(); i++) {
            int patternTotalCount = 0;
            for (int j = 0; j < numThreads; j++) {
                patternTotalCount += results_count_for_each_pattern[j][i];
            }
            std::cout << patternTotalCount << ": " << patterns[i] << std::endl;
        }
        std::cout << "Total word count: " << totalCount << " words" << std::endl;

        // Delete all the memory allocated before to avoid memory leak
        delete[] results_count_all_words_in_segment;
        for (int i = 0; i < numThreads; i++) {
            delete[] results_count_for_each_pattern[i];
        }
        delete[] results_count_for_each_pattern;
        delete[] chunk_char_each_threads;
        std::cout << "End Process SIMD Design" << std::endl;
    }
    if (MISD)
    {
        std::cout << "Start Process MISD Design ..." << std::endl;
        int totalwords = countWords_SISD(text);
        std::vector<int> results(patterns.size(), 0);
        std::vector<std::thread> threads;

        // Iterate through each pattern and create a thread for each search for MISD
        for (std::size_t i = 0; i < patterns.size(); ++i) {
            threads.emplace_back(search_SISD, std::ref(text), std::ref(patterns[i]), std::ref(results[i]));
        }
        for (auto& thread : threads) {
            thread.join();
        }
        for (std::size_t i = 0; i < patterns.size(); ++i) {
            std::cout  << results[i] << " : " << patterns[i] << std::endl;
        }

        std::cout << "Total word count: " << totalwords << " words" << std::endl;
        std::cout << "End Process MISD Design" << std::endl;
    }
    if (MIMD)
    {
        std::cout << "Start Process MIMD Design ..." << std::endl;
        // Defined the list of threads and the results
        std::vector<std::thread> threads(numThreads);
        std::vector<std::string> chunk_char_each_threads(numThreads);
        std::vector<int> results_count_all_words_in_segment(numThreads);
        std::vector<int*> results_count_for_each_pattern(numThreads);
        int totalCount = 0;
        int sizepatterns = patterns.size();

        // Loop through all the threads to split the original text into chunks corresponding to the number of threads
        for (int i = 0; i < numThreads; i++) {
            int start = i * (text.size() / numThreads);
            int end = (i == numThreads - 1) ? text.size() : (start + (text.size() / numThreads));
            chunk_char_each_threads[i] = text.substr(start, end - start);
        }

        // Loop through all the threads to assign the task with their specific input - results location to write/read
        for (int i = 0; i < numThreads; i++) {
            threads[i] = std::thread([&, i]() {
                results_count_for_each_pattern[i] = new int[sizepatterns];
                countWordsInSegment_MIMD(chunk_char_each_threads[i], patterns,
                    results_count_all_words_in_segment[i],
                    results_count_for_each_pattern[i], sizepatterns);
                });
        }

        // Loop through all the threads to call join method to make the main thread wait for all threads to finish their works
        for (auto& thread : threads) {
            thread.join();
        }

        // Loop through all the results to sum up for total words counted inside each thread in results_count_all_words_in_segment int*
        for (int i = 0; i < numThreads; i++) {
            totalCount += results_count_all_words_in_segment[i];
        }

        // Loop through all the results corresponding to the pattern to sum up for total times that words are occurred are counted inside each thread in results_count_for_each_pattern int**
        for (size_t i = 0; i < patterns.size(); i++) {
            int patternTotalCount = 0;
            for (int j = 0; j < numThreads; j++) {
                patternTotalCount += results_count_for_each_pattern[j][i];
            }
            std::cout << patternTotalCount << ": " << patterns[i] << std::endl;
        }
        std::cout << "Total word count: " << totalCount << " words" << std::endl;

        // Delete allocated memory
        for (int i = 0; i < numThreads; i++) {
            delete[] results_count_for_each_pattern[i];
        }

        std::cout << "End Process MIMD Design" << std::endl;
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalTime = endTime - startTime;
    std::cout << "Total time: " << totalTime.count() << " seconds" << std::endl;

    return 0;
}

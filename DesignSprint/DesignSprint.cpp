/*

// Design Sprint  - Parallel Class
// Name Student: Minh Nguyen
// Student ID: 8723388

// Explain the method using

*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

// Define the variables input
std::vector<std::string> patterns = { "Horatio", "and", "Hamlet", "God"};
const int numThreads = 4;
const std::string inputfile = "Hamlet.txt";

// Function to stream large file using type BufIt defined type alias as a char type to iterator over all character in ifstream in and using rdbuf() function to read the alias type
std::string Reading_Files(std::string const& filename) {
    using BufIt = std::istreambuf_iterator<char>;
    std::ifstream in(filename);
    return std::string(BufIt(in.rdbuf()), BufIt());
}

// Function to calculate for all the words in the text and all the times specific words occured inside the text 
void countWordsInSegment(const std::string& text, const std::vector<std::string>& patterns,
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

int main() {
    
    // Taking the input
    std::string text = Reading_Files(inputfile);

    // Start the clock
    auto startTime = std::chrono::high_resolution_clock::now();

    // Defined the list of threads and the results and the string input need to process and need to collect the results to recalculate later
    std::vector<std::thread> threads(numThreads);
    std::string* chunk_char_each_threads = new std::string[numThreads];
    int* results_count_all_words_in_segment = new int[numThreads]();
    int** results_count_for_each_pattern = new int* [numThreads];
    int totalCount = 0;

    // Allocate memory for the results count for each words have size equal to patterns.size() store the results in 2D array
    for (int i = 0; i < numThreads; i++) {
        results_count_for_each_pattern[i] = new int[patterns.size()]();
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
        threads[i] = std::thread(countWordsInSegment,chunk_char_each_threads[i], patterns, results_count_all_words_in_segment[i], results_count_for_each_pattern[i], patterns.size());
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
        std::cout << "Occurrences of pattern '" << patterns[i] << "': " << patternTotalCount << std::endl;
    }
    std::cout << "Total word count: " << totalCount << std::endl;

    // Stop the clock and print out results time
    auto endTime = std::chrono::high_resolution_clock::now();
    std::cout << "Total time: " << (endTime - startTime).count() << " seconds" << std::endl;

    // Delete all the memory allocated before to avoid memory leak
    delete[] results_count_all_words_in_segment;
    for (int i = 0; i < numThreads; i++) {
        delete[] results_count_for_each_pattern[i];
    }
    delete[] results_count_for_each_pattern;
    delete[] chunk_char_each_threads;

    return 0;
}

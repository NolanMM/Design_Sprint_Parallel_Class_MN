#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

std::vector<std::string> patterns = { "Horatio", "and", "Hamlet", "God"};
const int numThreads = 4;
const std::string inputfile = "Hamlet.txt";

std::string Reading_Files(std::string const& filename) {
    using BufIt = std::istreambuf_iterator<char>;
    std::ifstream in(filename);
    return std::string(BufIt(in.rdbuf()), BufIt());
}

void countWordsInSegment(const std::string& text, const std::vector<std::string>& patterns,
    int& result_count_all_words_in_segment, int*& result_count_for_each_pattern, int patternSize) {
    std::istringstream iss(text);
    std::string word;
    int localCount = 0;
    while (iss >> word) {
        ++localCount;
    }
    result_count_all_words_in_segment = localCount;
    result_count_for_each_pattern = new int[patternSize];

    for (size_t i = 0; i < patterns.size(); ++i) {
        int count = 0;
        int m = patterns[i].size();
        int n = text.size();

        std::vector<int> bad_char(256, -1);

        for (int j = 0; j < m; ++j) {
            bad_char[static_cast<int>(patterns[i][j])] = j;
        }

        int s = 0;
        while (s <= n - m) {
            int j = m - 1;
            while (j >= 0 && patterns[i][j] == text[s + j])
                --j;

            if (j < 0) {
                ++count;
                s += (s + m < n) ? m - bad_char[text[s + m]] : 1;
            }
            else {
                s += std::max(1, j - bad_char[text[s + j]]);
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

    // Allocate memory for the results count for each words have size equal to patterns.size() store the results in 2D array
    for (int i = 0; i < numThreads; ++i) {
        results_count_for_each_pattern[i] = new int[patterns.size()]();
    }

    // Loop through all the thread to split the original text to each chunks
    for (int i = 0; i < numThreads; ++i) {
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
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = std::thread([&, i]() {
            countWordsInSegment(chunk_char_each_threads[i], patterns,
            results_count_all_words_in_segment[i],
            results_count_for_each_pattern[i], patterns.size());
            });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    int totalCount = 0;
    for (int i = 0; i < numThreads; ++i) {
        totalCount += results_count_all_words_in_segment[i];
    }

    std::cout << "Total word count: " << totalCount << std::endl;

    for (size_t i = 0; i < patterns.size(); ++i) {
        int patternTotalCount = 0;
        for (int j = 0; j < numThreads; ++j) {
            patternTotalCount += results_count_for_each_pattern[j][i];
        }
        std::cout << "Occurrences of pattern '" << patterns[i] << "': " << patternTotalCount << std::endl;
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalTime = endTime - startTime;
    std::cout << "Total time: " << totalTime.count() << " seconds" << std::endl;
    delete[] results_count_all_words_in_segment;
    for (int i = 0; i < numThreads; ++i) {
        delete[] results_count_for_each_pattern[i];
    }
    delete[] results_count_for_each_pattern;
    delete[] chunk_char_each_threads;
    return 0;
}

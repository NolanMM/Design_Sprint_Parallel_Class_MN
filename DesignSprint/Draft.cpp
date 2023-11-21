//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sstream>
//#include <unordered_map>
//#include <algorithm>
//#include <chrono>
//#include <thread>
//#include <vector>
//
//std::vector<std::string> patterns = { "Horatio", "and", "Hamlet", "God"};
//const int numThreads = 4;
//const std::string inputfile = "Hamlet.txt";
//
//std::string Reading_Files(std::string const& filename) {
//    using BufIt = std::istreambuf_iterator<char>;
//    std::ifstream in(filename);
//    return std::string(BufIt(in.rdbuf()), BufIt());
//}
//
//int countTotalWords(const std::string& text) {
//    std::istringstream iss(text);
//    std::string word;
//    int totalOccurrences = 0;
//    while (iss >> word) {
//        ++totalOccurrences;
//    }
//    return totalOccurrences;
//}
//
//void search_thread(const std::string& text, const std::string& pattern, int& result) {
//    int count = 0;
//    int m = pattern.size();
//    int n = text.size();
//
//    std::vector<int> bad_char(256, -1);
//
//    for (int i = 0; i < m; ++i) {
//        bad_char[static_cast<int>(pattern[i])] = i;
//    }
//
//    int s = 0;
//    while (s <= n - m) {
//        int j = m - 1;
//        while (j >= 0 && pattern[j] == text[s + j])
//            --j;
//
//        if (j < 0) {
//            ++count;
//            s += (s + m < n) ? m - bad_char[text[s + m]] : 1;
//        }
//        else {
//            s += std::max(1, j - bad_char[text[s + j]]);
//        }
//    }
//    result = count;
//}
//
//int main() {
//    auto text = Reading_Files(inputfile);
//    std::chrono::duration<double> parallel_time(0);
//    auto startTime = std::chrono::high_resolution_clock::now();
//    int totalWordCount = countTotalWords(text);
//
//    std::vector<int> results(patterns.size());
//
//    std::vector<std::thread> threads;
//    
//    size_t patternsPerThread = patterns.size() / numThreads;
//
//    for (int i = 0; i < numThreads; ++i) {
//        // Calculate the range of patterns for the current thread
//        size_t start = i * patternsPerThread;
//        size_t end = (i == numThreads - 1) ? patterns.size() : (i + 1) * patternsPerThread;
//
//        // Create a thread for the specified range of patterns
//        threads.emplace_back([&, start, end]() {
//            for (size_t j = start; j < end; ++j) {
//                search_thread(text, patterns[j], results[j]);
//            }
//            });
//    }
//
//    for (auto& thread : threads) {
//        thread.join();
//    }
//
//    for (size_t i = 0; i < patterns.size(); ++i) {
//        std::cout << results[i] << " " << patterns[i] << '\n';
//    }
//
//    std::cout << "Total number of words: " << totalWordCount << " occurrences\n";
//    parallel_time = std::chrono::high_resolution_clock::now() - startTime;
//    std::cout << "Time of the parallel algorithm: " << parallel_time.count() << " seconds" << std::endl;
//
//    return 0;
//}
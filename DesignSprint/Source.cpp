//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sstream>
//#include <chrono>
//#include <thread>
//#include <vector>
//#include <mutex>
//
//std::vector<std::string> patterns = { "Horatio", "and", "Hamlet", "God", "or", "King" };
//const int numThreads = 6;  // Specify the desired number of threads
//const std::string inputfile = "Hamlet.txt";
//
//std::string Reading_Files(std::string const& filename) {
//    using BufIt = std::istreambuf_iterator<char>;
//    std::ifstream in(filename);
//    return std::string(BufIt(in.rdbuf()), BufIt());
//}
//
//int countTotalWordsInRange(const std::string& text, size_t start, size_t end) {
//    std::istringstream iss(text.substr(start, end - start));
//    std::string word;
//    int totalOccurrences = 0;
//    while (iss >> word) {
//        ++totalOccurrences;
//    }
//    return totalOccurrences;
//}
//
//void countWordsThread(const std::string& text, size_t start, size_t end, int& result, std::mutex& mutex) {
//    int count = countTotalWordsInRange(text, start, end);
//
//    std::lock_guard<std::mutex> lock(mutex);
//    result += count;
//}
//
//void searchThread(const std::string& text, const std::string& pattern, int& result) {
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
//    int totalWordCount = 0;
//    std::mutex wordCountMutex;
//
//    std::vector<int> results(patterns.size());
//    std::vector<std::thread> threads;
//
//    // Calculate the number of characters each thread should handle
//    size_t charsPerThread = text.size() / numThreads;
//
//    auto startTime = std::chrono::high_resolution_clock::now();
//
//    // Create threads for word counting
//    for (int i = 0; i < numThreads; ++i) {
//        size_t start = i * charsPerThread;
//        size_t end = (i == numThreads - 1) ? text.size() : (i + 1) * charsPerThread;
//        threads.emplace_back(countWordsThread, std::ref(text), start, end, std::ref(totalWordCount), std::ref(wordCountMutex));
//    }
//
//    // Join threads for word counting
//    for (auto& thread : threads) {
//        thread.join();
//    }
//
//    threads.clear();  // Clear the threads vector for word search threads
//
//    // Create threads for pattern search
//    for (size_t i = 0; i < patterns.size(); ++i) {
//        threads.emplace_back(searchThread, std::ref(text), std::ref(patterns[i]), std::ref(results[i]));
//    }
//
//    // Join threads for pattern search
//    for (auto& thread : threads) {
//        thread.join();
//    }
//
//    // Print results
//    for (size_t i = 0; i < patterns.size(); ++i) {
//        std::cout << results[i] << " " << patterns[i] << '\n';
//    }
//
//    std::cout << "Total number of words: " << totalWordCount << " occurrences\n";
//
//    auto endTime = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<double> totalTime = endTime - startTime;
//    std::cout << "Total time: " << totalTime.count() << " seconds" << std::endl;
//
//    return 0;
//}

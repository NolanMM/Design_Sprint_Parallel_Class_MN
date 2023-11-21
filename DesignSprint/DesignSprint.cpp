//
//
//// Design Sprint 
//// Student Name: Minh Nguyen
//// Student ID: 8723388
////
//// Explain method using Single Data and multiple struction
//// Using Reading the file in binary for fast reading file
//
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sstream>
//#include <unordered_map>
//#include <algorithm>
//#include <chrono>
//#include <thread>
//
//std::string slurp(std::string const& filename) {
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
//    auto text = slurp("Hamlet.txt");
//    int totalOccurrences = countTotalWords(text);
//    std::cout << "Total number of words: " << totalOccurrences << " occurrences\n";
//    std::vector<std::string> patterns = { "Horatio", "and", "Hamlet", "God" };
//
//    std::vector<int> results(patterns.size());
//
//    std::vector<std::thread> threads;
//    std::chrono::duration<double> parallel_time(0);
//    auto startTime = std::chrono::high_resolution_clock::now();
//    for (size_t i = 0; i < patterns.size(); ++i) {
//        threads.emplace_back(search_thread, std::ref(text), std::ref(patterns[i]), std::ref(results[i]));
//    }
//
//    for (auto& thread : threads) {
//        thread.join();
//    }
//
//    int totalWordCount = 0;
//
//    for (size_t i = 0; i < patterns.size(); ++i) {
//        std::cout << results[i] << " " << patterns[i] << '\n';
//        totalWordCount += results[i];
//    }
//
//
//    std::cout << "Total word count = " << totalWordCount << '\n';
//    parallel_time = std::chrono::high_resolution_clock::now() - startTime;
//    std::cout << "Time of the parallel algorithm: " << parallel_time.count() << " seconds" << std::endl;
//
//    return 0;
//}

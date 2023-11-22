///*
//
//// Design Sprint  - Parallel Class
//// Name Student: Minh Nguyen
//// Student ID: 8723388
//
//// Explain the method using
//
//*/
//
//// Include Libraries
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sstream>
//#include <thread>
//#include <vector>
//
//void runTest(std::vector<std::string> patterns, int numThreads, std::string inputfile);
//// Function to stream large file using type BufIt defined type alias as a char type to iterator over all character in ifstream in and using rdbuf() function to read the alias type
//std::string Reading_Files(std::string const& filename) {
//    using BufIt = std::istreambuf_iterator<char>;
//    std::ifstream in(filename);
//    return std::string(BufIt(in.rdbuf()), BufIt());
//}
//
//int countWords_SISD(const std::string& text) {
//    std::istringstream iss(text);
//    std::string word;
//    int localCount = 0;
//    while (iss >> word) {
//        ++localCount;
//    }
//    return localCount;
//}
//
//void search_SISD(const std::string& text, const std::string& pattern, int& result) {
//    int count = 0;
//    int length_pattern = pattern.size();
//    int lenth_text_input = text.size();
//
//    std::vector<int> bad_char(256, -1);
//
//    for (int j = 0; j < length_pattern; j++) {
//        bad_char[static_cast<int>(pattern[j])] = j;
//    }
//    int shift_value = 0;
//    while (shift_value <= lenth_text_input - length_pattern) {
//        int j = length_pattern - 1;
//        while (j >= 0 && pattern[j] == text[shift_value + j])
//            --j;
//
//        if (j < 0) {
//            ++count;
//            shift_value += (shift_value + length_pattern < lenth_text_input) ? length_pattern - bad_char[text[shift_value + length_pattern]] : 1;
//        }
//        else {
//            shift_value += std::max(1, j - bad_char[text[shift_value + j]]);
//        }
//    }
//    result = count;
//}
//
//// Function to calculate for all the words in the text and all the times specific words occured inside the text 
//void countWordsInSegment_SIMD(const std::string& text, const std::vector<std::string>& patterns,
//    int& result_count_all_words_in_segment, int*& result_count_for_each_pattern, int patternSize) {
//
//    std::istringstream iss(text);
//    std::string word;
//    int localCount = 0;
//
//    // using iss >> word to fast loop through all words
//    while (iss >> word) {
//        ++localCount;
//    }
//
//    // Store the number of total word inside this specific chunk text
//    result_count_all_words_in_segment = localCount;
//
//    // Allocated memory for the list to stores all the results is the count of times specific word in specific patterns list occured inside the text
//    result_count_for_each_pattern = new int[patternSize];
//
//    for (size_t i = 0; i < patterns.size(); i++) {
//        int count = 0;
//        int length_pattern = patterns[i].size();
//        int lenth_text_input = text.size();
//
//        std::vector<int> bad_char(256, -1);
//
//        for (int j = 0; j < length_pattern; j++) {
//            bad_char[static_cast<int>(patterns[i][j])] = j;
//        }
//
//        int shift_value = 0;
//        while (shift_value <= lenth_text_input - length_pattern) {
//            int j = length_pattern - 1;
//            while (j >= 0 && patterns[i][j] == text[shift_value + j])
//                --j;
//
//            if (j < 0) {
//                ++count;
//                shift_value += (shift_value + length_pattern < lenth_text_input) ? length_pattern - bad_char[text[shift_value + length_pattern]] : 1;
//            }
//            else {
//                shift_value += std::max(1, j - bad_char[text[shift_value + j]]);
//            }
//        }
//        result_count_for_each_pattern[i] = count;
//    }
//}
//
//void countWordsInSegment_MIMD(const std::string& text, const std::vector<std::string>& patterns,
//    int& result_count_all_words_in_segment, int*& result_count_for_each_pattern, int patternSize) {
//
//    std::istringstream iss(text);
//    std::string word;
//    int localCount = 0;
//
//    // using iss >> word to fast loop through all words
//    while (iss >> word) {
//        ++localCount;
//    }
//
//    // Store the number of total words inside this specific chunk text
//    result_count_all_words_in_segment = localCount;
//
//    // Allocate memory for the list to store all the results is the count of times a specific word in specific patterns list occurred inside the text
//    result_count_for_each_pattern = new int[patternSize];
//
//    for (size_t i = 0; i < patterns.size(); i++) {
//        int count = 0;
//        int length_pattern = patterns[i].size();
//        int length_text_input = text.size();
//
//        std::vector<int> bad_char(256, -1);
//
//        for (int j = 0; j < length_pattern; j++) {
//            bad_char[static_cast<int>(patterns[i][j])] = j;
//        }
//
//        int shift_value = 0;
//        while (shift_value <= length_text_input - length_pattern) {
//            int j = length_pattern - 1;
//            while (j >= 0 && patterns[i][j] == text[shift_value + j])
//                --j;
//
//            if (j < 0) {
//                ++count;
//                shift_value += (shift_value + length_pattern < length_text_input) ? length_pattern - bad_char[text[shift_value + length_pattern]] : 1;
//            }
//            else {
//                shift_value += std::max(1, j - bad_char[text[shift_value + j]]);
//            }
//        }
//        result_count_for_each_pattern[i] = count;
//    }
//}
//// Define global variables for different design modes
//bool SISD = false, SIMD = false, MISD = false, MIMD = false;
//
//// Function to run a test case
//void runTestCase(const std::string& mode, const std::vector<std::string>& patterns, int numThreads, const std::string& filename) {
//    std::cout << "\nTesting Mode: " << mode << " with file name: " << filename << std::endl;
//
//    if (mode == "SISD") {
//        SISD = true;
//        SIMD = false;
//        MISD = false;
//        MIMD = false;
//    }
//    else if (mode == "SIMD") {
//        SISD = false;
//        SIMD = true;
//        MISD = false;
//        MIMD = false;
//    }
//    else if (mode == "MISD") {
//        SISD = false;
//        SIMD = false;
//        MISD = true;
//        MIMD = false;
//    }
//    else if (mode == "MIMD") {
//        SISD = false;
//        SIMD = false;
//        MISD = false;
//        MIMD = true;
//    }
//    else {
//        std::cerr << "Invalid test mode: " << mode << std::endl;
//        return;
//    }
//
//    std::cout << "\nTest Case:" << std::endl;
//    runTest(patterns, numThreads, filename);
//
//    std::cout << "\nDone testing Mode: " << mode << " with file name: " << filename << std::endl;
//}
//
//int main() {
//    // Test Input Files
//    std::vector<std::string> filenameList = { "Hamlet.txt", "Small.txt", "Medium.txt", "Large.txt" };
//
//    // Define the variables input to test for each design performance
//    std::vector<std::string> patterns1 = { "Horatio", "and", "Hamlet", "God" };
//    std::vector<std::string> patterns2 = { "Horatio", "and", "Hamlet", "God", "question", "To", "sleep", "death", "King", "or" };
//    std::vector<std::string> patterns3 = { "Horatio", "and" };
//    std::vector<std::string> patterns4 = { "Horatio", "and", "Hamlet", "God","question", "To", "sleep", "death", "King", "or", "nothing", "eyes", "speak","myself" };
//    std::vector<std::string> patterns5 = { "Horatio", "and", "Hamlet", "God", "question", "To", "sleep", "death", "King", "or", "that", "nothing", "eyes", "speak", "myself", "cock", "of", "Denmark", "ourselves", "funeral", "dirge", "marriage", "son", "him" };
//    std::vector<std::string> patterns6 = { "Horatio", "and", "Hamlet", "God", "question", "To", "sleep", "death", "King", "or", "that", "nothing", "eyes", "speak", "myself", "cock", "of", "Denmark", "ourselves", "funeral", "dirge", "marriage", "son", "him", "will", "again" };
//
//    const int numThreads1 = 4;
//    const int numThreads2 = 3;
//    const int numThreads3 = 8;
//    const int numThreads4 = 14;
//    const int numThreads5 = 28;
//    const int numThreads6 = 50;
//
//    std::vector<std::string> designModes = { "SISD", "SIMD", "MISD", "MIMD" };
//
//    for (const std::string& filename : filenameList) {
//        std::cout << "\nTesting files: " << filename << std::endl;
//        for (const std::string& mode : designModes) {
//            runTestCase(mode, patterns1, numThreads1, filename);
//            runTestCase(mode, patterns2, numThreads2, filename);
//            runTestCase(mode, patterns3, numThreads3, filename);
//            runTestCase(mode, patterns4, numThreads4, filename);
//            runTestCase(mode, patterns5, numThreads5, filename);
//            runTestCase(mode, patterns6, numThreads6, filename);
//        }
//    }
//
//    std::cout << "\nDone evaluating performance" << std::endl;
//
//    return 0;
//}
//
//void runTest(std::vector<std::string> patterns,int numThreads, std::string inputfile) {
//
//    std::string text = Reading_Files(inputfile);
//    //auto startTime = std::chrono::high_resolution_clock::now();
//
//    if (SISD)
//    {
//        auto startTime = std::chrono::high_resolution_clock::now();
//        std::cout << "Start Process SISD Design ..." << std::endl;
//        int totalwords = countWords_SISD(text);
//        int result = 0;
//        std::ofstream outputFile("results.txt", std::ios::app);
//        std::string resultString;           // this will store the / mode / pattern size/numthreads / words count / time consume  
//        if (outputFile.is_open()) {
//            outputFile << "Record" << std::endl;
//            outputFile << "Mode: SISD" << std::endl;
//            outputFile << "Pattern_Size: " << patterns.size() << std::endl;
//            outputFile << "Files_Name: " << inputfile << std::endl;
//            outputFile << "Text_Size: " << text.size() << std::endl;
//            outputFile << "num_Threads: " << numThreads << std::endl;
//
//            for (const std::string& pattern : patterns) {
//                search_SISD(text, pattern, result);
//                std::cout << result << " : " << pattern << std::endl;
//            }
//
//            std::cout << "Total_Word: " << totalwords << " words" << std::endl;
//            auto endTime = std::chrono::high_resolution_clock::now();
//            std::chrono::duration<double> totalTime = endTime - startTime;
//
//            // Convert the timing information to a string
//            outputFile << "Total_Word: " << totalwords << std::endl;
//            outputFile << "Total_time: " + std::to_string(totalTime.count()) + " seconds\n\n";
//            outputFile.close();
//
//            std::cout << "Results exported to results.txt" << std::endl;
//        }
//        else {
//            std::cerr << "Unable to open file for exporting results." << std::endl;
//        }
//        std::cout << "End Process SISD Design" << std::endl;
//    }
//    if (SIMD)
//    {
//        auto startTime = std::chrono::high_resolution_clock::now();
//        std::cout << "Start Process SIMD Design ..." << std::endl;
//
//        std::ofstream outputFile("results.txt", std::ios::app);
//        if (outputFile.is_open()) {
//            outputFile << "Record" << std::endl;
//            outputFile << "Mode: SIMD" << std::endl;
//            outputFile << "Pattern_Size: " << patterns.size() << std::endl;
//            outputFile << "Files_Name: " << inputfile << std::endl;
//            outputFile << "Text_Size: " << text.size() << std::endl;
//            outputFile << "num_Threads: " << numThreads << std::endl;
//
//            // Defined the list of threads and the results and the string input need to process and need to collect the results to recalculate later
//            std::vector<std::thread> threads(numThreads);
//            std::string* chunk_char_each_threads = new std::string[numThreads];
//            int* results_count_all_words_in_segment = new int[numThreads]();
//            int** results_count_for_each_pattern = new int* [numThreads];
//            int totalCount = 0;
//            int sizepatterns = patterns.size();
//
//
//            // Allocate memory for the results count for each words have size equal to patterns.size() store the results in 2D array
//            for (int i = 0; i < numThreads; i++) {
//                results_count_for_each_pattern[i] = new int[sizepatterns];
//            }
//
//            // Loop through all the thread to split the original text to each chunks
//            for (int i = 0; i < numThreads; i++) {
//                // Divide the text into segments and check if there is remainder in the text input
//                int segmentSize = text.size() / numThreads;
//                int remainder = text.size() % numThreads;
//
//                int start = i * (text.size() / numThreads);
//                int end = (i == numThreads - 1) ? text.size() : (start + (text.size() / numThreads));
//                if (i == numThreads - 1 && remainder > 0) {
//                    end += remainder;
//                }
//                // Store back input string for each chunk to the string array
//                chunk_char_each_threads[i] = text.substr(start, end - start);
//            }
//            // Loop though all the threads to assign the task with their specific input - results location to write/read
//            for (int i = 0; i < numThreads; i++) {
//                threads[i] = std::thread([&, i]() {
//                    countWordsInSegment_SIMD(chunk_char_each_threads[i], patterns, results_count_all_words_in_segment[i], results_count_for_each_pattern[i], sizepatterns);
//                    });
//            }
//            // Loop though all the threads to call join method to make main threads wait for all threads finish their works
//            for (auto& thread : threads) {
//                thread.join();
//            }
//
//            // Loop though all the results to sum up for total words are counted inside each threads in results_count_all_words_in_segment int*
//            for (int i = 0; i < numThreads; i++) {
//                totalCount += results_count_all_words_in_segment[i];
//            }
//
//            // Loop though all the results corresponding to the pattern to sum up for total times that words are occured are counted inside each threads in results_count_for_each_pattern int**
//            for (size_t i = 0; i < patterns.size(); i++) {
//                int patternTotalCount = 0;
//                for (int j = 0; j < numThreads; j++) {
//                    patternTotalCount += results_count_for_each_pattern[j][i];
//                }
//                std::cout << patternTotalCount << ": " << patterns[i] << std::endl;
//            }
//            std::cout << "Total word count: " << totalCount << " words" << std::endl;
//            auto endTime = std::chrono::high_resolution_clock::now();
//            std::chrono::duration<double> totalTime = endTime - startTime;
//
//            // Convert the timing information to a string
//
//            // Delete all the memory allocated before to avoid memory leak
//            delete[] results_count_all_words_in_segment;
//            for (int i = 0; i < numThreads; i++) {
//                delete[] results_count_for_each_pattern[i];
//            }
//            delete[] results_count_for_each_pattern;
//            delete[] chunk_char_each_threads;
//
//            outputFile << "Total_Word: " << totalCount << std::endl;
//            outputFile << "Total_time: " + std::to_string(totalTime.count()) + " seconds\n\n";
//            outputFile.close();
//
//            std::cout << "Results exported to results.txt" << std::endl;
//        }
//        else {
//            std::cerr << "Unable to open file for exporting results." << std::endl;
//        }
//        std::cout << "End Process SIMD Design" << std::endl;
//    }
//    if (MISD)
//    {
//        auto startTime = std::chrono::high_resolution_clock::now();
//        std::cout << "Start Process MISD Design ..." << std::endl;
//
//        std::ofstream outputFile("results.txt", std::ios::app);
//        if (outputFile.is_open()) {
//            outputFile << "Record" << std::endl;
//            outputFile << "Mode: MISD" << std::endl;
//            outputFile << "Pattern_Size: " << patterns.size() << std::endl;
//            outputFile << "Files_Name: " << inputfile << std::endl;
//            outputFile << "Text_Size: " << text.size() << std::endl;
//            outputFile << "num_Threads: " << numThreads << std::endl;
//
//            int totalwords = countWords_SISD(text);
//            std::vector<int> results(patterns.size(), 0);
//            std::vector<std::thread> threads;
//
//            // Iterate through each pattern and create a thread for each search for MISD
//            for (std::size_t i = 0; i < patterns.size(); ++i) {
//                threads.emplace_back(search_SISD, std::ref(text), std::ref(patterns[i]), std::ref(results[i]));
//            }
//            for (auto& thread : threads) {
//                thread.join();
//            }
//            for (std::size_t i = 0; i < patterns.size(); ++i) {
//                std::cout << results[i] << " : " << patterns[i] << std::endl;
//            }
//            std::cout << "Total word count: " << totalwords << " words" << std::endl;
//
//            auto endTime = std::chrono::high_resolution_clock::now();
//            std::chrono::duration<double> totalTime = endTime - startTime;
//
//            // Export results to a text file
//            outputFile << "Total_Word: " << totalwords << std::endl;
//            outputFile << "Total_time: " + std::to_string(totalTime.count()) + " seconds\n\n";
//            outputFile.close();
//
//            std::cout << "Results exported to results.txt" << std::endl;
//        }
//        else {
//            std::cerr << "Unable to open file for exporting results." << std::endl;
//        }
//        std::cout << "End Process MISD Design" << std::endl;
//    }
//    if (MIMD)
//    {
//        auto startTime = std::chrono::high_resolution_clock::now();
//        std::cout << "Start Process MIMD Design ..." << std::endl;
//        std::ofstream outputFile("results.txt", std::ios::app);
//        if (outputFile.is_open()) {
//            outputFile << "Record" << std::endl;
//            outputFile << "Mode: MIMD" << std::endl;
//            outputFile << "Pattern_Size: " << patterns.size() << std::endl;
//            outputFile << "Files_Name: " << inputfile << std::endl;
//            outputFile << "Text_Size: " << text.size() << std::endl;
//            outputFile << "num_Threads: " << numThreads << std::endl;
//
//            // Defined the list of threads and the results
//            std::vector<std::thread> threads(numThreads);
//            std::vector<std::string> chunk_char_each_threads(numThreads);
//            std::vector<int> results_count_all_words_in_segment(numThreads);
//            std::vector<int*> results_count_for_each_pattern(numThreads);
//            int totalCount = 0;
//            int sizepatterns = patterns.size();
//
//            // Loop through all the threads to split the original text into chunks corresponding to the number of threads
//            for (int i = 0; i < numThreads; i++) {
//                int start = i * (text.size() / numThreads);
//                int end = (i == numThreads - 1) ? text.size() : (start + (text.size() / numThreads));
//                chunk_char_each_threads[i] = text.substr(start, end - start);
//            }
//
//            // Loop through all the threads to assign the task with their specific input - results location to write/read
//            for (int i = 0; i < numThreads; i++) {
//                threads[i] = std::thread([&, i]() {
//                    results_count_for_each_pattern[i] = new int[sizepatterns];
//                    countWordsInSegment_MIMD(chunk_char_each_threads[i], patterns,
//                        results_count_all_words_in_segment[i],
//                        results_count_for_each_pattern[i], sizepatterns);
//                    });
//            }
//
//            // Loop through all the threads to call join method to make the main thread wait for all threads to finish their works
//            for (auto& thread : threads) {
//                thread.join();
//            }
//
//            // Loop through all the results to sum up for total words counted inside each thread in results_count_all_words_in_segment int*
//            for (int i = 0; i < numThreads; i++) {
//                totalCount += results_count_all_words_in_segment[i];
//            }
//            // Loop through all the results corresponding to the pattern to sum up for total times that words are occurred are counted inside each thread in results_count_for_each_pattern int**
//            for (size_t i = 0; i < patterns.size(); i++) {
//                int patternTotalCount = 0;
//                for (int j = 0; j < numThreads; j++) {
//                    patternTotalCount += results_count_for_each_pattern[j][i];
//                }
//                std::cout << patternTotalCount << ": " << patterns[i] << std::endl;
//            }
//            std::cout << "Total word count: " << totalCount << " words" << std::endl;
//
//            auto endTime = std::chrono::high_resolution_clock::now();
//            std::chrono::duration<double> totalTime = endTime - startTime;
//
//            // Delete allocated memory
//            for (int i = 0; i < numThreads; i++) {
//                delete[] results_count_for_each_pattern[i];
//            }
//            // Export results to a text file
//            outputFile << "Total_Word: " << totalCount << std::endl;
//            outputFile << "Total_time: " + std::to_string(totalTime.count()) + " seconds\n\n";
//            outputFile.close();
//
//            std::cout << "Results exported to results.txt" << std::endl;
//        }
//        else {
//            std::cerr << "Unable to open file for exporting results." << std::endl;
//        }
//        std::cout << "End Process MIMD Design" << std::endl;
//    }
//    //auto endTime = std::chrono::high_resolution_clock::now();
//    //std::chrono::duration<double> totalTime = endTime - startTime;
//    //std::cout << "Total time: " << totalTime.count() << " seconds" << std::endl;
//}

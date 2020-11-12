#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <numeric>

#include "../../src/logic/hashing/HashTable.hpp"

using namespace std;
using namespace std::chrono;

double timeInsert(HashTable<int> &table, vector<string>& strings) {
    double time;
    for (string str : strings) {
        auto start = chrono::high_resolution_clock::now();
        table.insert(str, 1);
        auto end = chrono::high_resolution_clock::now();
        time += duration<double, std::micro>(end - start).count();
    }
    return time / (double) strings.size();
}

double timeSearch(HashTable<int> &table, vector<string>& strings) {
    double time = 0;
    for (string str : strings) {
        auto start = chrono::high_resolution_clock::now();
        int value;
        table.get(str, value);
        auto end = chrono::high_resolution_clock::now();
        time += duration<double, std::micro>(end - start).count();
    }
    return time / (double) strings.size();
}

double timeDelete(HashTable<int> &table, vector<string>& strings) {
    double time = 0;
    for (string str : strings) {
        auto start = chrono::high_resolution_clock::now();
        table.remove(str);
        auto end = chrono::high_resolution_clock::now();
        time += duration<double, std::micro>(end - start).count();
    }
    return time / (double) strings.size();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Please supply the name of the input file\n";
        return 1;
    }
    // Read input
    vector<string> test_strings;
    fstream inFile(argv[1]);
    int number_of_strings;
    inFile >> number_of_strings;

    for (int i = 0; i < number_of_strings; i++) {
        string current;
        inFile >> current;
        test_strings.push_back(current);
    }
    inFile.close();

    int number_of_trials = 5000;
    vector<double> insert_averages(number_of_trials);
    vector<double> search_averages(number_of_trials);
    vector<double> delete_averages(number_of_trials);
    for (int i = 0; i < number_of_trials; i++) {
        HashTable<int> table(test_strings.size());
        double average_insert = timeInsert(table, test_strings);
        insert_averages[i] = average_insert;
        // cout << "[T" << i << "] AVERAGE INSERT: " << to_string(average_insert) << " [µs]" << endl;
        
        double average_search = timeSearch(table, test_strings);
        search_averages[i] = average_search;
        // cout << "[T" << i << "] AVERAGE SEARCH: " << average_insert << " [µs]" << endl;
        
        double average_delete = timeDelete(table, test_strings);
        delete_averages[i] = average_delete;
        // cout << "[T" << i << "] AVERAGE DELETE: " << average_insert << " [µs]" << endl;
    }

    cout << "---- Average time for " << number_of_strings << " strings with " << number_of_trials << " trials ----" << endl;
    double total_insert_average = accumulate(insert_averages.begin(), insert_averages.end(), decltype(insert_averages)::value_type(0));
    total_insert_average /= (double) number_of_trials;
    cout << "TOTAL INSERT AVERAGE: " << total_insert_average << " [µs]" << endl;
    
    double total_search_average = accumulate(search_averages.begin(), search_averages.end(), decltype(search_averages)::value_type(0));
    total_search_average /= (double)number_of_trials;
    cout << "TOTAL SEARCH AVERAGE: " << total_search_average << " [µs]" << endl;

    double total_delete_average = accumulate(delete_averages.begin(), delete_averages.end(), decltype(delete_averages)::value_type(0));
    total_delete_average /= (double)number_of_trials;
    cout << "TOTAL DELETE AVERAGE: " << total_delete_average << " [µs]" << endl;

    return 0;
}
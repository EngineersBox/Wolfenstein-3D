#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "../../src/logic/queue/MinHeap.hpp"
#include "../../src/logic/pathfinding/GraphNode.hpp"

using namespace std;
using namespace std::chrono;

double timeEmplace(MinHeap<int, GraphNode>& queue, vector<pair<int, GraphNode>>& nodes) {
    double time;
    for (pair<int, GraphNode> node : nodes) {
        auto start = chrono::high_resolution_clock::now();
        queue.emplace(node.first, node.second);
        auto end = chrono::high_resolution_clock::now();
        time += duration<double, std::micro>(end - start).count();
    }
    return time / (double)nodes.size();
}

double timeTopPop(MinHeap<int, GraphNode>& queue, vector<pair<int, GraphNode>>& nodes) {
    double time;
    for (pair<int, GraphNode> node : nodes) {
        auto start = chrono::high_resolution_clock::now();
        queue.top();
        queue.pop();
        auto end = chrono::high_resolution_clock::now();
        time += duration<double, std::micro>(end - start).count();
    }
    return time / (double)nodes.size();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Please supply the name of the input file\n";
        return 1;
    }
    // Read input
    vector<pair<int, GraphNode>> test_nodes;
    fstream inFile(argv[1]);
    int number_of_nodes;
    inFile >> number_of_nodes;

    for (int i = 0; i < number_of_nodes; i++) {
        int id, x, y;
        inFile >> id >> x >> y;
        test_nodes.push_back(pair<int, GraphNode>(id, GraphNode(x, y)));
    }
    inFile.close();

    int number_of_trials = 5000;
    vector<double> emplace_averages(number_of_trials);
    vector<double> top_pop_averages(number_of_trials);
    for (int i = 0; i < number_of_trials; i++) {
        MinHeap<int, GraphNode> queue;
        double average_emplace = timeEmplace(queue, test_nodes);
        emplace_averages[i] = average_emplace;
        // cout << "[T" << i << "] AVERAGE EMPLACE: " << to_string(average_insert) << " [µs]" << endl;

        double average_top_pop = timeTopPop(queue, test_nodes);
        top_pop_averages[i] = average_top_pop;
        // cout << "[T" << i << "] AVERAGE TOP + POP: " << average_insert << " [µs]" << endl;
    }

    cout << "---- Average time for " << number_of_nodes << " nodes with " << number_of_trials << " trials ----" << endl;
    double total_insert_average = accumulate(emplace_averages.begin(), emplace_averages.end(), decltype(emplace_averages)::value_type(0));
    total_insert_average /= (double)number_of_trials;
    cout << "TOTAL EMPLACE AVERAGE: " << total_insert_average << " [µs]" << endl;

    double total_search_average = accumulate(top_pop_averages.begin(), top_pop_averages.end(), decltype(top_pop_averages)::value_type(0));
    total_search_average /= (double)number_of_trials;
    cout << "TOTAL TOP + POP AVERAGE: " << total_search_average << " [µs]" << endl;


    return 0;
}
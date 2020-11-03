#pragma once

#include <assert.h>

#include <algorithm>

#include "../../exceptions/heap/EmptyHeapException.hpp"

using namespace std;

template <typename K, typename V>
struct HeapEntry {
    K key;
    V value;
};

template <typename K, typename V>
class MinHeap {
    public:
        MinHeap();

        void emplace(K key, V value);
        V& top();
        void pop();

        inline bool isEmpty() const noexcept;
        void clear() noexcept;
    private:
        void heapifyUp(int idx);
        void heapifyDown(int idx);

        vector<HeapEntry<K,V>> queue;
        int size;
};

template <typename K, typename V>
MinHeap<K,V>::MinHeap() {
    this->size = 0;
};

template <typename K, typename V>
void MinHeap<K,V>::heapifyUp(int idx) {
    if (idx < 1) {
        return;
    }

    int parent = IDIV_2(idx - 1);

    if (this->queue[idx].key < this->queue[parent].key) {
        iter_swap(this->queue.begin() + idx, this->queue.begin() + parent);
        heapifyUp(parent);
    }
};

template <typename K, typename V>
void MinHeap<K,V>::heapifyDown(int idx) {
    int min = -1;
    int leftIdx = IMUL_2(idx) + 1;
    int rightIdx = IMUL_2(idx) + 2;

    if (leftIdx < size && queue[idx].key > queue[leftIdx].key) {
        min = leftIdx;
    }

    if (rightIdx < size && queue[(min == -1 ? idx : min)].key > queue[rightIdx].key) {
        min = rightIdx;
    }

    if (min == -1)
        return;

    iter_swap(this->queue.begin() + idx, this->queue.begin() + min);
    heapifyDown(min);
}

template <typename K, typename V>
void MinHeap<K, V>::emplace(K key, V value) {
    this->queue.push_back(HeapEntry<K,V>{key, value});
    heapifyUp(size);
    this->size++;
}

template <typename K, typename V>
V& MinHeap<K,V>::top() {
    if (size == 0) {
        throw EmptyHeapException();
    }
    return this->queue[0].value;
}

template <typename K, typename V>
void MinHeap<K,V>::pop() {
    if (size == 0) {
        throw EmptyHeapException();
    }
    this->queue.erase(this->queue.begin());
    size--;
    heapifyDown(0);
}

template <typename K, typename V>
bool MinHeap<K,V>::isEmpty() const noexcept {
    return size == 0;
}

template <typename K, typename V>
void MinHeap<K,V>::clear() noexcept {
    this->queue.clear();
}
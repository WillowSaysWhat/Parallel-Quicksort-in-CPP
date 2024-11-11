#include "partition.h"


// lNums = Array of License Numbers 

using namespace std;

int partition(vector<int>& mockData, int low, int high) {

    // Choosing the last element as the pivot
    int pivot = mockData[high];
    // the index of the smaller element
    int smallIndex = low - 1;        

    for (int currentIndex = low; currentIndex < high; currentIndex++) {
        // If the current element is smaller than or equal to pivot
        if (mockData[currentIndex] <= pivot) {
            // Increment the index of the smaller element
            smallIndex++;  

            swap(mockData[smallIndex], mockData[currentIndex]);
        }
    }
    // Swap the pivot element with the element at i+1
    swap(mockData[smallIndex + 1], mockData[high]);
    return smallIndex + 1;  // Return the partition index
}

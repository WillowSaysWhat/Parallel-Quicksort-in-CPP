# Partition Function

<p align="center">
<img src="/docs/assets/Partitioning.gif" alt="partitioning" >
</p>

This is the partitioning function. It finds the last index (pivot) and then organises other data to the left or right of it depending on whether the value is greater than or less than the pivot.

## Breakdown

This function takes a vector of integers, (mockData) and two indices (low and high) which represent the range of the array to be partitioned.
It returns an integer, which is the "partition index" — the final position of the pivot after partitioning.

```cpp
//partition.h
int partition(vector<int>& mockData, int low, int high);

```
The pivot element is chosen as the last element in the array (at index high).
This pivot will be used to separate elements smaller than or equal to it (on its left) from elements greater than it (on its right).

```cpp

int pivot = mockData[high];

```

smallIndex keeps track of the position of the last element that is less than or equal to the pivot.
It starts from low - 1 because we haven’t processed any elements yet. The purpose of smallIndex is to help identify where to place elements smaller than or equal to the pivot.

```cpp
int smallIndex = low - 1;

```
This loop iterates from low to high - 1 examining each element in the range [low, high-1].
currentIndex represents the current element being compared to the pivot.

```cpp
for (int currentIndex = low; currentIndex < high; currentIndex++) {}

```

If the current element (mockData[currentIndex]) is less than or equal to the pivot, it should be on the left side of the pivot.

```cpp
if (mockData[currentIndex] <= pivot) {}

```
smallIndex++: Increment smallIndex to point to the next available position on the left side.
swap(mockData[smallIndex], mockData[currentIndex]);: Swap the element at currentIndex (which is <= pivot) with the element at smallIndex.
This swap places elements smaller than or equal to the pivot on the left side of the array

```cpp
smallIndex++;
swap(mockData[smallIndex], mockData[currentIndex]);

```
After the loop completes, all elements <= pivot are positioned to the left of smallIndex, and all elements > pivot are to its right.
swap(mockData[smallIndex + 1], mockData[high]);: The pivot is then swapped with the element at smallIndex + 1, placing the pivot in its final position in the sorted order.

```cpp
swap(mockData[smallIndex + 1], mockData[high]);

``` 
The function returns smallIndex + 1, which is the index of the pivot in the newly partitioned array.
This index is used in Quicksort to recursively sort the left and right subarrays around the pivot.

```cpp

return smallIndex + 1;

```
Below is the full code implementation of the partition fuction. it is separated into a .cpp file for readability.

## Implementation
```cpp
#include "partition.h"


// lNums = Array of License Numbers 

using namespace std;

int partition(vector<int>& mockData, int low, int high) {

    // select the last element as the pivot
    int pivot = mockData[high];
    // the index of the smaller element
    int smallIndex = low - 1;        

    for (int currentIndex = low; currentIndex < high; currentIndex++) {
        // If the current element is smaller than or equal to pivot, swap data.
        if (mockData[currentIndex] <= pivot) {
            smallIndex++;  
            swap(mockData[smallIndex], mockData[currentIndex]);
        }
    }
    // Swap the pivot element with the element at smallIndex+1
    swap(mockData[smallIndex + 1], mockData[high]);
    return smallIndex + 1;  // Return the partition index
}

```

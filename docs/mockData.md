# Mock Data Generator

This code defines the function `mockData` that generates a vector of unique random integers in a shuffled order.

## how it works

If mockData(5) is called:

1. mockData is initially filled as [0, 1, 2, 3, 4].
2. After the shuffle step, it might return a randomized order, like [3, 1, 4, 0, 2] for example.

This randomised output can be used for testing sorting algorithms, searching functions, or other operations that benefit from random input data.

## Breakdown

```cpp
#include <algorithm>
#include <random>

```
`algorithm` provides algorithms for common operations, like random_shuffle.
`random` provides utilities for generating random numbers, which we’ll use to shuffle the data.

```cpp
vector<int> mockData(int amount)

```
`mockData` takes an integer amount as input representing the number of elements to generate.
It returns a vector<int> filled with unique random integers in a shuffled order.

```cpp
vector<int> mockData;

```
`mockData` is declared as an empty vector of integers, which will store the generated numbers.

```cpp
for (int i = 0; i < amount; i++) {
    mockData.push_back(i);
}

```
the loop iterates from 0 to amount - 1.
For each iteration it appends the current value of i to mockData, so mockData ends up containing the values [0, 1, 2, ..., amount - 1].

```cpp
random_device ranDev;
mt19937 g(ranDev());

```
`random_device ranDev;` creates a `random_device` object, which is used to seed the random number generator with a non-deterministic value.
`mt19937 g(ranDev());` initialises an mt19937 random number generator (Mersenne Twister) using ranDev as its seed. Mersenne Twister provides a high-quality pseudo-random number generation suitable for shuffling.

At first, `random_shuffle` was used to reorganise the vector. However, this was deprecated in C++14 in favour of `shuffle`.
```cpp
shuffle(mockData.begin(), mockData.end(), g);
return mockData;
```
`shuffle` randomly reorders the elements in mockData using the specified random generator g. This results in a shuffled version of the numbers in mockData. Finally, the function returns the mockData vector.



# Implementation

```cpp
#include <algorithm>
#include <random>

using namespace std;

vector<int> mockData(int amount) {

    std::vector<int> mockData;

    // make a vector of accending numbers
    for (int i = 0; i < amount; i++) {
        mockData.push_back(i);
    }

    // randomise the numbers 
    random_device ranDev;
    mt19937 g(ranDev());
    shuffle(mockData.begin(), mockData.end(), g);

    return mockData;
}
```
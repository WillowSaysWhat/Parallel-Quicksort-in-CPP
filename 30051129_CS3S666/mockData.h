#pragma once
#include <algorithm>
#include <random>

using namespace std;

vector<int> mockData(int amount) {

    vector<int> mockData;

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
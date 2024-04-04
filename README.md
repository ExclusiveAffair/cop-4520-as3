# cop-4520-as3

Instructions for how to run this program on Windows 10 OS are as follows:

To compile: `g++ -std=c++17 filename.cpp`

To run: `./a`

## Problem 1 Report

### Approach

I use two ordered sets, `presentList` and `unprocessedPresents`, to represent the presents in the linked list and the presents that have yet to be added to the list, respectively. When a servant adds a present to the list, he chooses one at random from the presents in `unprocessedPresents`. When a servant removes a present from the list, he chooses one at random from the presents in `presentList` and increments the number of thank you cards written.

To ensure that each servant alternates between attempting the two actions, I maintain a vector called `state` that stores which action each servant is due to attempt next.

### Correctness

By using mutex locks, I ensure that no two servants are able to simultaneously enter the critical section. In this way, I ensure that each present-giver has a thank you card written for them.

### Efficiency

My program takes a little under a minute to run, but could likely be optimized further by eliminating the log factor inherent with ordered sets. In my decision to use ordered sets, I took a hit with regards to efficiency in the name of simplicity.

## Problem 2 Report

### Approach

For each minute in the hour, I select a random sensor to obtain the temperature reading. I add this reading to a global vector called `temps` that is cleared every hour after the readings are processed.

To determine the window in which the largest difference in temperatures is reported, I use a sliding window technique in which I insert and remove elements from an ordered set and query the smallest and largest elements in the set. This is an O(nlogn) approach, but an O(n^2) approach would also suffice because n is small enough.

Finally, I sort the temperature readings to easily obtain the five coldest and five hottest temperatures.

### Correctness

By using mutex locks, I ensure that no two sensors are able to simultaneously enter the critical section. In this way, I ensure that the number of temperature readings is accurate.

### Efficiency

My program runs almost instantaneously due to the low bounds on n.
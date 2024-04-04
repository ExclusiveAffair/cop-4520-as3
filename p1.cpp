#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>
#include <fstream>
#include <bits/extc++.h> /** keep-include */
using namespace std;
using namespace std::chrono;
using ll = long long;
using namespace __gnu_pbds;

template<class T>
using Tree = tree<T, null_type, less<T>, rb_tree_tag,
    tree_order_statistics_node_update>;

const int PRESENTS = 500000;
const int SERVANTS = 4;
mutex m;
int numThanksWritten;
Tree<int> presentList, unprocessedPresents;
vector<int> state(SERVANTS); // 0 indicates adding present, 1 indicates writing note

// returns id of the next present in the list provided, or -1 if this present is the last one
int nextPresent(Tree<int> &list, int id) {
    auto res = list.upper_bound(id);
    if (res == list.end()) return -1;
    return *res;
}

// returns id of the previous present in the list provided, or -1 if this present is the first one
int previousPresent(Tree<int> &list, int id) {
    auto res = list.lower_bound(id);
    if (res == list.begin()) return -1;
    return *prev(res);
}

// returns true if a present is in the list provided, returns false otherwise
bool presentInList(Tree<int> &list, int id) {
    int order = list.order_of_key(id);
    return *list.find_by_order(order) == id;
}

// returns the kth present (zero-indexed) in the list provided, or -1 if there is none
int getKthPresent(Tree<int> &list, int k) {
    if (k >= list.size()) return -1;
    return *list.find_by_order(k);
}

// adds a present from the list. returns true if the operation
// was successful, returns false otherwise
bool addPresent(int servant) {
    m.lock();
    state[servant] = 1 - state[servant];

    // no presents to add
    if (unprocessedPresents.size() == 0) {
        m.unlock();
        return false;
    }
    int pid = (rand() * rand()) % unprocessedPresents.size();

    int transfer = getKthPresent(unprocessedPresents, pid);
    presentList.insert(transfer);
    unprocessedPresents.erase(transfer);
    
    m.unlock();
    return true;
}

// removes a present from the list. returns true if the operation
// was successful, returns false otherwise
bool writeThankYou(int servant) {
    m.lock();
    state[servant] = 1 - state[servant];

    // no thank yous to write
    if (presentList.size() == 0) {
        m.unlock();
        return false;
    }
    int pid = (rand() * rand()) % presentList.size();

    int rem = getKthPresent(presentList, pid);
    presentList.erase(rem);
    numThanksWritten++;
    
    m.unlock();
    return true;
}

void solve() {
    ll start = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    ).count();

    vector<thread> threads(SERVANTS);
    for (int i = 0; i < SERVANTS; i++) threads[i] = thread(addPresent, i);
    for (int i = 0; i < PRESENTS; i++) unprocessedPresents.insert(i);

    while (numThanksWritten != PRESENTS) {
        int servant = rand() % SERVANTS;
        threads[servant].join();

        if (state[servant] == 0) {
            threads[servant] = thread(writeThankYou, servant);
        }
        else {
            threads[servant] = thread(addPresent, servant);
        }
    }

    for (auto& th : threads) {
        th.join();
    }

    ll end = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    ).count();

    cout << "All thank yous have been written!" << endl;
    cout << "Duration: " << end-start << " ms" << endl;
}

int main() {
    srand(time(0));
    solve();
}
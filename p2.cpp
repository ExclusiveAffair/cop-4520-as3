#include <bits/stdc++.h>
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>
#include <fstream>
using namespace std;
using namespace std::chrono;
using ll = long long;

mutex m;
const int SENSORS = 8;
const int MIN_TEMPERATURE = -100, MAX_TEMPERATURE = 70;
const int HOURS_IN_DAY = 24, MINUTES_IN_HOUR = 60;

random_device rdev;
mt19937 rng(rdev());
uniform_int_distribution<> temp(MIN_TEMPERATURE, MAX_TEMPERATURE);
vector<int> temps;

int read() {
    return temp(rng);
}

void readTemperature() {
    m.lock();
    temps.push_back(read());
    m.unlock();
}

void clearList() {
    m.lock();
    temps.clear();
    m.unlock();
}

void solve() {
    ll start = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    ).count();

    vector<thread> threads(SENSORS);
    for (int i = 0; i < SENSORS; i++) threads[i] = thread();

    for (int hour = 0; hour < HOURS_IN_DAY; hour++) {
        for (int minute = 0; minute < MINUTES_IN_HOUR; minute++) {
            int sensor = rand() % SENSORS;
            if (threads[sensor].joinable()) threads[sensor].join();
            threads[sensor] = thread(readTemperature);
        }
        for (auto& th : threads) {
            th.join();
        }
        assert(temps.size() == MINUTES_IN_HOUR);

        multiset<int> window;
        for (int i = 0; i < 10; i++) {
            window.emplace(temps[i]);
        }
        int startingMinute = -1, largestDiff = -1e9;
        for (int adv = 10; adv <= temps.size(); adv++) {
            // process this range
            int currentDiff = *prev(window.end()) - *window.begin();
            if (largestDiff < currentDiff) {
                largestDiff = currentDiff;
                startingMinute = adv - 10;
            }

            if (adv < temps.size()) {
                // shift window
                window.erase(window.find(temps[adv-10]));
                window.emplace(temps[adv]);
            }
        }

        cout << "Hour " << hour+1 << '\n';
        cout << "==============================\n";

        sort(begin(temps), end(temps));
        cout << "Five coldest temps" << '\n';
        for (int i = 0; i < 5; i++) cout << temps[i] << '\n';
        cout << "Five hottest temps" << '\n';
        for (int i = temps.size()-5; i < temps.size(); i++) cout << temps[i] << '\n';
        cout << "The window with the largest variation is from minute " << startingMinute+1 << " to minute " << startingMinute+11 << ", with a variation of " << largestDiff << " degrees\n";

        cout << '\n';
        clearList();
    }

    ll end = duration_cast< milliseconds >(
        system_clock::now().time_since_epoch()
    ).count();

    cout << "Simulation Complete!" << endl;
    cout << "Duration: " << end-start << " ms" << endl;
}

int main() {
    srand(time(0));
    solve();
}
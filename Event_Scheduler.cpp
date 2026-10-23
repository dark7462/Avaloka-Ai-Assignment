
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <functional>
#include <chrono>
using namespace std;

class Event_Scheduler {
public:
    bool can_attend_all(vector<pair<int, int>> events) {
        if (events.empty()) return true;
        sort(events.begin(), events.end());
        if(events[0].first > events[0].second){
            cout << "Invalid interval" << endl;
            return false;
        }
        for (int i = 1; i < events.size(); i++) {
            if(events[i].first > events[i].second){
                cout << "Invalid interval" << endl;
                return false;
            }
            if (events[i-1].second > events[i].first)
                return false;
        }
        return true;
    }

    int min_rooms_required(vector<pair<int, int>> events) {
        sort(events.begin(), events.end());

        priority_queue<int, vector<int>, greater<int>> heap;

        for (auto &event : events) {
            if(event.first > event.second){
                cout << "Invalid interval" << endl;
                return -1;
            }
            if (!heap.empty() && heap.top() <= event.first) {
                heap.pop();
            }
            heap.push(event.second);
        }

        return heap.size();
    }
};

#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <vector>
using namespace std;

int main() {
    int Q;
    cin >> Q;
    // ヒープ (最小の要素を取り出す)
    priority_queue<int, vector<int>, greater<>> X;
    // キュー
    queue<int> Y;

    for(int i = 0; i < Q; i++) {
        int type;
        cin >> type;
        if(type == 1) {
            int v;
            cin >> v;
            Y.emplace(v);
        } else if(type == 2) {
            if(X.size()) {
                int v = X.top();
                X.pop();
                cout << v << endl;
            } else {
                int v = Y.front();
                Y.pop();
                cout << v << endl;
            }
        } else if(type == 3) {
            while(Y.size()) {
                int v = Y.front();
                Y.pop();
                X.emplace(v);
            }
        }
    }
    return 0;
}

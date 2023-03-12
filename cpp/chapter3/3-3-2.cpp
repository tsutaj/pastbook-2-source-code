#include <algorithm>
#include <iostream>
#include <numeric>
#include <stack>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;

int main() {
    int N;
    cin >> N;
    vector<int> A(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i];
    }

    // 番兵
    A.emplace_back(0);
    N++;

    // stack では (長方形の高さ、左端の添字) を管理する
    stack<pair<int, int>> st;
    int ans = 0;
    for(int i = 0; i < N; i++) {
        int left_index = i;
        while(st.size() && st.top().first > A[i]) {
            int value = st.top().first;
            left_index = st.top().second;
            int length = i - left_index;
            st.pop();
            ans = max(ans, value * length);
        }
        st.emplace(A[i], left_index);
    }
    cout << ans << endl;
    return 0;
}

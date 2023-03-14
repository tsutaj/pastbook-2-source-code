#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

int main() {
    // 入力
    int N, H;
    cin >> N >> H;
    vector<pair<int, int>> act(N);
    for(int i = 0; i < N; i++) {
        int a, b;
        cin >> a >> b;
        act[i] = make_pair(a, b);
    }

    // 活動を a[i] / b[i] が大きい順にソートする
    sort(act.begin(), act.end(), [](auto x, auto y) {
        // x.first / x.second > y.first / y.second;
        // -> x.first * y.second > y.first * x.second;
        return 1LL * x.first * y.second > 1LL * y.first * x.second;
    });

    // 配列 dp を定義する
    vector<vector<long long>> dp(N + 1, vector<long long>(H + 1));

    // dp[i + 1] の値を順に求めていく
    for(int i = 0; i < N; i++) {
        auto [a, b] = act[i];
        for(int h = 0; h <= H; h++) {
            dp[i + 1][h] = max(dp[i + 1][h], dp[i][h]);
            dp[i + 1][max(h - b, 0)] =
                max(dp[i + 1][max(h - b, 0)], dp[i][h] + 1LL * a * h);
        }
    }

    // 答え
    cout << *max_element(dp[N].begin(), dp[N].end()) << endl;
    return 0;
}

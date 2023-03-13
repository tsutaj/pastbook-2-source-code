#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 入力を受け取る
    int N, W;
    cin >> N >> W;
    vector<int> w(N), v(N);
    for(int i = 0; i < N; i++) {
        cin >> w[i] >> v[i];
    }

    // 配列 dp を定義
    const long long INF = 1LL << 60; // 十分大きい値
    vector<vector<long long>> dp(N + 1, vector<long long>(W + 1, -INF));

    // 初期値 (0 個の品物から重さ 0 で選んで、価値の総和は 0)
    dp[0][0] = 0;

    // i が小さい順に dp の値を求めていく
    for(int i = 0; i < N; i++) {
        for(int j = 0; j <= W; j++) {
            // 品物 i を選ばない場合
            dp[i + 1][j] = max(dp[i + 1][j], dp[i][j]);

            // 品物 i を選ぶ場合
            if(j >= w[i]) {
                dp[i + 1][j] = max(dp[i + 1][j], dp[i][j - w[i]] + v[i]);
            }
        }
    }
    // 答えを求める
    cout << *max_element(dp[N].begin(), dp[N].end()) << endl;
    return 0;
}

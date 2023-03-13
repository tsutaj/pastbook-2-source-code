#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 入力
    int N;
    cin >> N;
    vector<vector<int>> c(N + 1, vector<int>(N + 1));
    for(int i = 0; i <= N; i++) {
        for(int j = 0; j <= N; j++) {
            cin >> c[i][j];
        }
    }

    // 配列 dp を定義
    const long long INF = 1LL << 60; // 無限大を表す値
    vector<long long> dp(N + 1, INF);

    // 初期値
    dp[0] = 0;

    // i が小さい順に dp の値を求めていく
    for(int i = 1; i <= N; i++) {
        for(int j = 0; j < i; j++) {
            dp[i] = min(dp[i], dp[j] + c[j][i]);
        }
    }

    // 答えを求める
    cout << dp[N] << endl;
    return 0;
}

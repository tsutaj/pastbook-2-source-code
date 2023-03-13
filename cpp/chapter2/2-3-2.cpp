#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// 十分大きい値
const long long INF = 1LL << 60;

int main() {
    // 入力
    int N, M, K;
    cin >> N >> M >> K;
    vector<long long> A(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i];
    }

    // 前処理
    vector<vector<long long>> cost(N + 1, vector<long long>(M + 1));
    for(int i = 0; i < N; i++) {
        long long min_value = INF;
        long long max_value = 0;
        for(int j = i; j < min(N, M + i); j++) {
            min_value = min(min_value, A[j]);
            max_value = max(max_value, A[j]);
            cost[i][j - i + 1] = K + (max_value - min_value) * (j - i + 1);
        }
    }

    // DP
    vector<long long> dp(N + 1, INF);
    dp[0] = 0;
    for(int i = 1; i <= N; i++) {
        for(int j = max(i - M, 0); j < i; j++) {
            dp[i] = min(dp[i], dp[j] + cost[j][i - j]);
        }
    }
    cout << dp[N] << endl;
    return 0;
}

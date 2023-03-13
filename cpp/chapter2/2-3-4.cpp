#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 入力
    int N;
    cin >> N;
    vector<long long> a(N);
    for(int i = 0; i < N; i++) {
        cin >> a[i];
    }

    // 累積和を求める
    vector<long long> S(N + 1);
    for(int i = 0; i < N; i++) {
        S[i + 1] = S[i] + a[i];
    }

    // 配列 dp を定義する
    const long long INF = 1LL << 60; // 十分大きい値
    vector<vector<long long>> dp(N + 1, vector<long long>(N + 1, INF));

    // 初期条件
    for(int i = 0; i < N; i++) {
        dp[i][i + 1] = 0;
    }

    // dp[i][j] の値を順に求めていく
    for(int between = 2; between <= N; between++) {
        for(int i = 0; i < N; i++) {
            int j = i + between; // 区間の右端
            if(j > N) {
                break;
            }

            // 区間 [i, j) の分割位置で場合分けする
            for(int k = i + 1; k < j; k++) {
                dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j] + S[j] - S[i]);
            }
        }
    }

    // 答え
    cout << dp[0][N] << endl;
    return 0;
}

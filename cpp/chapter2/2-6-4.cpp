#include <algorithm>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>
using namespace std;

// 定数
const int MOD = 1000000007;

int main() {
    // 入力
    int N;
    cin >> N;
    string s;
    cin >> s;

    // 配列 dp を定義する
    vector<vector<long long>> dp(N + 1, vector<long long>(N + 1));

    // 初期条件
    for(int j = 0; j < N; j++) {
        dp[1][j] = 1;
    }

    // dp[i+1][j] を順に埋めていく
    for(int i = 1; i < N; i++) {
        // dp[i] の累積和をとる
        vector<long long> sumdp(N + 2);
        for(int j = 0; j <= N; j++) {
            sumdp[j + 1] = sumdp[j] + dp[i][j];
        }

        // dp[i+1][j] の値の更新
        for(int j = 0; j <= N - i; j++) {
            if(s[i - 1] == '<') {
                dp[i + 1][j] = (sumdp[N - i + 1] - sumdp[j + 1]) % MOD;
            } else {
                dp[i + 1][j] = (sumdp[j + 1]) % MOD;
            }

            if(dp[i + 1][j] < 0) {
                dp[i + 1][j] += MOD;
            }
        }
    }

    // 答え
    cout << dp[N][0] << endl;
    return 0;
}

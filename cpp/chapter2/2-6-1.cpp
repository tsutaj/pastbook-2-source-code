#include <algorithm>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>
using namespace std;

// 定数
const long long MOD = 1000000007;

// 配列 dp を定義
int dp[10010][2][110];

int main() {
    // 入力
    string K;
    cin >> K;
    int D;
    cin >> D;
    int N = K.size();

    // 初期条件
    dp[0][0][0] = 1;

    // dp[i+1][j][r] を順に求めていく
    for(int i = 0; i < N; i++) {
        int x = K[i] - '0';

        for(int r = 0; r < D; r++) {
            // dp[i][0] から dp[i+1][0] への遷移
            dp[i + 1][0][(r + x) % D] += dp[i][0][r];
            dp[i + 1][0][(r + x) % D] %= MOD;

            // dp[i][0] から dp[i+1][1] への遷移
            for(int j = 0; j < x; j++) {
                dp[i + 1][1][(r + j) % D] += dp[i][0][r];
                dp[i + 1][1][(r + j) % D] %= MOD;
            }

            // dp[i][1] から dp[i+1][1] への遷移
            for(int j = 0; j < 10; j++) {
                dp[i + 1][1][(r + j) % D] += dp[i][1][r];
                dp[i + 1][1][(r + j) % D] %= MOD;
            }
        }
    }

    // 答え（0 の場合が含まれるので除外する）
    int ans = (dp[N][0][0] + dp[N][1][0] - 1) % MOD;
    if(ans < 0) {
        ans += MOD;
    }
    cout << ans << endl;
    return 0;
}

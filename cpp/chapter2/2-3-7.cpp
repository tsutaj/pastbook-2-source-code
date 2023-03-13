#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// 数え上げの法
const int MOD = 1000000007;

int main() {
    // 入力
    int N;
    cin >> N;
    vector<vector<int>> a(N, vector<int>(N));
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            cin >> a[i][j];
        }
    }

    // 配列 dp を定義する
    vector<int> dp(1 << N);

    // 初期条件
    dp[0] = 1;

    // dp[S] の値を順に求めていく
    for(int S = 1; S < (1 << N); S++) {
        // 整数値 S で表される集合の要素数
        int i = __builtin_popcount(S);

        // 最後の男性 i - 1 に割り当てられる女性で場合分け
        for(int j = 0; j < N; j++) {
            if(a[i - 1][j] == 1 and (S & (1 << j)) > 0) {
                dp[S] += dp[S & ~(1 << j)];
                dp[S] %= MOD;
            }
        }
    }

    // 答え
    cout << dp[(1 << N) - 1] << endl;
    return 0;
}

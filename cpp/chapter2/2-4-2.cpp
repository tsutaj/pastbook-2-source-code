#include <iostream>
#include <vector>
using namespace std;

// 定数
const int MOD = 1000000007;

int main() {
    // 入力
    int N, K;
    cin >> N >> K;
    vector<int> a(N);
    for(int i = 0; i < N; i++) {
        cin >> a[i];
    }

    // 配列 dp, sum_dp を宣言する
    vector<vector<int>> dp(N + 1, vector<int>(K + 1));
    vector<vector<int>> sum_dp(N + 1, vector<int>(K + 2));

    // 初期条件
    dp[0][0] = 1;

    // dp[i+1][j] の値を順に求めていく
    for(int i = 0; i < N; i++) {
        // dp[i] の累積和を求める
        for(int j = 0; j <= K; j++) {
            sum_dp[i][j + 1] = (sum_dp[i][j] + dp[i][j]) % MOD;
        }

        for(int j = 0; j <= K; j++) {
            dp[i + 1][j] =
                (sum_dp[i][j + 1] - sum_dp[i][max(j - a[i], 0)]) % MOD;
            if(dp[i + 1][j] < 0) {
                dp[i + 1][j] += MOD;
            }
        }
    }

    // 出力
    cout << dp[N][K] << endl;
    return 0;
}

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// 定数
const int MOD = 10007;

int main() {
    // 入力を受け取る
    int N;
    cin >> N;
    string responsible;
    cin >> responsible;

    // 配列 dp を定義
    vector<vector<int>> dp(N + 1, vector<int>(8));

    // 初期値 (左上マスが黒色であることを表す)
    dp[0][1 << 0] = 1;

    // d が小さい順に dp の値を求めていく
    for(int d = 0; d < N; d++) {
        for(int S = 0; S < 8; S++) {
            // d 列目は S[d] を責任者に持つ必要がある
            if(responsible[d] == 'J' && (S & (1 << 0)) == 0) {
                continue;
            }
            if(responsible[d] == 'O' && (S & (1 << 1)) == 0) {
                continue;
            }
            if(responsible[d] == 'I' && (S & (1 << 2)) == 0) {
                continue;
            }

            // d-1 列目の塗られ方で場合分けする
            for(int T = 0; T < 8; T++) {
                // 左右に黒色マスが隣接している箇所があるかを確認
                if((S & T) != 0) {
                    dp[d + 1][S] = (dp[d + 1][S] + dp[d][T]) % MOD;
                }
            }
        }
    }
    int ans = 0;
    for(int i = 0; i < 8; i++) {
        ans += dp[N][i];
        ans %= MOD;
    }
    cout << ans << endl;
    return 0;
}

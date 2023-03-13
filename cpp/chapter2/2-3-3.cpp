#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 入力
    int M, N;
    cin >> M >> N;
    string S, T;
    cin >> S >> T;

    // 十分大きい値
    const int INF = max(M, N);

    // DP
    vector<vector<int>> dp(M + 1, vector<int>(N + 1, INF));

    // 初期条件
    dp[0][0] = 0;

    // dp[i][j] の値を順に更新していく
    for(int i = 0; i <= M; i++) {
        for(int j = 0; j <= N; j++) {
            // 場合 1：変更
            if(i > 0 && j > 0) {
                dp[i][j] =
                    min(dp[i][j], dp[i - 1][j - 1] + (S[i - 1] != T[j - 1]));
            }

            // 場合 2：削除
            if(i > 0) {
                dp[i][j] = min(dp[i][j], dp[i - 1][j] + 1);
            }

            // 場合 3：挿入
            if(j > 0) {
                dp[i][j] = min(dp[i][j], dp[i][j - 1] + 1);
            }
        }
    }

    // 出力
    cout << dp[M][N] << endl;
    return 0;
}

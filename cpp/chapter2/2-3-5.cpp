#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 入力
    string S;
    cin >> S;
    int N = S.size();

    // 配列 dp を定義する
    vector<vector<int>> dp(N + 1, vector<int>(N + 1));

    // dp[i][j] の値を順に更新していく
    for(int between = 2; between <= N; between++) {
        for(int i = 0; i < N; i++) {
            int j = i + between; // 区間の右端
            if(j > N) {
                break;
            }

            // 場合 1
            if(S[i] == 'i' && S[j - 1] == 'i') {
                for(int k = i + 1; k < j - 1; k++) {
                    if(S[k] == 'w' && dp[i + 1][k] * 3 == k - i - 1 &&
                       dp[k + 1][j - 1] * 3 == j - k - 2) {
                        dp[i][j] = (j - i) / 3;
                    }
                }
            }

            // 場合 2
            for(int k = i + 1; k < j - 1; k++) {
                dp[i][j] = max(dp[i][j], dp[i][k] + dp[k][j]);
            }
        }
    }

    // 出力
    cout << dp[0][N] << endl;
    return 0;
}

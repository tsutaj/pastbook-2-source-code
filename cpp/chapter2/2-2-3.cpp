#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 入力
    int N;
    cin >> N;
    string S;
    cin >> S;
    vector<long long> C(N), D(N);
    for(int i = 0; i < N; i++) {
        cin >> C[i];
    }
    for(int i = 0; i < N; i++) {
        cin >> D[i];
    }

    // 配列 dp を定義する
    const long long INF = 1LL << 60; // 十分大きい値
    vector<vector<long long>> dp(N + 1, vector<long long>(N + 1, INF));

    // 初期条件
    dp[0][0] = 0;

    // dp[i + 1][j] の値を順に求めていく
    for(int i = 0; i < N; i++) {
        for(int j = 0; j <= N; j++) {
            // S[i] に何もしない場合
            if(S[i] == '(' && j > 0) {
                dp[i + 1][j] = min(dp[i + 1][j], dp[i][j - 1]);
            }
            if(S[i] == ')' && j < N) {
                dp[i + 1][j] = min(dp[i + 1][j], dp[i][j + 1]);
            }

            // S[i] を変更するとき
            if(S[i] == '(' && j < N) {
                dp[i + 1][j] = min(dp[i + 1][j], dp[i][j + 1] + C[i]);
            }
            if(S[i] == ')' && j > 0) {
                dp[i + 1][j] = min(dp[i + 1][j], dp[i][j - 1] + C[i]);
            }

            // S[i] を削除するとき
            if(S[i] == '(') {
                dp[i + 1][j] = min(dp[i + 1][j], dp[i][j] + D[i]);
            }
            if(S[i] == ')') {
                dp[i + 1][j] = min(dp[i + 1][j], dp[i][j] + D[i]);
            }
        }
    }

    // 出力
    cout << dp[N][0] << endl;
    return 0;
}

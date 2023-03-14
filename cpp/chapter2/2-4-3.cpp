#include <iostream>
#include <vector>
using namespace std;

// 定数
const long long MOD = 1000000007;

int main() {
    // 入力
    int H, W;
    cin >> H >> W;
    vector<string> S(H);
    for(int i = 0; i < H; i++) {
        cin >> S[i];
    }

    // 配列 dp, X, Y, Z を定義する
    vector<vector<long long>> dp(H, vector<long long>(W));
    vector<vector<long long>> X(H + 1, vector<long long>(W + 1));
    vector<vector<long long>> Y(H + 1, vector<long long>(W + 1));
    vector<vector<long long>> Z(H + 1, vector<long long>(W + 1));

    // 初期条件
    dp[0][0] = 1;
    X[0][1] = Y[1][0] = Z[1][1] = 1;

    // dp[r][c] の値を順に求めていく
    for(int r = 0; r < H; r++) {
        for(int c = 0; c < W; c++) {
            // マス (r, c) が壁であるとき
            if(S[r][c] == '#') {
                X[r][c + 1] = 0;
                Y[r + 1][c] = 0;
                Z[r + 1][c + 1] = 0;
                continue;
            }

            // dp[r][c] の値を更新する
            dp[r][c] = (dp[r][c] + X[r][c] + Y[r][c] + Z[r][c]) % MOD;

            // X, Y, Z を更新する
            X[r][c + 1] = (X[r][c] + dp[r][c]) % MOD;
            Y[r + 1][c] = (Y[r][c] + dp[r][c]) % MOD;
            Z[r + 1][c + 1] = (Z[r][c] + dp[r][c]) % MOD;
        }
    }

    // 出力
    cout << dp[H - 1][W - 1] << endl;
    return 0;
}

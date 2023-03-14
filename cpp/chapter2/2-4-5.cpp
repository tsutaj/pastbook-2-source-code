#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

// 定数
const long long MOD = 1000000007;

// 行列乗算を実施する関数
using Matrix = vector<vector<long long>>;
Matrix mul(const Matrix &A, const Matrix &B) {
    int N = A.size();
    Matrix C(N, vector<long long>(N));
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            for(int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
                C[i][j] %= MOD;
            }
        }
    }
    return C;
}

int main() {
    // 入力
    int H, R;
    cin >> H >> R;
    vector<vector<int>> G(R, vector<int>(R));
    for(int i = 0; i < R; i++) {
        for(int j = 0; j < R; j++) {
            cin >> G[i][j];
        }
    }

    // 単位行列
    Matrix UNIT(R, vector<long long>(R));
    for(int i = 0; i < R; i++) {
        UNIT[i][i] = 1;
    }

    // 行列 M を求める
    Matrix M(R, vector<long long>(R));
    for(int s = 0; s < R; s++) {
        // 配列 dp を定義する
        vector<vector<long long>> dp(1 << R, vector<long long>(R));

        // 初期条件
        dp[1 << s][s] = 1;

        // dp[S][i] を順に求めていく
        for(int S = 0; S < (1 << R); S++) {
            for(int i = 0; i < R; i++) {
                if((S & (1 << i)) == 0) {
                    continue;
                }

                for(int j = 0; j < R; j++) {
                    if(i == j || (S & (1 << j)) == 0) {
                        continue;
                    }
                    if(G[j][i] == 1) {
                        dp[S][i] += dp[S & ~(1 << i)][j];
                        dp[S][i] %= MOD;
                    }
                }
            }
        }

        // M に反映させる
        for(int t = 0; t < R; t++) {
            for(int S = 0; S < (1 << R); S++) {
                M[s][t] = (M[s][t] + dp[S][t]) % MOD;
            }
        }
    }

    // M^K を計算する
    auto res = UNIT; // 答え
    auto T = M;      // M^(2^n) の途中経過
    while(H > 0) {
        if(H % 2 == 1) {
            res = mul(res, T);
        }
        T = mul(T, T);
        H /= 2;
    }

    // 答え
    cout << res[0][0] << endl;
    return 0;
}

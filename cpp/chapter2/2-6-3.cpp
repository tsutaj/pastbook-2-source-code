#include <algorithm>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>
using namespace std;

// 定数
const long long INF = 1LL << 60;

// 自身と上下左右を表す配列
const int DX[] = {0, 1, 0, -1, 0};
const int DY[] = {0, 0, 1, 0, -1};

int main() {
    // 入力を受け取り、整数値に直す
    vector<string> row_grid(4);
    for(int i = 0; i < 4; i++) {
        cin >> row_grid[i];
    }
    int grid = 0;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(row_grid[i][j] == '#') {
                grid |= (1 << (i * 4 + j));
            }
        }
    }

    // 配列 dp を定義する
    vector<double> dp(1 << 16, INF);

    // 初期条件
    dp[0] = 0.0;

    // dp[S] の値を順に埋めていく
    for(int S = 0; S < (1 << 16); S++) {
        // 狙うマスを順に試す
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                // 自身と周囲 5 マスにボールが当たった後のグリッドを求める
                vector<int> S2s;

                // ボールが当たるマスを順に調べる
                for(int k = 0; k < 5; k++) {
                    int dx = DX[k], dy = DY[k];
                    int i2 = i + dx, j2 = j + dy;

                    // ボールがグリッド外に出る場合は変化しない
                    if(i2 < 0 || i2 >= 4 || j2 < 0 || j2 >= 4) {
                        continue;
                    }

                    // ボールが的のないマスに当たる場合も変化しない
                    if((S & (1 << (i2 * 4 + j2))) == 0) {
                        continue;
                    }

                    // 新たなグリッド
                    int S2 = S & ~(1 << (i2 * 4 + j2));
                    S2s.emplace_back(S2);
                }

                // dp[S] を求める
                if(S2s.size() > 0) {
                    double sum_dp = 0.0;
                    for(int x : S2s) {
                        sum_dp += dp[x];
                    }
                    dp[S] = min(dp[S], sum_dp / S2s.size() + 5.0 / S2s.size());
                }
            }
        }
    }

    // 答え
    printf("%.12f\n", dp[grid]);
    return 0;
}

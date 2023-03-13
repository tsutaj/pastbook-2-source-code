#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 入力
    int N;
    cin >> N;
    vector<vector<long long>> A(N, vector<long long>(N));
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            cin >> A[i][j];
        }
    }

    // 配列 dp を定義する
    const long long INF = 1LL << 60;
    vector<vector<long long>> dp(1 << N, vector<long long>(N, INF));

    // 初期条件
    dp[1][0] = 0;

    // dp[S][i] の値を順に求めていく
    for(int S = 0; S < (1 << N); S++) {
        for(int i = 0; i < N; i++) {
            // 都市 i が S に含まれない場合はスキップ'
            if((S & (1 << i)) == 0) {
                continue;
            }

            // 都市 j で場合分け
            for(int j = 0; j < N; j++) {
                if(j == i || (S & (1 << j)) == 0) {
                    continue;
                }

                // 更新式
                dp[S][i] = min(dp[S][i], dp[S & ~(1 << i)][j] + A[j][i]);
            }
        }
    }

    // 答え（最後に都市 0 に戻るケースをすべて探索する）
    long long ans = INF;
    for(int i = 1; i < N; i++) {
        ans = min(ans, dp[(1 << N) - 1][i] + A[i][0]);
    }
    cout << ans << endl;
    return 0;
}

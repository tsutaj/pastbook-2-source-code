#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 入力
    int N, M;
    cin >> N >> M;
    vector<int> A(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i], A[i]--;
    }

    // 前処理
    vector<vector<int>> num(M, vector<int>(N + 1));
    for(int i = 0; i < M; i++) {
        for(int j = 0; j < N; j++) {
            num[i][j + 1] = num[i][j] + (A[j] == i);
        }
    }

    // 配列 dp を宣言する
    const long long INF = 1LL << 60; // 十分大きい値
    vector<long long> dp(1 << M, INF);

    // 初期条件
    dp[0] = 0;

    // dp[S] の値を順に求めていく
    for(int S = 1; S < (1 << M); S++) {
        // r：数列 A の要素にうち、整数値 S で表される集合に含まれる個数
        int right = 0;
        for(int i = 0; i < M; i++) {
            if((S & (1 << i)) > 0) {
                right += num[i][N];
            }
        }

        // 整数値 S で表される集合の要素のうち、最後に並べる要素で場合分け
        for(int i = 0; i < M; i++) {
            if((S & (1 << i)) == 0) {
                continue;
            }

            // right のうち、要素値が i でないものの個数
            int left = right - num[i][N];

            // 更新式
            int add = right - left - (num[i][right] - num[i][left]);
            dp[S] = min(dp[S], dp[S & ~(1 << i)] + add);
        }
    }

    // 出力
    cout << dp[(1 << M) - 1] << endl;
    return 0;
}

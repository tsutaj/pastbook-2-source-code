#include <algorithm>
#include <iostream>
#include <numeric>
#include <tuple>
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

    // 前計算によって、集合 X をグループ化したときのスコアを求める
    vector<long long> score(1 << N);
    for(int S = 0; S < (1 << N); S++) {
        for(int i = 0; i < N; i++) {
            for(int j = i + 1; j < N; j++) {
                if((S & (1 << i)) == 0 || (S & (1 << j)) == 0) {
                    continue;
                }
                score[S] += A[i][j];
            }
        }
    }

    // 配列 dp を定義する
    vector<long long> dp(1 << N);
    for(int S = 0; S < (1 << N); S++) {
        int T = S;
        while(T >= 0) {
            T &= S;
            dp[S] = max(dp[S], dp[T] + score[S - T]);
            T--;
        }
    }

    // 答え
    cout << dp[(1 << N) - 1] << endl;
    return 0;
}

#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    vector<int> A(N), B(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i] >> B[i];
    }
    vector<int> C(M), D(M);
    for(int i = 0; i < M; i++) {
        cin >> C[i] >> D[i];
    }

    // 組み合わせが存在することを確認した X の値
    double X_ok = 0.0;
    // 組み合わせが存在しないことを確認した X の値
    double X_ng = 200000.0;

    for(int iter = 0; iter < 100; iter++) {
        // 二分探索の 1 回の試行
        double X = (X_ok + X_ng) / 2;

        // 各モンスター i について P[i] を計算する
        vector<double> P(N);
        for(int i = 0; i < N; i++) {
            P[i] = B[i] - A[i] * X;
        }

        // 各お助けモンスター i について Q[i] を計算する
        vector<double> Q(M);
        for(int i = 0; i < M; i++) {
            Q[i] = D[i] - C[i] * X;
        }

        // P, Q を大きい順にソートする
        sort(P.rbegin(), P.rend());
        sort(Q.rbegin(), Q.rend());

        // お助けモンスターを使わない場合
        double sum1 = accumulate(P.begin(), P.begin() + 5, 0.0);
        // お助けモンスターを使う場合
        double sum2 = accumulate(P.begin(), P.begin() + 4, 0.0) + Q[0];

        if(sum1 >= 0 || sum2 >= 0) {
            // 合成後のモンスターの強さが X 以上となるような
            // モンスターの組み合わせは存在する
            X_ok = X;
        } else {
            // 合成後のモンスターの強さが X 以上となるような
            // モンスターの組み合わせは存在しない
            X_ng = X;
        }
    }

    printf("%.12f\n", X_ok);
    return 0;
}

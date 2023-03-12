#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

// 平均値の最大値を二分探索で求める
double calc_max_average(int N, vector<int> &A) {
    double X_ok = 0.0, X_ng = 1e10;
    for(int iter = 0; iter < 100; iter++) {
        double X = (X_ok + X_ng) / 2.0;

        vector<double> B(N);
        for(int i = 0; i < N; i++) {
            B[i] = A[i] - X;
        }

        vector<double> S(N + 1), T(N + 1);
        for(int i = 0; i < N; i++) {
            S[i + 1] = max(T[i], S[i]) + B[i];
            T[i + 1] = S[i];
        }

        if(max(S[N], T[N]) >= 0) {
            X_ok = X;
        } else {
            X_ng = X;
        }
    }
    return X_ok;
}

// 中央値の最大値を二分探索で求める
int calc_max_median(int N, vector<int> &A) {
    int X_ok = 0, X_ng = 1 << 30;
    while(X_ng - X_ok > 1) {
        int X = (X_ok + X_ng) / 2;

        vector<int> B(N);
        for(int i = 0; i < N; i++) {
            if(A[i] >= X) {
                B[i] = 1;
            } else {
                B[i] = -1;
            }
        }

        vector<int> S(N + 1), T(N + 1);
        for(int i = 0; i < N; i++) {
            S[i + 1] = max(T[i], S[i]) + B[i];
            T[i + 1] = S[i];
        }
        if(max(S[N], T[N]) > 0) {
            X_ok = X;

        } else {
            X_ng = X;
        }
    }
    return X_ok;
}

int main() {
    int N;
    cin >> N;
    vector<int> A(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i];
    }

    double average = calc_max_average(N, A);
    printf("%.12f\n", average);

    int median = calc_max_median(N, A);
    printf("%d\n", median);
    return 0;
}

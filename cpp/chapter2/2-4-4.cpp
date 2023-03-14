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
    int N;
    long long K;
    cin >> N >> K;
    Matrix M(N, vector<long long>(N));
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            cin >> M[i][j];
        }
    }

    // 単位行列
    Matrix UNIT(N, vector<long long>(N));
    for(int i = 0; i < N; i++) {
        UNIT[i][i] = 1;
    }

    // M^K を計算する
    auto R = UNIT; // 答え
    auto T = M;    // M^(2^n) の途中経過
    while(K > 0) {
        if(K % 2 == 1) {
            R = mul(R, T);
        }
        T = mul(T, T);
        K /= 2;
    }

    // 答え
    long long ans = 0;
    for(int i = 0; i < N; i++) {
        ans += accumulate(R[i].begin(), R[i].end(), 0LL);
        ans %= MOD;
    }
    cout << ans << endl;
    return 0;
}

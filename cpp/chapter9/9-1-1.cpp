#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    long long N, K;
    cin >> N >> K;
    vector<long long> A(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i];
    }

    // 累積和を表す配列
    vector<long long> S(N + 1);
    for(int i = 0; i < N; i++) {
        S[i + 1] = S[i] + A[i];
    }
    // S はソートしてしまってしまってよい
    sort(S.begin(), S.end());

    // 条件を満たす (i,j) の個数が 2K 以上になるような X の最小値を
    // 二分探索で求める
    long long ok = 1e15;
    long long ng = -1;
    while(ok - ng > 1) {
        long long X = (ok + ng) / 2;
        // 各 j について S[j]-X <= S[i] <= S[j]+X となる
        // i の数を数える
        long long count = 0;
        int L = 0;
        int R = 0;
        for(int j = 0; j <= N; j++) {
            while(S[L] < S[j] - X) {
                L++;
            }
            while(R < N && S[R + 1] <= S[j] + X) {
                R++;
            }
            count += R - L;
        }
        if(count >= 2 * K) {
            ok = X;
        } else {
            ng = X;
        }
    }
    cout << ok << endl;
    return 0;
}

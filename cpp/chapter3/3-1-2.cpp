#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

int main() {
    int N;
    cin >> N;

    // 昇順にソートしておく
    vector<int> A(N), B(N), C(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i];
    }
    for(int i = 0; i < N; i++) {
        cin >> B[i];
    }
    for(int i = 0; i < N; i++) {
        cin >> C[i];
    }
    sort(A.begin(), A.end());
    sort(B.begin(), B.end());
    sort(C.begin(), C.end());

    // b より小さい最大の a と、b より大きい最小の c の位置を管理
    // 数列はソート済みであるので、これらの値は単調に増加する
    int index_a = 0;
    int index_c = 0;

    long long ans = 0;
    for(int i = 0; i < N; i++) {
        while(index_a < N && A[index_a] < B[i]) {
            index_a++;
        }
        while(index_c < N && C[index_c] <= B[i]) {
            index_c++;
        }

        // a の取りうる範囲: [0, index_a)
        int range_a = index_a;
        // c の取りうる範囲: [index_c, N)
        int range_c = N - index_c;
        ans += 1LL * range_a * range_c;
    }
    cout << ans << endl;
    return 0;
}

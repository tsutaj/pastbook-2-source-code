#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

int main() {
    int N;
    cin >> N;
    vector<int> A(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i];
    }

    int r = 0;
    long long v_xor = 0, v_sum = 0;
    long long ans = 0;
    for(int l = 0; l < N; l++) {
        while(r < N && (v_xor ^ A[r]) == (v_sum + A[r])) {
            v_xor ^= A[r];
            v_sum += A[r];
            r++;
        }
        ans += r - l;

        // 長さが 1 ならば必ず条件を満たすので、長さが正かどうかの場合分けは不要
        v_xor ^= A[l];
        v_sum -= A[l];
    }
    cout << ans << endl;
    return 0;
}

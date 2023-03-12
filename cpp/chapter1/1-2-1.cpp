#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

int main() {
    long long N, L;
    cin >> N >> L;
    vector<long long> A(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i];
    }
    vector<long long> S(N + 1);
    for(int i = 0; i < N; i++) {
        S[i + 1] = S[i] + A[i];
    }

    long long X_ok = 0, X_ng = L + 1;
    while(X_ng - X_ok > 1) {
        long long X = (X_ng + X_ok) / 2;

        int l = 0, r = 0;
        vector<int> C(N + 1), D(N + 1);
        D[0] += 1;
        D[1] -= 1;
        for(int i = 0; i < N; i++) {
            if(i > 0) {
                C[i] = C[i - 1] + D[i];
            } else {
                C[i] = D[i];
            }
            if(C[i] == 0) {
                continue;
            }

            while(l <= N && S[l] - S[i] < X) {
                l++;
            }
            if(l > N) {
                continue;
            }

            while(r + 1 <= N && S[r + 1] - S[i] <= L) {
                r++;
            }
            D[l]++;
            if(r + 1 <= N) {
                D[r + 1]--;
            }
        }
        C[N] = C[N - 1] + D[N];

        if(C[N] > 0) {
            X_ok = X;
        } else {
            X_ng = X;
        }
    }

    cout << X_ok << endl;
    return 0;
}

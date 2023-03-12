#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

int main() {
    string S;
    cin >> S;
    int K;
    cin >> K;

    int N = S.size(), r = 0;
    int dots = 0, ans = 0;
    for(int l = 0; l < N; l++) {
        // 区間の中の '.' の個数が K 以下なら、区間を伸ばして良い
        while(r < N) {
            int is_dot = (S[r] == '.');
            if(dots + is_dot > K) {
                break;
            }
            dots += is_dot;
            r++;
        }

        ans = max(ans, r - l);
        if(r - l >= 1) {
            dots -= (S[l] == '.');
        } else {
            r++;
        }
    }
    cout << ans << endl;
    return 0;
}

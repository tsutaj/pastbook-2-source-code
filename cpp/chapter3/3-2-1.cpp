#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <tuple>
#include <vector>
using namespace std;

int main() {
    int N;
    long long K;
    cin >> N >> K;

    bool exists_zero = false;
    vector<long long> S(N);
    for(int i = 0; i < N; i++) {
        cin >> S[i];
        if(S[i] == 0) {
            exists_zero = true;
        }
    }

    // 0 が含まれているときは、すべての要素を含んだ区間が答えとなる
    if(exists_zero) {
        cout << N << endl;
        return 0;
    }

    // 0-indexed で処理
    int r = 0;
    long long prod = 1;
    int ans = 0;

    // l は固定 (最も長い区間を求め終えるたびに 1 ずつ増やす)
    for(int l = 0; l < N; l++) {
        // 条件を満たしている限り、r を 1 ずつ増やし続ける (区間を伸ばす)
        while(r < N && prod * S[r] <= K) {
            prod *= S[r];
            r++;
        }
        // 左端を固定したときの、条件を満たす中で最も長い区間が求められた
        ans = max(ans, r - l);
        // l の値を 1 増やす (区間を縮める)
        // l と r が等しい場合に注意！
        if(r - l >= 1) {
            prod /= S[l];
        } else {
            r++;
        }
    }

    cout << ans << endl;
    return 0;
}

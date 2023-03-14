#include <iostream>
#include <vector>
using namespace std;

// 定数
const int MOD = 1234567;

int main() {
    // 入力
    int N, P;
    cin >> N >> P;
    vector<int> H(N);
    for(int i = 0; i < N; i++)
        cin >> H[i];

    // 各 i に対して、数列 H の区間 [x, i) の総和が
    // P 以下になる最小の x を求める
    vector<int> x(N + 1);
    int left = 0;
    int sum_interval = 0; // 現在考えている区間の総和
    for(int right = 0; right <= N; right++) {
        // 区間 [left, right) の総和が P より大きい限り、left を増やす
        while(left < right && sum_interval > P) {
            sum_interval -= H[left];
            left++;
        }

        // 解を記録
        x[right] = left;

        // 区間の右端 right を増やすことに伴い、区間の総和に足す
        if(right < N) {
            sum_interval += H[right];
        }
    }

    // 配列 dp, sum_dp を宣言する
    vector<int> dp(N + 1), sum_dp(N + 2);

    // 初期条件
    dp[0] = 1;
    sum_dp[1] = 1;

    // dp[i] の値を順に決めていく
    for(int i = 1; i <= N; i++) {
        dp[i] = (sum_dp[i] - sum_dp[x[i]]) % MOD;
        if (dp[i] < 0) {
            dp[i] += MOD;
        }
        sum_dp[i + 1] = (sum_dp[i] + dp[i]) % MOD;
    }

    // 出力
    cout << dp[N] << endl;
    return 0;
}

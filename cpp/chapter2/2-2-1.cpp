#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// 定数
const int MOD = 1000000007;
const string T = "atcoder";

// a に b を足して MOD をとる関数
int add(int a, int b) {
    a += b;
    if(a >= MOD) {
        a -= MOD;
    }
    return a;
}

int main() {
    // 入力
    int N;
    cin >> N;
    string S;
    cin >> S;

    // 配列 dp を宣言する
    vector<vector<int>> dp(N + 1, vector<int>(T.size() + 1));

    // 初期条件
    dp[0][0] = 1;

    // dp[i + 1][j] の値を順に求めていく
    for(int i = 0; i < N; i++) {
        for(int j = 0; j <= (int)T.size(); j++) {
            // S[i] を選ばない場合
            dp[i + 1][j] = add(dp[i + 1][j], dp[i][j]);

            // S[i] を選ぶ場合
            if(j > 0 && S[i] == T[j - 1]) {
                dp[i + 1][j] = add(dp[i + 1][j], dp[i][j - 1]);
            }
        }
    }
    cout << dp[N][T.size()] << endl;
    return 0;
}

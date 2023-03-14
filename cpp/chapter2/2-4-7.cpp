#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include <tuple>
using namespace std;

int main() {
    // 入力
    int N;
    cin >> N;
    vector<tuple<int, int, int>> blocks(N);
    for(int i = 0; i < N; i++) {
        int w, s, v;
        cin >> w >> s >> v;
        blocks[i] = make_tuple(w, s, v);
    }

    // ブロックを所定の順序に並び替える
    sort(blocks.begin(), blocks.end(), [](auto b1, auto b2) {
        auto [w1, s1, v1] = b1;
        auto [w2, s2, v2] = b2;
        return min(s1, s2 - w1) > min(s2, s1 - w2);
    });

    // 配列 dp を定義する
    const int MAX_S = 20000;
    vector<vector<long long>> dp(N + 1, vector<long long>(MAX_S + 1));

    // dp[i + 1] の値を順に埋めていく
    for(int i = 0; i < N; i++) {
        auto [w, s, v] = blocks[i];
        for(int j = 0; j <= MAX_S; j++) {
            dp[i + 1][j] = max(dp[i + 1][j], dp[i][j]);
            if(j <= s) {
                dp[i + 1][j + w] = max(dp[i + 1][j + w], dp[i][j] + v);
            }
        }
    }

    // 答え
    cout << *max_element(dp[N].begin(), dp[N].end()) << endl;
    return 0;
}

#include <algorithm>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>
using namespace std;

// v: 現在探索中の頂点, p: v の親（v が根のときは p = -1）
void dfs(const vector<vector<int>> &G, vector<int> &dp, int v, int p) {
    dp[v] = 1;

    // v に隣接した頂点を見ていく
    for(int c : G[v]) {
        // c が親である場合は除く
        if(c == p) {
            continue;
        }

        // 子頂点 c を再帰的に探索する
        dfs(G, dp, c, v);

        // dp[v] の値を更新する
        dp[v] += dp[c];
    }
}

int main() {
    // 入力
    int N;
    cin >> N;
    vector<vector<int>> G(N);
    for(int i = 0; i < N - 1; i++) {
        int x, y;
        cin >> x >> y;
        x--, y--;
        G[x].emplace_back(y);
        G[y].emplace_back(x);
    }

    // 配列 dp を定義する
    vector<int> dp(N);

    // 根である頂点 0 を呼び出す
    dfs(G, dp, 0, -1);

    // 答え
    for(int v = 0; v < N; v++) {
        cout << dp[v] << endl;
    }
    return 0;
}

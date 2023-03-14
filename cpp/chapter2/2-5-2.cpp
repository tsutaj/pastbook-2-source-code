#include <algorithm>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>
using namespace std;

// 定数
const long long MOD = 1000000007;

// v: 現在探索中の頂点, p: v の親（v が根のときは p = -1）
void dfs(const vector<vector<int>> &G, vector<long long> &dp_white,
         vector<long long> &dp_black, int v, int p) {
    // v に隣接した頂点を見ていく
    for(int c : G[v]) {
        // c が親である場合は除く
        if(c == p) {
            continue;
        }

        // 子頂点 c を再帰的に探索する
        dfs(G, dp_white, dp_black, c, v);

        // dp[v] の値を更新する
        dp_white[v] *= dp_white[c] + dp_black[c];
        dp_black[v] *= dp_white[c];
        dp_white[v] %= MOD;
        dp_black[v] %= MOD;
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
    vector<long long> dp_white(N, 1);
    vector<long long> dp_black(N, 1);

    // 根である頂点 0 を呼び出す
    dfs(G, dp_white, dp_black, 0, -1);

    // 答え
    cout << (dp_white[0] + dp_black[0]) % MOD << endl;
    return 0;
}

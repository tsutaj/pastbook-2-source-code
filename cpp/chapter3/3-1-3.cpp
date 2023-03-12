#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <tuple>
#include <vector>
using namespace std;

void impossible() {
    puts("-1");
    exit(0);
}

int main() {
    int N, M;
    cin >> N >> M;
    vector<vector<pair<int, int>>> G(N);
    vector<tuple<int, int, int>> edges(M);

    for(int i = 0; i < M; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        a--, b--;
        G[a].emplace_back(b, c);
        G[b].emplace_back(a, c);
        edges[i] = make_tuple(a, b, c);
    }

    // 頂点 0 (0-indexed) に x を書き込んだとする。他の頂点は何を書き込めるか？
    long long INF = 1LL << 60;
    vector<long long> positive(N), negative(N);
    vector<bool> exists_posi(N), exists_nega(N);
    vector<bool> checked(N);

    // x としてあり得る範囲 [lbound, ubound]
    long long lbound = 0, ubound = INF;

    // BFS で頂点を探索
    queue<int> que;
    que.emplace(0);
    exists_posi[0] = true;
    while(que.size()) {
        int u = que.front();
        que.pop();
        for(auto [v, c] : G[u]) {
            bool updated = false;

            // 頂点 u で positive[u] + x という値だったとき
            // 頂点 v では c - positive[u] - x となり、これが 0 以上になる
            // x <= c - positive[u]
            if(exists_posi[u] && !exists_nega[v]) {
                exists_nega[v] = true;
                negative[v] = c - positive[u];
                ubound = min(ubound, c - positive[u]);
                updated = true;
            }
            // 頂点 u で negative[u] - x という値だったとき
            // 頂点 v では c - negative[u] + x となり、これが 0 以上になる
            // x >= negative[u] - c
            if(exists_nega[u] && !exists_posi[v]) {
                exists_posi[v] = true;
                positive[v] = c - negative[u];
                lbound = max(lbound, negative[u] - c);
                updated = true;
            }
            if(updated) {
                que.emplace(v);
            }
        }
    }

    if(lbound > ubound) {
        impossible();
    }

    vector<long long> ans(N);
    long long x = ubound;
    // negative[i] - x = positive[i] + x より、
    // negative と positive の情報が両方あれば x の値は一意に定まる
    for(int i = 0; i < N; i++) {
        if(exists_posi[i] && exists_nega[i]) {
            if((negative[i] - positive[i]) % 2 != 0) {
                impossible();
            }
            x = (negative[i] - positive[i]) / 2;
            break;
        }
    }

    for(int i = 0; i < N; i++) {
        if(exists_posi[i]) {
            ans[i] = positive[i] + x;
        } else if(exists_nega[i]) {
            ans[i] = negative[i] - x;
        }

        if(ans[i] < 0) {
            impossible();
        }
    }

    // 必要十分ではないため、最後にチェック
    for(auto [a, b, c] : edges) {
        if(ans[a] + ans[b] != c) {
            impossible();
        }
    }
    for(int i = 0; i < N; i++) {
        cout << ans[i] << endl;
    }
    return 0;
}

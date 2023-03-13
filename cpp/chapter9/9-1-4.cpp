#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
using namespace std;

class LowestCommonAncestor {
  public:
    using EdgeCostTp = int;

    LowestCommonAncestor(int n) {
        n_ = n;
        logn_ = int(log2(n_) + 2.0);
        depth_.resize(n_, 0);
        distance_.resize(n_, EdgeCostTp(0));
        ancestor_.resize(logn_, vector<int>(n_, -1));
        edges_.resize(n_);
    }

    // (u, v) 間 (0-indexed) に重み w の辺を追加
    void add_edge(int u, int v, EdgeCostTp w = 1) {
        edges_[u].emplace_back(v, w);
        edges_[v].emplace_back(u, w);
    }

    // 根を root にした木に対して計算
    void build(int root = 0) {
        stack<int> st;
        st.emplace(root);
        while(st.size()) {
            int cur = st.top();
            st.pop();
            for(auto [nxt, w] : edges_[cur]) {
                if(ancestor_[0][nxt] != cur && ancestor_[0][cur] != nxt) {
                    ancestor_[0][nxt] = cur;
                    depth_[nxt] = depth_[cur] + 1;
                    distance_[nxt] = distance_[cur] + w;
                    st.emplace(nxt);
                }
            }
        }

        for(int k = 1; k < logn_; k++) {
            for(int i = 0; i < n_; i++) {
                if(ancestor_[k - 1][i] == -1) {
                    ancestor_[k][i] = -1;
                } else {
                    ancestor_[k][i] = ancestor_[k - 1][ancestor_[k - 1][i]];
                }
            }
        }
    }

    // u と v (0-indexed) の LCA を求める
    int lca(int u, int v) {
        // u よりも v のほうが深い状態にしておく
        if(depth_[u] > depth_[v]) {
            swap(u, v);
        }

        // v の深さを u に合わせる
        for(int k = logn_ - 1; k >= 0; k--) {
            if(((depth_[v] - depth_[u]) >> k) & 1) {
                v = ancestor_[k][v];
            }
        }

        // この時点で一致していれば、それが答え
        if(u == v) {
            return u;
        }

        // u, v がギリギリ一致しないように親方向に登る
        for(int k = logn_ - 1; k >= 0; k--) {
            if(ancestor_[k][u] != ancestor_[k][v]) {
                u = ancestor_[k][u];
                v = ancestor_[k][v];
            }
        }

        // 最後に 1 ステップ親方向に登ったところが LCA になる
        return ancestor_[0][u];
    }

    EdgeCostTp distance(int u, int v) {
        return distance_[u] + distance_[v] - 2 * distance_[lca(u, v)];
    }

  private:
    int n_;
    int logn_;
    vector<int> depth_;
    vector<EdgeCostTp> distance_;
    vector<vector<int>> ancestor_;
    vector<vector<pair<int, EdgeCostTp>>> edges_;
};

int main() {
    int N, M;
    cin >> N >> M;
    vector<vector<pair<int, int>>> G(N);
    vector<pair<int, int>> E;
    for(int i = 0; i < M; i++) {
        int a, b;
        cin >> a >> b;
        a--, b--;
        G[a].emplace_back(b, i);
        G[b].emplace_back(a, i);
        E.emplace_back(a, b);
    }
    vector<bool> in_T(M);

    // 幅優先探索で、グラフから 1 つ木を取り出す
    vector<bool> visit(N);
    queue<int> que;
    que.emplace(0);
    visit[0] = true;
    while(que.size()) {
        int v = que.front();
        que.pop();
        for(auto [u, i] : G[v]) {
            if(visit[u]) {
                continue;
            }
            visit[u] = true;
            in_T[i] = true;
            que.emplace(u);
        }
    }

    // グラフから取り出した木の最小共通祖先
    LowestCommonAncestor T(N);
    // 木に含まれない辺のリスト
    vector<pair<int, int>> U;
    for(int i = 0; i < M; i++) {
        auto [a, b] = E[i];
        if(in_T[i]) {
            T.add_edge(a, b);
        } else {
            U.emplace_back(a, b);
        }
    }
    T.build();

    // 木に含まれない辺の端点の集合
    set<int> S;
    for(auto [u, v] : U) {
        S.emplace(u);
        S.emplace(v);
    }

    // S の全点対の最小距離をワーシャルフロイド法で求める
    map<int, map<int, int>> dist;
    for(int i : S) {
        for(int j : S) {
            dist[i][j] = T.distance(i, j);
        }
    }
    for(auto [u, v] : U) {
        dist[u][v] = dist[v][u] = 1;
    }
    for(int k : S) {
        for(int i : S) {
            for(int j : S) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    // クエリに応答する
    int Q;
    cin >> Q;
    for(int iter = 0; iter < Q; iter++) {
        int u, v;
        cin >> u >> v;
        u--, v--;
        int ans = T.distance(u, v);
        map<int, int> head, tail;
        for(int i : S) {
            head[i] = T.distance(u, i);
            tail[i] = T.distance(i, v);
        }
        for(int i : S) {
            for(int j : S) {
                ans = min(ans, head[i] + dist[i][j] + tail[j]);
            }
        }
        cout << ans << endl;
    }
    return 0;
}

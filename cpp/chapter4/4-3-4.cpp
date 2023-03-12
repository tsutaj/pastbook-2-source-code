#include <cmath>
#include <cstdio>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

class LowestCommonAncestor {
  public:
    LowestCommonAncestor(int n) {
        n_ = n;
        logn_ = int(log2(n_) + 2.0);
        depth_.resize(n_, 0);
        ancestor_.resize(logn_, vector<int>(n_, -1));
        parent_edge_id_.resize(n_, -1);
        edges_.resize(n_);
    }

    // (u, v) 間 (0-indexed) に、edge_id 番目の辺を追加
    void add_edge(int u, int v, int edge_id) {
        edges_[u].emplace_back(v, edge_id);
        edges_[v].emplace_back(u, edge_id);
    }

    // 根を root にした木に対して計算
    void build(int root = 0) {
        stack<int> st;
        st.emplace(root);
        while(st.size()) {
            int cur = st.top();
            st.pop();
            for(auto [nxt, edge_id] : edges_[cur]) {
                if(ancestor_[0][nxt] != cur && ancestor_[0][cur] != nxt) {
                    ancestor_[0][nxt] = cur;
                    parent_edge_id_[nxt] = edge_id;
                    depth_[nxt] = depth_[cur] + 1;
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

    // 頂点 v の親と接続する辺の番号を返す
    int get_parent_edge_id(int v) { return parent_edge_id_[v]; }

    // 頂点 v の親を返す
    int get_parent(int v) { return ancestor_[0][v]; }

    // 頂点 v の深さを返す
    int get_depth(int v) { return depth_[v]; }

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

    int distance(int u, int v) {
        return depth_[u] + depth_[v] - 2 * depth_[lca(u, v)];
    }

  private:
    int n_;
    int logn_;
    vector<int> depth_;
    vector<vector<int>> ancestor_;
    vector<vector<pair<int, int>>> edges_;
    // 親と接続している辺の番号
    vector<int> parent_edge_id_;
};

class UnionFind {
  public:
    // N 頂点で初期化
    UnionFind(int N) {
        size_.resize(N, 1);
        parent_.resize(N, -1);
    }

    // 代表値を求める (経路圧縮も行う)
    int find(int v) {
        if(parent_[v] == -1) {
            return v;
        } else {
            return parent_[v] = find(parent_[v]);
        }
    }

    // 2 つの集合を併合する
    // 実際に併合が起こったかどうかを返しておくと便利
    bool unite(int x, int y) {
        // 代表値 (木の根) を求める
        x = find(x);
        y = find(y);

        // すでに同じ集合に属するときは何もしない
        if(x == y) {
            return false;
        }

        // y 側の集合を x 側に併合 (union-by-size は行わない)
        parent_[y] = x;
        size_[x] += size_[y];
        return true;
    }

    // 2 つの要素が同じ集合に属するか求める (あると便利)
    bool same(int x, int y) {
        x = find(x);
        y = find(y);
        return x == y;
    }

    // 集合のサイズを求める (あると便利)
    int size(int x) { return size_[find(x)]; }

  private:
    // 木の根 r に対して、size[r] は木の頂点数を表す
    // 根でない頂点に対しては不定 (参照しない)
    vector<int> size_;
    // 頂点 v に対して、parent[v] は v の親を表す (v が根なら -1)
    vector<int> parent_;
};

int main() {
    int N, Q;
    cin >> N >> Q;
    LowestCommonAncestor tree(N);
    UnionFind uf(N);

    for(int i = 0; i < N - 1; i++) {
        int u, v;
        cin >> u >> v;
        tree.add_edge(u - 1, v - 1, i);
    }
    tree.build();

    vector<int> U(Q), V(Q), C(Q);
    for(int i = 0; i < Q; i++) {
        cin >> U[i] >> V[i] >> C[i];
        U[i]--, V[i]--;
    }

    vector<int> ans(N - 1);
    for(int i = Q - 1; i >= 0; i--) {
        int x = tree.lca(U[i], V[i]);
        int u = uf.find(U[i]), v = uf.find(V[i]);
        int depth_x = tree.get_depth(x);
        int depth_u = tree.get_depth(u);
        int depth_v = tree.get_depth(v);

        while(depth_u > depth_x) {
            int edge_id = tree.get_parent_edge_id(u);
            ans[edge_id] = C[i];
            int p = tree.get_parent(u);
            uf.unite(p, u);
            u = uf.find(u);
            depth_u = tree.get_depth(u);
        }

        while(depth_v > depth_x) {
            int edge_id = tree.get_parent_edge_id(v);
            ans[edge_id] = C[i];
            int p = tree.get_parent(v);
            uf.unite(p, v);
            v = uf.find(v);
            depth_v = tree.get_depth(v);
        }
    }

    for(int i = 0; i < N - 1; i++) {
        cout << ans[i] << endl;
    }
    return 0;
}

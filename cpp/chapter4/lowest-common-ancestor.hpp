#include <cmath>
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

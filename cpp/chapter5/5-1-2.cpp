#include <algorithm>
#include <cstdio>
#include <iostream>
#include <limits>
#include <queue>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;

template <typename CapacityTp> class Flow {
  public:
    using Edge = tuple<int, CapacityTp, int>;

    Flow(int V) {
        V_ = V;
        inf_capacity_ = numeric_limits<CapacityTp>::max();
        G_.resize(V_);
    }

    // 頂点 u から頂点 v へ容量 c の辺を張る
    void add_edge(int u, int v, CapacityTp c) {
        // [終点, 容量, 逆辺のインデックス] を辺として保存する
        G_[u].emplace_back(v, c, G_[v].size());
        // 逆辺を張る
        G_[v].emplace_back(u, 0, G_[u].size() - 1);
    }

    // 頂点 s
    // からフローを流せる辺をたどったときの最短距離を各頂点について求め、距離のリストを返す
    // D[v] = -1 のとき、頂点 s から頂点 v
    // へフローを流せる辺をたどって到達できないことを表す
    vector<int> bfs(int s) {
        vector<int> D(V_, -1);
        D[s] = 0;
        queue<int> Q;
        Q.emplace(s);
        while(Q.size()) {
            int u = Q.front();
            Q.pop();
            for(auto [nxt, capacity, rev] : G_[u]) {
                // 辺に容量が残っていて、かつ、未訪問の頂点のとき
                if(capacity > 0 && D[nxt] < 0) {
                    D[nxt] = D[u] + 1;
                    Q.emplace(nxt);
                }
            }
        }
        return D;
    }

    // 頂点 v から頂点 t へ、f を上限としてフローを流す
    CapacityTp dfs(int v, int t, CapacityTp f, vector<int> &removed,
                   vector<int> &D) {
        if(v == t) {
            return f;
        }

        while(removed[v] < (int)G_[v].size()) {
            auto [nxt, capacity, rev] = G_[v][removed[v]];
            // 辺の容量が残っていて、かつ、最短経路上にあるとき、
            // この辺にフローを流すことを試みる
            if(capacity > 0 && D[v] < D[nxt]) {
                // capacity を超えるフローは流せないため、
                // 流すフローの上限値は min(f, capacity) になる
                CapacityTp flow = dfs(nxt, t, min(f, capacity), removed, D);
                // この辺にフローを流すことができるとき
                if(flow > 0) {
                    // 辺の容量を流したフロー分だけ減らす
                    get<1>(G_[v][removed[v]]) -= flow;
                    // 逆辺の容量を流したフロー分だけ増やす
                    get<1>(G_[nxt][rev]) += flow;
                    return flow;
                }
            }
            // フローが流せなかった辺は削除する
            // 実際に削除する操作は重いので、削除したことにした辺の数を1増やすことで、
            // 削除したことにする
            removed[v] += 1;
        }
        return 0;
    }

    // 頂点 s から頂点 t へ流せるフローの最大値を計算する
    CapacityTp calc_max_flow(int s, int t) {
        CapacityTp flow = 0;
        while(true) {
            // 幅優先探索で頂点 s からフローが流せる辺をたどったときの
            // 各頂点への最短距離を求める
            vector<int> D = bfs(s);

            // 頂点 s から頂点 t へフローが流せる辺をたどった経路がないとき
            // 計算を終了する
            if(D[t] < 0) {
                return flow;
            }

            // 各頂点について、何番目まで辺を削除したかを表す配列をつくる
            vector<int> removed(V_, 0);

            // フローが流せなくなるまで深さ優先探索を繰り返してフローを流す
            while(true) {
                // 深さ優先探索で頂点 s から頂点 t への最短経路を見つけ、
                // フローを流せるだけ流す
                CapacityTp f = dfs(s, t, inf_capacity_, removed, D);

                // これ以上フローが流れなければ操作を終了する
                if(f == 0) {
                    break;
                }
                flow += f;
            }
        }
        return flow;
    }

  private:
    vector<vector<Edge>> G_;
    int V_;
    CapacityTp inf_capacity_;
};

int main() {
    int H, W, N;
    cin >> H >> W >> N;
    int V = H + W + N * 2 + 2;

    Flow<int> flow(V);
    // 始点
    int X = H + W + N * 2;
    // 終点
    int Y = H + W + N * 2 + 1;

    for(int i = 0; i < N; i++) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        a--, b--;
        // 行に対応する頂点から、駒の入口に対応する頂点へ、辺を張る
        for(int r = a; r < c; r++) {
            flow.add_edge(r, H + W + i, 1);
        }
        // 駒の出口に対応する頂点から、列に対応する頂点へ、辺を張る
        for(int c = b; c < d; c++) {
            flow.add_edge(H + W + N + i, H + c, 1);
        }
    }

    // 行に対応する各頂点へ始点から辺を張る
    for(int r = 0; r < H; r++) {
        flow.add_edge(X, r, 1);
    }
    // 列に対応する各頂点から終点へ辺を張る
    for(int c = 0; c < W; c++) {
        flow.add_edge(H + c, Y, 1);
    }
    // 各駒について、入口から出口へ辺を張る
    for(int i = 0; i < N; i++) {
        flow.add_edge(H + W + i, H + W + N + i, 1);
    }
    cout << flow.calc_max_flow(X, Y) << endl;
    return 0;
}

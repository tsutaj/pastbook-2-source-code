#include <algorithm>
#include <iostream>
#include <limits>
#include <tuple>
#include <vector>
using namespace std;

template <typename CapacityTp, typename CostTp> class MinCostFlow {
  public:
    using Edge = tuple<int, CapacityTp, CostTp, int>;

    MinCostFlow(int V) {
        V_ = V;
        inf_cost_ = numeric_limits<CostTp>::max();
        G_.resize(V_);
    }

    void add_edge(int u, int v, CapacityTp capacity, CostTp cost) {
        G_[u].emplace_back(v, capacity, cost, G_[v].size());
        G_[v].emplace_back(u, 0, -cost, G_[u].size() - 1);
    }

    tuple<vector<CostTp>, vector<int>, vector<int>> bellman_ford(int s) {
        vector<CostTp> dist(V_, inf_cost_);
        dist[s] = 0;
        vector<int> pv(V_, 0);
        vector<int> pe(V_, 0);
        while(true) {
            bool update = false;
            for(int v = 0; v < V_; v++) {
                if(dist[v] == inf_cost_) {
                    continue;
                }
                for(int i = 0; i < (int)G_[v].size(); i++) {
                    auto [nxt, capacity, cost, rev] = G_[v][i];
                    if(capacity > 0 && dist[nxt] > dist[v] + cost) {
                        dist[nxt] = dist[v] + cost;
                        update = true;
                        pv[nxt] = v;
                        pe[nxt] = i;
                    }
                }
            }
            if(!update) {
                break;
            }
        }
        return make_tuple(dist, pv, pe);
    }

    CostTp calc_min_cost_flow(int s, int t, CapacityTp f) {
        CostTp result = 0;
        while(f > 0) {
            auto [dist, pv, pe] = bellman_ford(s);
            if(dist[t] == inf_cost_) {
                return inf_cost_;
            }
            CapacityTp flow = f;
            int v = t;
            while(v != s) {
                flow = min(flow, get<1>(G_[pv[v]][pe[v]]));
                v = pv[v];
            }
            result += flow * dist[t];
            f -= flow;
            v = t;
            while(v != s) {
                // 辺の容量を更新する
                get<1>(G_[pv[v]][pe[v]]) -= flow;
                // 逆辺についても同様にする
                int rev = get<3>(G_[pv[v]][pe[v]]);
                get<1>(G_[v][rev]) += flow;
                v = pv[v];
            }
        }
        return result;
    }

  private:
    int V_;
    vector<vector<Edge>> G_;
    CostTp inf_cost_;
};

int main() {
    int N, M;
    cin >> N >> M;
    vector<int> A(N), B(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i];
    }
    for(int i = 0; i < N; i++) {
        cin >> B[i];
    }
    vector<int> R(3);
    for(int i = 0; i < 3; i++) {
        cin >> R[i];
    }

    int V = 3 * (N + 1) + N + 2;
    MinCostFlow<long long, long long> mincost_flow(V);

    // 各ラウンドのグラフの始点
    vector<int> S = {N * 4, N * 4 + 1, N * 4 + 2};
    // グラフ全体の始点
    int X = N * 4 + 3;
    // グラフ全体の終点
    int Y = N * 4 + 4;

    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < N; j++) {
            // ラウンド i で棒 j に輪がかかったときに得られる得点
            long long P_j = A[j];
            for(int k = 0; k <= i; k++) {
                P_j *= B[j];
                P_j %= R[i];
            }

            // 各ラウンドの始点から、そのラウンドの棒jに対応する頂点へ辺を張る
            mincost_flow.add_edge(S[i], N * i + j, 1, -P_j);

            // ラウンドの棒jに対応する頂点から、
            // 棒jのペナルティを計算する頂点へ辺を張る
            mincost_flow.add_edge(N * i + j, N * 3 + j, 1, 0);
        }
    }

    // グラフ全体の始点から、各ラウンドの始点へ辺を張る
    for(int i = 0; i < 3; i++) {
        mincost_flow.add_edge(X, S[i], M, 0);
    }

    // 棒jのペナルティを計算する頂点から終点へ辺を張る
    for(int j = 0; j < N; j++) {
        // 棒jに1個目の輪がかかったときに新たに発生するペナルティ
        long long Q_1 = 1LL * A[j] * B[j];

        // 棒jに2個目の輪がかかったときに新たに発生するペナルティ
        long long Q_2 = 1LL * A[j] * B[j] * B[j] - Q_1;

        // 棒jに3個目の輪がかかったときに新たに発生するペナルティ
        long long Q_3 = 1LL * A[j] * B[j] * B[j] * B[j] - Q_1 - Q_2;

        mincost_flow.add_edge(N * 3 + j, Y, 1, Q_1);
        mincost_flow.add_edge(N * 3 + j, Y, 1, Q_2);
        mincost_flow.add_edge(N * 3 + j, Y, 1, Q_3);
    }

    // コストを -1 倍したものが得点となる
    cout << -mincost_flow.calc_min_cost_flow(X, Y, M * 3) << endl;
    return 0;
}

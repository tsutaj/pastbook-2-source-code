#include <algorithm>
#include <iostream>
#include <limits>
#include <numeric>
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
    int P, Q;
    cin >> P >> Q;
    vector<string> S(P);
    for(int i = 0; i < P; i++) {
        cin >> S[i];
    }

    vector<int> A(P), B(P);
    for(int i = 0; i < P; i++) {
        cin >> A[i] >> B[i];
    }
    vector<int> C(Q), D(Q);
    for(int i = 0; i < Q; i++) {
        cin >> C[i] >> D[i];
    }

    // グラフ全体の頂点数
    int V = P + Q + 2;
    // 始点
    int X = P + Q;
    // 終点
    int Y = P + Q + 1;

    MinCostFlow<long long, long long> mincost_flow(V);
    for(int i = 0; i < P; i++) {
        // 始点からオス i の頂点へ辺を張る
        mincost_flow.add_edge(X, i, 1, 0);
        // オス i の頂点から終点へ辺を張る
        mincost_flow.add_edge(i, Y, 1, -B[i]);
    }
    for(int i = 0; i < P; i++) {
        for(int j = 0; j < Q; j++) {
            // オス i とメス j がつがいを作れるとき、
            // 頂点 i と頂点 P+j の間に辺を張る
            if(S[i][j] == '1') {
                mincost_flow.add_edge(i, P + j, 1, -A[i]);
            }
        }
    }
    for(int j = 0; j < Q; j++) {
        // メス j の頂点から終点へ辺を張る
        mincost_flow.add_edge(P + j, Y, 1, -C[j] + D[j]);
    }
    long long ans = accumulate(D.begin(), D.end(), 0LL) -
                    mincost_flow.calc_min_cost_flow(X, Y, P);
    cout << ans << endl;
    return 0;
}

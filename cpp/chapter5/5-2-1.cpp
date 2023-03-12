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
    int V, E, F;
    cin >> V >> E >> F;
    MinCostFlow<long long, long long> mincost_flow(V);
    for(int i = 0; i < E; i++) {
        int u, v, capacity, cost;
        cin >> u >> v >> capacity >> cost;
        u--, v--;
        mincost_flow.add_edge(u, v, capacity, cost);
    }
    cout << mincost_flow.calc_min_cost_flow(0, V - 1, F) << endl;
    return 0;
}

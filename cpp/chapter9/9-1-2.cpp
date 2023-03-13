#include <iostream>
#include <queue>
#include <vector>
using namespace std;

// ダイクストラ法で使う状態オブジェクトの定義
struct State {
    long long distance;
    int pos;

    State(long long d, int v) : distance(d), pos(v) {}
    // 距離が短いものが先に取り出されるようにする
    bool operator<(const State &state) const {
        return distance > state.distance;
    }
};

// ダイクストラ法で、頂点 s を始点として、
// 各頂点への距離を求める関数
vector<long long> dijkstra(const vector<vector<pair<int, int>>> &G, int s) {
    const int N = G.size();

    vector<long long> D(N, 1e15);
    D[s] = 0;
    priority_queue<State> Q;
    Q.emplace(0, s);
    vector<bool> visit(N);
    while(Q.size()) {
        auto state = Q.top();
        Q.pop();
        int v = state.pos;
        if(visit[v]) {
            continue;
        }
        visit[v] = true;
        for(auto [u, t] : G[v]) {
            if(D[u] > D[v] + t) {
                D[u] = D[v] + t;
                Q.emplace(D[u], u);
            }
        }
    }
    return D;
}

int main() {
    int N, M;
    cin >> N >> M;
    vector<int> A(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i];
    }

    vector<vector<pair<int, int>>> G(N);
    for(int i = 0; i < M; i++) {
        int u, v, t;
        cin >> u >> v >> t;
        u--, v--;
        G[u].emplace_back(v, t);
        G[v].emplace_back(u, t);
    }

    // 始点から各頂点への距離を求める
    auto R = dijkstra(G, 0);
    // 終点から各頂点への距離を求める
    auto S = dijkstra(G, N - 1);
    // 各頂点への遷移可能な頂点の数
    // I[v] := 頂点 v に遷移可能な頂点の数
    vector<int> I(N);

    // T[v] := 頂点 v から遷移可能な頂点のリスト
    vector<vector<int>> T(N);
    for(int v = 0; v < N; v++) {
        for(auto [u, t] : G[v]) {
            if(R[v] + t + S[u] == R[N - 1]) {
                // v から u へ向かう辺が 0 から N-1 への最短経路に
                // 含まれているとき
                I[u]++;
                T[v].emplace_back(u);
            }
        }
    }

    vector<long long> dp(N);
    dp[0] = A[0];
    queue<int> Q;
    Q.emplace(0);
    // 各頂点について、調べ終わったその頂点への遷移の数
    vector<int> done(N);
    while(Q.size()) {
        int v = Q.front();
        Q.pop();
        for(int u : T[v]) {
            dp[u] = max(dp[u], dp[v] + A[u]);
            done[u]++;

            // 頂点 u への遷移をすべて調べ終わっていたら、
            // 頂点 u を Q に入れる
            if(I[u] == done[u]) {
                Q.emplace(u);
            }
        }
    }
    cout << dp[N - 1] << endl;
    return 0;
}

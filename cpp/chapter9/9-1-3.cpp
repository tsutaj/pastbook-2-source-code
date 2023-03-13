#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
using namespace std;

using EdgeType = pair<int, double>;
using StateType = pair<double, int>;

// プリム法でグラフ G の最小全域木を求め、
// その重みの合計を返す関数
double prim(const vector<vector<EdgeType>> &G) {
    int N = G.size();
    vector<bool> marked(N);
    int marked_count = 0;
    marked[0] = true;
    marked_count++;

    priority_queue<StateType, vector<StateType>, greater<>> Q;
    for(auto [j, c] : G[0]) {
        Q.emplace(c, j);
    }
    double sum = 0;
    while(marked_count < N) {
        auto [c, i] = Q.top();
        Q.pop();
        if(marked[i]) {
            continue;
        }
        marked[i] = true;
        marked_count++;
        sum += c;
        for(auto [j, c] : G[i]) {
            if(marked[j]) {
                continue;
            }
            Q.emplace(c, j);
        }
    }
    return sum;
}

int main() {
    int N, M;
    cin >> N >> M;
    vector<int> PX(N), PY(N);
    for(int i = 0; i < N; i++) {
        cin >> PX[i] >> PY[i];
    }
    vector<int> CX(M), CY(M), R(M);
    for(int i = 0; i < M; i++) {
        cin >> CX[i] >> CY[i] >> R[i];
    }

    vector<double> ans;
    // すべての環状交差点の部分集合について、最小全域木を求める
    for(int subset = 0; subset < (1 << M); subset++) {
        vector<int> S;
        for(int i = 0; i < M; i++) {
            if((subset >> i & 1) == 1) {
                S.emplace_back(i);
            }
        }
        // すべてのタワーと部分集合に含まれる環状交差点のグラフを作る
        vector<vector<EdgeType>> G(N + S.size());
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                double d = sqrt(pow(PX[i] - PX[j], 2) + pow(PY[i] - PY[j], 2));
                G[i].emplace_back(j, d);
            }
        }
        // タワー i と環状交差点 S[j] を結ぶ道路の長さ e を計算する
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < (int)S.size(); j++) {
                double e = abs(
                    sqrt(pow(PX[i] - CX[S[j]], 2) + pow(PY[i] - CY[S[j]], 2)) -
                    R[S[j]]);
                G[i].emplace_back(N + j, e);
                G[N + j].emplace_back(i, e);
            }
        }
        // 環状交差点 S[i] と S[j] を結ぶ道路の長さ f を計算する
        for(int i = 0; i < (int)S.size(); i++) {
            for(int j = 0; j < (int)S.size(); j++) {
                if(R[S[i]] < R[S[j]]) {
                    continue;
                }
                double c = sqrt(pow(CX[S[i]] - CX[S[j]], 2) +
                                pow(CY[S[i]] - CY[S[j]], 2));
                double f;
                if(R[S[i]] + R[S[j]] <= c) {
                    f = c - R[S[i]] - R[S[j]];
                } else if(R[S[i]] - R[S[j]] < c && c < R[S[i]] + R[S[j]]) {
                    f = 0;
                } else {
                    f = R[S[i]] - R[S[j]] - c;
                }
                G[N + i].emplace_back(N + j, f);
                G[N + j].emplace_back(N + i, f);
            }
        }
        // グラフ G の最小全域木を求める
        ans.emplace_back(prim(G));
    }
    printf("%.12f\n", *min_element(ans.begin(), ans.end()));
    return 0;
}

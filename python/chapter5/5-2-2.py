INF = 1e100
N, M = map(int, input().split())
A = list(map(int, input().split()))
B = list(map(int, input().split()))
R = list(map(int, input().split()))
# グラフの頂点数
V = 3 * (N + 1) + N + 2
G = [[] for _ in range(V)]


def add_edge(u, v, capacity, cost):
    G[u].append((v, capacity, cost, len(G[v])))
    G[v].append((u, 0, -cost, len(G[u]) - 1))


def bellman_ford(s):
    dist = [INF] * V
    dist[s] = 0
    pv = [0] * V
    pe = [0] * V
    while True:
        update = False
        for v in range(V):
            if dist[v] == INF:
                continue
            for i in range(len(G[v])):
                next, capacity, cost, _ = G[v][i]
                if capacity > 0 and dist[next] > dist[v] + cost:
                    dist[next] = dist[v] + cost
                    update = True
                    pv[next] = v
                    pe[next] = i
        if not update:
            break
    return dist, pv, pe


def calc_min_cost_flow(s, t, f):
    result = 0
    while f > 0:
        dist, pv, pe = bellman_ford(s)
        if dist[t] == INF:
            return INF
        flow = f
        v = t
        while v != s:
            flow = min(flow, G[pv[v]][pe[v]][1])
            v = pv[v]
        result += flow * dist[t]
        f -= flow
        v = t
        while v != s:
            # 辺の情報を持つタプルを分解する
            dest, capacity, cost, r = G[pv[v]][pe[v]]
            # 辺の容量を更新する
            capacity -= flow
            # 書き換え終わったので、タプルに戻す
            G[pv[v]][pe[v]] = (dest, capacity, cost, r)
            # 逆辺についても同様にする
            rev = G[pv[v]][pe[v]][3]
            dest, capacity, cost, r = G[v][rev]
            capacity += flow
            G[v][rev] = (dest, capacity, cost, r)
            v = pv[v]
    return result


S = [N * 4, N * 4 + 1, N * 4 + 2]
# グラフ全体の始点
X = N * 4 + 3
# グラフ全体の終点
Y = N * 4 + 4
for i in range(3):
    for j in range(N):
        # ラウンド i で棒 j に輪がかかったときに得られる得点
        P_j = (A[j] * (B[j] ** (i + 1))) % R[i]
        # 各ラウンドの始点から、そのラウンドの棒 j に対応する頂点へ辺を張る
        add_edge(S[i], N * i + j, 1, -P_j)
        # ラウンドの棒 j に対応する頂点から、
        # 棒 j のペナルティを計算する頂点へ辺を張る
        add_edge(N * i + j, N * 3 + j, 1, 0)
for i in range(3):
    add_edge(X, S[i], M, 0)
# 棒 j のペナルティを計算する頂点から終点へ辺を張る
for j in range(N):
    # 棒 j に 1 個目の輪がかかったときに新たに発生するペナルティ
    Q_1 = A[j] * B[j]
    # 棒 j に 2 個目の輪がかかったときに新たに発生するペナルティ
    Q_2 = A[j] * (B[j] ** 2) - Q_1
    # 棒 j に 3 個目の輪がかかったときに新たに発生するペナルティ
    Q_3 = A[j] * (B[j] ** 3) - Q_1 - Q_2
    add_edge(N * 3 + j, Y, 1, Q_1)
    add_edge(N * 3 + j, Y, 1, Q_2)
    add_edge(N * 3 + j, Y, 1, Q_3)
ans = calc_min_cost_flow(X, Y, M * 3)
# コストを -1 倍したものが得点となる
print(-ans)

INF = 1e100
P, Q = map(int, input().split())
S = [""] * P
for i in range(P):
    S[i] = input()
A = [0] * P
B = [0] * P
for i in range(P):
    A[i], B[i] = map(int, input().split())
C = [0] * Q
D = [0] * Q
for i in range(Q):
    C[i], D[i] = map(int, input().split())
# グラフ全体の頂点数
V = P + Q + 2
# 始点
X = P + Q
# 終点
Y = P + Q + 1
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


for i in range(P):
    # 始点からオス i の頂点へ辺を張る
    add_edge(X, i, 1, 0)
    # オス i の頂点から終点へ辺を張る
    add_edge(i, Y, 1, -B[i])
for i in range(P):
    for j in range(Q):
        # オス i とメス j がつがいを作れるとき、
        # 頂点 i と頂点 P+j の間に辺を張る
        if S[i][j] == "1":
            add_edge(i, P + j, 1, -A[i])
for j in range(Q):
    # メス j の頂点から終点へ辺を張る
    add_edge(P + j, Y, 1, -C[j] + D[j])
ans = sum(D) - calc_min_cost_flow(X, Y, P)
print(ans)

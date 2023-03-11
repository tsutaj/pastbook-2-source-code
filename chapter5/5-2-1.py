INF = 1e100
V, E, F = map(int, input().split())
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


for _ in range(E):
    u, v, capacity, cost = map(int, input().split())
    u -= 1
    v -= 1
    add_edge(u, v, capacity, cost)
ans = calc_min_cost_flow(0, V - 1, F)
print(ans)

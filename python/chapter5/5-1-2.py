from collections import deque

H, W, N = map(int, input().split())
V = H + W + N * 2 + 2
G = [[] for _ in range(V)]
# 始点
X = H + W + N * 2
# 終点
Y = H + W + N * 2 + 1


def add_edge(u, v, c):
    G[u].append([v, c, len(G[v])])
    G[v].append([u, 0, len(G[u]) - 1])


def bfs(s):
    D = [-1] * V
    D[s] = 0
    Q = deque()
    Q.append(s)
    while len(Q) > 0:
        u = Q.popleft()
        for next, capacity, _ in G[u]:
            if capacity > 0 and D[next] < 0:
                D[next] = D[u] + 1
                Q.append(next)
    return D


def dfs(v, t, f, removed, D):
    if v == t:
        return f
    while removed[v] < len(G[v]):
        next, capacity, rev = G[v][removed[v]]
        if capacity > 0 and D[v] < D[next]:
            flow = dfs(next, t, min(f, capacity), removed, D)
            if flow > 0:
                G[v][removed[v]][1] -= flow
                G[next][rev][1] += flow
                return flow
        removed[v] += 1
    return 0


def calc_max_flow(s, t):
    flow = 0
    while True:
        D = bfs(s)
        if D[t] < 0:
            return flow
        removed = [0] * V
        while True:
            f = dfs(s, t, 1e10, removed, D)
            if f == 0:
                break
            flow += f


for i in range(N):
    A, B, C, D = map(int, input().split())
    A -= 1
    B -= 1
    C -= 1
    D -= 1
    # 行に対応する頂点から、駒の入口に対応する頂点へ、辺を張る
    for r in range(A, C + 1):
        add_edge(r, H + W + i, 1)
    # 駒の出口に対応する頂点から、列に対応する頂点へ、辺を張る
    for c in range(B, D + 1):
        add_edge(H + W + N + i, H + c, 1)
# 行に対応する各頂点へ始点から辺を張る
for r in range(0, H):
    add_edge(X, r, 1)
# 列に対応する各頂点から終点へ辺を張る
for c in range(0, W):
    add_edge(H + c, Y, 1)
# 各駒について、入口から出口へ辺を張る
for i in range(0, N):
    add_edge(H + W + i, H + W + N + i, 1)
max_flow = calc_max_flow(X, Y)
print(max_flow)

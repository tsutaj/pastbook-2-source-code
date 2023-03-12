from collections import deque

V, E = map(int, input().split())
G = [[] for _ in range(V)]


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


for _ in range(E):
    u, v, c = map(int, input().split())
    u -= 1
    v -= 1
    add_edge(u, v, c)
max_flow = calc_max_flow(0, V - 1)
print(max_flow)

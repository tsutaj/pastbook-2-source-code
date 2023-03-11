import heapq
from collections import deque

N, M = map(int, input().split())
A = list(map(int, input().split()))
G = [[] for _ in range(N)]
for _ in range(M):
    u, v, t = map(int, input().split())
    u -= 1
    v -= 1
    G[u].append((v, t))
    G[v].append((u, t))
# ダイクストラ法で、頂点 s を始点として、
# 各頂点への距離を求める関数
def dijkstra(s):
    D = [1e15] * N
    D[s] = 0
    Q = []
    heapq.heappush(Q, (0, s))
    visit = [False] * N
    while len(Q) > 0:
        _, v = heapq.heappop(Q)
        if visit[v]:
            continue
        visit[v] = True
        for (u, t) in G[v]:
            if D[u] > D[v] + t:
                D[u] = D[v] + t
                heapq.heappush(Q, (D[u], u))
    return D


# 始点から各頂点への距離を求める
R = dijkstra(0)
# 終点から各頂点への距離を求める
S = dijkstra(N - 1)
# 各頂点への遷移可能な頂点の数
# I[v] := 頂点 v に遷移可能な頂点の数
I = [0] * N

# T[v] := 頂点 v から遷移可能な頂点のリスト
T = [[] for _ in range(N)]
for v in range(N):
    for (u, t) in G[v]:
        if R[v] + t + S[u] == R[N - 1]:
            # v から u へ向かう辺が 0 から N-1 への最短経路に
            # 含まれているとき
            I[u] += 1
            T[v].append(u)
dp = [0] * N
dp[0] = A[0]
Q = deque()
Q.append(0)
# 各頂点について、調べ終わったその頂点への遷移の数
done = [0] * N
while len(Q) > 0:
    v = Q.pop()
    for u in T[v]:
        dp[u] = max(dp[u], dp[v] + A[u])
        done[u] += 1
        # 頂点 u への遷移をすべて調べ終わっていたら、
        # 頂点 u を Q に入れる
        if I[u] == done[u]:
            Q.append(u)
print(dp[N - 1])

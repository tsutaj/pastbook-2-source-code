from collections import deque
import math


class LowestCommonAncestor:
    def __init__(self, n):
        self._n = n
        self._logn = int(math.log2(self._n) + 2)
        self._depth = [0 for _ in range(self._n)]
        self._distance = [0 for _ in range(self._n)]
        self._ancestor = [[-1 for _ in range(self._n)] for k in range(self._logn)]
        self._edges = [[] for _ in range(self._n)]

    # (u, v) 間 (0-indexed) に重み w の辺を追加
    def add_edge(self, u, v, w=1):
        self._edges[u].append((v, w))
        self._edges[v].append((u, w))

    # 根を root にした木に対して計算
    def build(self, root=0):
        stack = [root]
        while len(stack):
            cur = stack.pop()
            for nxt, w in self._edges[cur]:
                if self._ancestor[0][nxt] != cur and self._ancestor[0][cur] != nxt:
                    self._ancestor[0][nxt] = cur
                    self._depth[nxt] = self._depth[cur] + 1
                    self._distance[nxt] = self._distance[cur] + w
                    stack.append(nxt)
        for k in range(1, self._logn):
            for i in range(self._n):
                if self._ancestor[k - 1][i] == -1:
                    self._ancestor[k][i] = -1
                else:
                    self._ancestor[k][i] = self._ancestor[k - 1][
                        self._ancestor[k - 1][i]
                    ]

    # u と v (0-indexed) の LCA を求める
    def lca(self, u, v):
        # u よりも v のほうが深い状態にしておく
        if self._depth[u] > self._depth[v]:
            u, v = v, u
        # v の深さを u に合わせる
        for k in range(self._logn - 1, -1, -1):
            if ((self._depth[v] - self._depth[u]) >> k) & 1:
                v = self._ancestor[k][v]
        # この時点で一致していれば、それが答え
        if u == v:
            return u
        # u, v がギリギリ一致しないように親方向に辿る
        for k in range(self._logn - 1, -1, -1):
            if self._ancestor[k][u] != self._ancestor[k][v]:
                u = self._ancestor[k][u]
                v = self._ancestor[k][v]
        # 最後に 1 ステップ親方向に辿ったところが LCA になる
        return self._ancestor[0][u]

    # u と v (0-indexed) の距離を求める
    def distance(self, u, v):
        return (
            self._distance[u] + self._distance[v] - 2 * self._distance[self.lca(u, v)]
        )


N, M = map(int, input().split())
G = [[] for _ in range(N)]
E = []
for i in range(M):
    a, b = map(int, input().split())
    a -= 1
    b -= 1
    G[a].append((b, i))
    G[b].append((a, i))
    E.append((a, b))
in_T = [False] * M
##################################################
# 幅優先探索で、グラフから 1 つ木を取り出す
##################################################
visit = [False] * N
Q = deque()
Q.append(0)
visit[0] = True
while len(Q) > 0:
    v = Q.popleft()
    for (u, i) in G[v]:
        if visit[u]:
            continue
        visit[u] = True
        in_T[i] = True
        Q.append(u)
# グラフから取り出した木の最小共通祖先
T = LowestCommonAncestor(N)
# 木に含まれない辺のリスト
U = []
for i in range(M):
    a, b = E[i]
    if in_T[i]:
        T.add_edge(a, b)
    else:
        U.append((a, b))
T.build()
# 木に含まれない辺の端点の集合
S = set()
for u, v in U:
    S.add(u)
    S.add(v)
##################################################
# S の全点対の最小距離をワーシャルフロイド法で求める
##################################################
dist = {}
for i in S:
    dist[i] = {}
    for j in S:
        dist[i][j] = T.distance(i, j)
for u, v in U:
    dist[u][v] = 1
    dist[v][u] = 1
for k in S:
    for i in S:
        for j in S:
            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
# クエリに応答する
Q = int(input())
for _ in range(Q):
    u, v = map(int, input().split())
    u -= 1
    v -= 1
    ans = T.distance(u, v)
    head = {}
    tail = {}
    for i in S:
        head[i] = T.distance(u, i)
        tail[i] = T.distance(i, v)
    for i in S:
        for j in S:
            ans = min(ans, head[i] + dist[i][j] + tail[j])
    print(ans)

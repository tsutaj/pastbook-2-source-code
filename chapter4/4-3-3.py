import math


class LowestCommonAncestor:
    def __init__(self, n):
        self._n = n
        self._logn = int(math.log2(self._n) + 2)
        self._depth = [0 for _ in range(self._n)]
        self._distance = [0 for _ in range(self._n)]
        self._ancestor = [
            [-1 for _ in range(self._n)]
            for k in range(self._logn)
        ]
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
                if (
                    self._ancestor[0][nxt] != cur and
                    self._ancestor[0][cur] != nxt
                ):
                    self._ancestor[0][nxt] = cur
                    self._depth[nxt] = self._depth[cur] + 1
                    self._distance[nxt] = self._distance[cur] + w
                    stack.append(nxt)

        for k in range(1, self._logn):
            for i in range(self._n):
                if self._ancestor[k - 1][i] == -1:
                    self._ancestor[k][i] = -1
                else:
                    self._ancestor[k][i] = \
                        self._ancestor[k - 1][self._ancestor[k - 1][i]]

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

        # u, v がギリギリ一致しないように親方向に登る
        for k in range(self._logn - 1, -1, -1):
            if self._ancestor[k][u] != self._ancestor[k][v]:
                u = self._ancestor[k][u]
                v = self._ancestor[k][v]

        # 最後に 1 ステップ親方向に登ったところが LCA になる
        return self._ancestor[0][u]

    # u と v (0-indexed) の距離を求める
    def distance(self, u, v):
        return (
            self._distance[u] +
            self._distance[v] -
            2 * self._distance[self.lca(u, v)]
        )


N = int(input())
tree = LowestCommonAncestor(N)
for _ in range(N - 1):
    u, v = map(int, input().split())
    tree.add_edge(u - 1, v - 1)
tree.build()

Q = int(input())
for _ in range(Q):
    a, b = map(int, input().split())
    print(tree.distance(a - 1, b - 1) + 1)

import math


class LowestCommonAncestor:
    def __init__(self, n):
        self._n = n
        self._logn = int(math.log2(self._n) + 2)
        self._depth = [0 for _ in range(self._n)]
        self._ancestor = [
            [-1 for _ in range(self._n)]
            for k in range(self._logn)
        ]
        self._parent_edge_id = [
            -1 for _ in range(self._n)
        ]  # 親と接続している辺の番号
        self._edges = [[] for _ in range(self._n)]

    # (u, v) 間 (0-indexed) に、edge_id 番目の辺を追加
    def add_edge(self, u, v, edge_id):
        self._edges[u].append((v, edge_id))
        self._edges[v].append((u, edge_id))

    # 根を root にした木に対して計算
    def build(self, root=0):
        stack = [root]
        while len(stack):
            cur = stack.pop()
            for nxt, edge_id in self._edges[cur]:
                if (
                    self._ancestor[0][nxt] != cur and
                    self._ancestor[0][cur] != nxt
                ):
                    self._ancestor[0][nxt] = cur
                    self._parent_edge_id[nxt] = edge_id
                    self._depth[nxt] = self._depth[cur] + 1
                    stack.append(nxt)

        for k in range(1, self._logn):
            for i in range(self._n):
                if self._ancestor[k - 1][i] == -1:
                    self._ancestor[k][i] = -1
                else:
                    self._ancestor[k][i] = \
                        self._ancestor[k - 1][self._ancestor[k - 1][i]]

    # 頂点 v の親と接続する辺の番号を返す
    def get_parent_edge_id(self, v):
        return self._parent_edge_id[v]

    # 頂点 v の親を返す
    def get_parent(self, v):
        return self._ancestor[0][v]

    # 頂点 v の深さを返す
    def get_depth(self, v):
        return self._depth[v]

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
            self._depth[u] +
            self._depth[v] -
            2 * self._depth[self.lca(u, v)]
        )


class UnionFind:
    # N 頂点で初期化
    def __init__(self, N):
        # 木の根 r に対して、self._size[r] は木の頂点数を表す
        # 根でない頂点に対しては不定 (参照しない)
        self._size = [1 for i in range(N)]
        # 頂点 v に対して、self._parent[v] は v の親を表す (v が根なら -1)
        self._parent = [-1 for i in range(N)]

    # 代表値を求める (経路圧縮も行う)
    def find(self, v):
        if self._parent[v] == -1:
            return v
        else:
            vertices = []
            while self._parent[v] >= 0:
                vertices.append(v)
                v = self._parent[v]
            for i in vertices:
                self._parent[i] = v
            return v

    # 2 つの集合を併合する
    # 実際に併合が起こったかどうかを返しておくと便利
    def unite(self, x, y):
        # 代表値 (木の根) を求める
        x = self.find(x)
        y = self.find(y)

        # すでに同じ集合に属するときは何もしない
        if x == y:
            return False

        # y 側の集合を x 側に併合 (union-by-size は行わない)
        self._parent[y] = x
        self._size[x] += self._size[y]
        return True

    # 2 つの要素が同じ集合に属するか求める (あると便利)
    def same(self, x, y):
        x = self.find(x)
        y = self.find(y)
        return x == y

    # 集合のサイズを求める (あると便利)
    def size(self, x):
        return self._size[self.find(x)]


N, Q = map(int, input().split())
tree = LowestCommonAncestor(N)
uf = UnionFind(N)

for i in range(N - 1):
    u, v = map(int, input().split())
    tree.add_edge(u - 1, v - 1, i)
tree.build()

U = [0 for _ in range(Q)]
V = [0 for _ in range(Q)]
C = [0 for _ in range(Q)]
for i in range(Q):
    U[i], V[i], C[i] = map(int, input().split())
    U[i] -= 1
    V[i] -= 1

ans = [0 for _ in range(N - 1)]
for i in range(Q - 1, -1, -1):
    x = tree.lca(U[i], V[i])
    u, v = uf.find(U[i]), uf.find(V[i])
    depth_x = tree.get_depth(x)
    depth_u = tree.get_depth(u)
    depth_v = tree.get_depth(v)

    while depth_u > depth_x:
        edge_id = tree.get_parent_edge_id(u)
        ans[edge_id] = C[i]
        p = tree.get_parent(u)
        uf.unite(p, u)
        u = uf.find(u)
        depth_u = tree.get_depth(u)

    while depth_v > depth_x:
        edge_id = tree.get_parent_edge_id(v)
        ans[edge_id] = C[i]
        p = tree.get_parent(v)
        uf.unite(p, v)
        v = uf.find(v)
        depth_v = tree.get_depth(v)

for i in range(N - 1):
    print(ans[i])

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

        # 木のサイズが小さいものを大きいものに併合
        if self._size[x] < self._size[y]:
            x, y = y, x
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


N, M = map(int, input().split())

# [造られた年, 辺の端点 a, 辺の端点 b]
edges = [[0 for _ in range(3)] for _ in range(M)]
for i in range(M):
    a, b, y = map(int, input().split())
    edges[i] = [y, a - 1, b - 1]
edges.sort(reverse=True)

Q = int(input())
# [造られた年の制約, 住んでいる頂点, 何番目の国民か]
people = [[0 for _ in range(3)] for _ in range(Q)]
for i in range(Q):
    v, w = map(int, input().split())
    people[i] = [w, v - 1, i]
people.sort(reverse=True)

uf = UnionFind(N)
i_edge = 0
ans = [0 for _ in range(Q)]
for i_person in range(Q):
    w, v, i = people[i_person]
    while i_edge < M and edges[i_edge][0] > w:
        a, b = edges[i_edge][1:]
        uf.unite(a, b)
        i_edge += 1
    ans[i] = uf.size(v)
for i in range(Q):
    print(ans[i])

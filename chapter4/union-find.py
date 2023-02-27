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

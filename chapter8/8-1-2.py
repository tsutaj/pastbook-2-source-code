import sys
# add: a, b を引数とし、a+b を返す関数
from operator import add
# 入力の受け取りの高速化
input = sys.stdin.readline


class SegmentTree:
    def __init__(
        self,
        n,            # 列の長さ
        identity_e,   # 単位元
        combine_f,    # 2 つのデータから値を合成するための関数
    ):
        self._n = n
        self._size = 1
        while self._size < self._n:
            self._size <<= 1
        self._identity_e = identity_e
        self._combine_f = combine_f
        self._node = [self._identity_e] * (2 * self._size)

    # 配列の各要素を登録する
    def build(self, array):
        assert len(array) == self._n
        for index, value in enumerate(array, start=self._size):
            self._node[index] = value
        for index in range(self._size - 1, 0, -1):
            self._node[index] = self._combine_f(
                self._node[index << 1 | 0],  # 左の子
                self._node[index << 1 | 1],  # 右の子
            )

    # [一点更新] 位置 index (0-indexed) を値 value で更新
    def update(self, index, value):
        i = self._size + index
        self._node[i] = value
        while i > 1:
            i >>= 1
            self._node[i] = self._combine_f(
                self._node[i << 1 | 0],  # 左の子
                self._node[i << 1 | 1],  # 右の子
            )

    # [区間取得] 区間 [l, r) (0-indexed) 内の要素について、
    # l 番目から順に combine_f を適用した結果を返す (交換法則が前提になくてもよい)
    def fold(self, L, R):
        L += self._size
        R += self._size
        value_L = self._identity_e
        value_R = self._identity_e
        while L < R:
            if L & 1:
                value_L = self._combine_f(value_L, self._node[L])
                L += 1
            if R & 1:
                R -= 1
                value_R = self._combine_f(self._node[R], value_R)
            L >>= 1
            R >>= 1
        return self._combine_f(value_L, value_R)


N, Q = map(int, input().split())

# C (色の情報) は、入力から 1 引いた状態で管理する
C = list(map(lambda x: x - 1, map(int, input().split())))

# position[c] := 玉の色が c であるインデックスのリスト
# cursors[c] := position[c] の何番目を参照するべきか
positions = [[] for _ in range(N)]
cursors = [0 for _ in range(N)]
for i in range(N):
    positions[C[i]].append(i)

# queries[L] := 与えられる区間の左端が L であるようなクエリのリスト
queries = [[] for _ in range(N)]
for i in range(Q):
    L, R = map(int, input().split())
    L -= 1
    queries[L].append((R, i))

# それぞれの色について、最左にあるものの場所を管理する
segtree = SegmentTree(N, 0, add)
for color in range(N):
    if len(positions[color]) > 0:
        segtree.update(positions[color][0], 1)

# 区間の左端が L であるクエリを処理する
ans = [0 for _ in range(Q)]
for L in range(N):
    for R, i in queries[L]:
        ans[i] = segtree.fold(L, R)

    # これより後には、区間の左端が L+1 以上のクエリしか見ない
    # L 番目の玉はもう見られないので、セグメント木を更新
    color = C[L]
    segtree.update(L, 0)
    cursors[color] += 1
    if cursors[color] < len(positions[color]):
        segtree.update(positions[color][cursors[color]], 1)

print(*ans, sep="\n")

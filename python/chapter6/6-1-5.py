from bisect import bisect_left


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


L, Q = map(int, input().split())
c = [0 for _ in range(Q)]
x = [0 for _ in range(Q)]
for i in range(Q):
    c[i], x[i] = map(int, input().split())

# クエリで登場する x 座標を、重複を除いた状態で持つ
positions = sorted(list(set(x)))
N = len(positions)

# ある座標に対して、その左/右方向に最も近い切れ目を探す
# 左方向の場合は最大値、右方向の場合は最小値が分かればよい
seg_L = SegmentTree(N, 0, max)
seg_R = SegmentTree(N, L, min)

for ci, xi in zip(c, x):
    # xi は positions の何番目の要素か (二分探索で検索)
    i = bisect_left(positions, xi)
    if ci == 1:
        seg_L.update(i, xi)
        seg_R.update(i, xi)
    else:
        L = seg_L.fold(0, i)
        R = seg_R.fold(i, N)
        print(R - L)

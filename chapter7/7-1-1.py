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


MOD = 10**9 + 7


# セグメント木に使用する関数
def mod_add(a, b):
    return (a + b) % MOD


N = int(input())
A = list(map(int, input().split()))

# 数列 A を座標圧縮し、大小関係を維持しつつ、各要素を高々 N-1 の整数で表す
A_unique = sorted(list(set(A)))
A = [bisect_left(A_unique, A[i]) for i in range(N)]

seg0 = SegmentTree(N, 0, mod_add)
seg1 = SegmentTree(N, 0, mod_add)
for i in range(N):
    v0 = seg0.fold(A[i], A[i] + 1)
    v1 = seg1.fold(A[i], A[i] + 1)

    # 要素を使う (直前の要素よりも真に大きくする)
    seg1.update(A[i], v1 + seg0.fold(0, A[i]) + 1)

    # 要素を使う (直前の要素よりも真に小さくする)
    seg0.update(A[i], v0 + seg1.fold(A[i] + 1, N) + 1)

ans = (seg0.fold(0, N) + seg1.fold(0, N) - 2 * N) % MOD
if ans < 0:
    ans += MOD
print(ans)

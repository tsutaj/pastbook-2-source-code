# 一点更新・区間最小値を処理できる
class SegmentTree:
    # 2^{31} - 1 を正の無限大とみなす
    IDENTITY_E = (1 << 31) - 1

    def __init__(
        self,
        n,  # 列の長さ
    ):
        self._n = n
        self._size = 1
        while self._size < self._n:
            self._size <<= 1
        self._node = [self.IDENTITY_E] * (2 * self._size)

    # 配列の各要素を登録する
    def build(self, array):
        assert len(array) == self._n
        for index, value in enumerate(array, start=self._size):
            self._node[index] = value
        for index in range(self._size - 1, 0, -1):
            self._node[index] = min(
                self._node[index << 1 | 0],  # 左の子
                self._node[index << 1 | 1],  # 右の子
            )

    # [一点更新] 位置 index (0-indexed) を値 value で更新
    def update(self, index, value):
        i = self._size + index
        self._node[i] = value
        while i > 1:
            i >>= 1
            self._node[i] = min(
                self._node[i << 1 | 0],  # 左の子
                self._node[i << 1 | 1],  # 右の子
            )

    # [区間取得] 区間 [l, r) (0-indexed) 内の要素の最小値
    def fold(self, L, R):
        L += self._size
        R += self._size
        value = self.IDENTITY_E
        while L < R:
            if L & 1:
                value = min(value, self._node[L])
                L += 1
            if R & 1:
                R -= 1
                value = min(value, self._node[R])
            L >>= 1
            R >>= 1
        return value


N, Q = map(int, input().split())
A = list(map(int, input().split()))
segment_tree = SegmentTree(N)
segment_tree.build(A)
for _ in range(Q):
    t, x, y = map(int, input().split())
    if t == 1:
        segment_tree.update(x, y)
    else:
        print(segment_tree.fold(x, y))

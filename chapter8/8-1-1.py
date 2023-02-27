from functools import cmp_to_key


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


# セグメント木のサイズ
SEGMENT_TREE_SIZE = 100010


# 2 つのプレゼントの情報 a, b を比較し、ソート時にどの順番になるべきか決定する
def compare_presents(a, b):
    if a[0] != b[0]:
        return a[0] - b[0]  # w が小さいほど前に来る
    elif a[1] != b[1]:
        return b[1] - a[1]  # h が大きいほど前に来る
    else:
        return 0


N = int(input())
presents = []
for i in range(N):
    w, h = map(int, input().split())
    presents.append((w, h))
presents.sort(key=cmp_to_key(compare_presents))

segtree = SegmentTree(SEGMENT_TREE_SIZE, 0, max)
for w, h in presents:
    current_value = segtree.fold(h, h + 1)
    segtree.update(h, max(current_value, segtree.fold(0, h) + 1))
print(segtree.fold(0, SEGMENT_TREE_SIZE))

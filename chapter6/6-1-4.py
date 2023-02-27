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


# N 枚のカードの列について、左から position 番目 (1-indexed) のカードの、
# 指定した方の面 (parity) を見たとき、何が書かれているかを返す

# face (parity == 0):   N, N-1, ...,    2,  1
# back (parity == 1): N+1, N+2, ..., 2N-1, 2N
# position          :   1,   2, ...,  N-1,  N
def get_value(N, position, parity):
    if parity == 0:
        return N + 1 - position
    else:
        return N + position


def add(a, b):
    return a + b


N, Q = map(int, input().split())
segment_tree = SegmentTree(N, 0, add)

for i in range(Q):
    t, k = map(int, input().split())
    if t == 1:
        # セグメント木上で何番目に相当する要素か求める
        position = N + 1 - k if k <= N else k - N
        # 偶奇 (どちら側の整数になるか求める)
        parity = (segment_tree.fold(position - 1, N) + (k > N)) % 2
        print(get_value(N, position, parity))
    else:
        # ひっくり返す (偶奇が変化)
        sum_value = segment_tree.fold(k - 1, k)
        segment_tree.update(k - 1, sum_value + 1)

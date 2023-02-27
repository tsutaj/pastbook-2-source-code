from itertools import accumulate


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


INF = 1 << 60

N, M = map(int, input().split())
A = list(map(int, input().split()))
S_posi = [0] + list(accumulate(A))  # 累積和
S_nega = [-v for v in S_posi]

queries = []
for i in range(M):
    l, r, c = map(int, input().split())
    queries.append((r, l - 1, c))
queries.sort()

# seg0 の i 番目では、dp の i 番目の要素を管理
seg0 = SegmentTree(N + 1, 0, max)
# seg1 の i 番目では、dp の i 番目の要素から S[i] を引いたものを管理
seg1 = SegmentTree(N + 1, -INF, max)
seg1.build(S_nega)

# 区間の右端が小さい順に処理
for r, l, c in queries:
    # 区間と被らない場所から遷移
    v0 = seg0.fold(0, l) + S_posi[r] - S_posi[l] - c
    # 区間と被る場所から遷移
    v1 = seg1.fold(l, r) + S_posi[r] - c
    dp_value = max(v0, v1)

    # 動的計画法の配列の更新
    seg0.update(r, max(seg0.fold(r, r + 1), dp_value))
    seg1.update(r, max(seg1.fold(r, r + 1), dp_value - S_posi[r]))
print(seg0.fold(0, N + 1))

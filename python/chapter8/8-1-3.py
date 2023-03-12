from bisect import bisect_left
from operator import add


class LazySegmentTree:
    def __init__(
        self,
        n,  # 列の長さ
        identity_e_node,  # 値データの単位元
        identity_e_lazy,  # 遅延データの単位元
        combine_node_f,  # 値データどうしを合成するために使用する関数
        combine_lazy_f,  # 遅延データを伝播させるために使用する関数
        reflect_f,  # 遅延データを値データに反映させるために使用する関数
    ):
        self._n = n
        self._size = 1
        self._height = 0
        while self._size < self._n:
            self._size <<= 1
            self._height += 1
        self._identity_e_node = identity_e_node
        self._identity_e_lazy = identity_e_lazy
        self._combine_node_f = combine_node_f
        self._combine_lazy_f = combine_lazy_f
        self._reflect_f = reflect_f
        self._node = [self._identity_e_node] * (2 * self._size)
        self._lazy = [self._identity_e_lazy] * (2 * self._size)

    # 遅延データの値を値データに反映させたときの結果を返す
    def _reflect_lazy(self, index):
        return self._reflect_f(self._node[index], self._lazy[index])

    # [遅延評価] index 番目 (0-indexed) の要素を含む区間について遅延データを伝播させる
    # 根に近いものから処理される
    def _propagate_from_top(self, index):
        index += self._size
        for h in range(self._height, 0, -1):
            i = index >> h
            if self._lazy[i] != self._identity_e_lazy:
                # 遅延データの情報を子に伝播させる
                self._lazy[i << 1] = self._combine_lazy_f(
                    self._lazy[i << 1], self._lazy[i]  # 左の子
                )
                self._lazy[i << 1 | 1] = self._combine_lazy_f(
                    self._lazy[i << 1 | 1], self._lazy[i]  # 右の子
                )

                # 遅延データの情報を値データに反映させ、遅延データの値をリセット
                self._node[i] = self._reflect_lazy(i)
                self._lazy[i] = self._identity_e_lazy

    # index 番目 (0-indexed) の要素を表す葉から順に値データを確定させる
    # (正確には葉に対しては行っておらず、葉の親から順に確定させている)
    def _update_from_bottom(self, index):
        index = (index + self._size) >> 1
        while index > 0:
            self._node[index] = self._combine_node_f(
                self._reflect_lazy(index << 1),
                self._reflect_lazy(index << 1 | 1)
            )
            index >>= 1

    # 配列の各要素を登録する
    def build(self, array):
        assert len(array) == self._n
        for index, value in enumerate(array, start=self._size):
            self._node[index] = value
        for index in range(self._size - 1, 0, -1):
            self._node[index] = self._combine_node_f(
                self._node[index << 1],  # 左の子
                self._node[index << 1 | 1],  # 右の子
            )

    # [区間更新] 位置 [L, R) (0-indexed) を値 value で更新
    def update(self, L, R, value):
        # トップダウンに遅延データの値を子に伝播させる
        self._propagate_from_top(L)
        self._propagate_from_top(R - 1)

        # 入力に対応する区間について遅延データを更新
        L_lazy = L + self._size
        R_lazy = R + self._size
        while L_lazy < R_lazy:
            if L_lazy & 1:
                self._lazy[L_lazy] = \
                    self._combine_lazy_f(self._lazy[L_lazy], value)
                L_lazy += 1
            if R_lazy & 1:
                R_lazy -= 1
                self._lazy[R_lazy] = \
                    self._combine_lazy_f(self._lazy[R_lazy], value)
            L_lazy >>= 1
            R_lazy >>= 1

        # 値データをボトムアップに更新
        self._update_from_bottom(L)
        self._update_from_bottom(R - 1)

    # [区間取得] 区間 [l, r) (0-indexed) 内の要素について、
    # l 番目から順に combine_node_f を適用した結果を返す (交換法則が前提になくても良い)
    def fold(self, L, R):
        # トップダウンに遅延データの値を子に伝播させる
        self._propagate_from_top(L)
        self._propagate_from_top(R - 1)

        # 入力に対応する区間について値を取得して合成
        L += self._size
        R += self._size
        value_L = self._identity_e_node
        value_R = self._identity_e_node
        while L < R:
            if L & 1:
                value_L = self._combine_node_f(value_L,
                                               self._reflect_lazy(L))
                L += 1
            if R & 1:
                R -= 1
                value_R = self._combine_node_f(self._reflect_lazy(R),
                                               value_R)
            L >>= 1
            R >>= 1
        return self._combine_node_f(value_L, value_R)


N, Q = map(int, input().split())

# クエリの種類
# y 座標が同じであれば ADD -> PLAN -> DEL の順に処理される
ADD = 0
PLAN = 1
DEL = 2

# (y 座標, クエリのタイプ, x 座標 (左), (x 座標 (右) or 計画の番号), (建設コスト)) を管理
queries = []
xs = []
for i in range(N):
    x, y, d, c = map(int, input().split())
    queries.append((y, ADD, x, x + d, c))
    queries.append((y + d, DEL, x, x + d, c))
    xs.append(x)
    xs.append(x + d)

for i in range(Q):
    a, b = map(int, input().split())
    queries.append((b, PLAN, a, i, -1))
    xs.append(a)
queries.sort()

# x 座標を重複のない形で昇順に持つ
xs = sorted(list(set(xs)))
M = len(xs)
ans = [0 for _ in range(Q)]

# 区間加算・区間和取得ができる遅延評価セグメント木を作る
segtree = LazySegmentTree(M, 0, 0, add, add, add)
segtree.build([0 for _ in range(M)])

# y 座標が小さい順にクエリを処理
for y, query_type, x1, x2, c in queries:
    if query_type == ADD or query_type == DEL:
        value = c if query_type == ADD else -c
        i_x1 = bisect_left(xs, x1)
        i_x2 = bisect_left(xs, x2) + 1
        segtree.update(i_x1, i_x2, value)
    elif query_type == PLAN:
        i_x = bisect_left(xs, x1)
        i_q = x2
        ans[i_q] = segtree.fold(i_x, i_x + 1)

print(*ans, sep="\n")

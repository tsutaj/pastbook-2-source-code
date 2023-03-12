#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>
using namespace std;

template <typename MonoidTp, typename OperatorTp> class LazySegmentTree {
  public:
    using MMtoM = function<MonoidTp(MonoidTp, MonoidTp)>;
    using MOtoM = function<MonoidTp(MonoidTp, OperatorTp)>;
    using OOtoO = function<OperatorTp(OperatorTp, OperatorTp)>;

    LazySegmentTree(
        int n,                      // 列の長さ
        MonoidTp identity_e_node,   // 値データの単位元
        OperatorTp identity_e_lazy, // 遅延データの単位元
        MMtoM combine_node_f, // 値データどうしを合成するために使用する関数
        OOtoO combine_lazy_f, // 遅延データを伝播させるために使用する関数
        MOtoM reflect_f // 遅延データを値データに反映させるために使用する関数
    ) {
        n_ = n;
        size_ = 1;
        height_ = 0;
        while(size_ < n_) {
            size_ <<= 1;
            height_++;
        }
        identity_e_node_ = identity_e_node;
        identity_e_lazy_ = identity_e_lazy;
        combine_node_f_ = combine_node_f;
        combine_lazy_f_ = combine_lazy_f;
        reflect_f_ = reflect_f;
        node_.resize(2 * size_, identity_e_node_);
        lazy_.resize(2 * size_, identity_e_lazy_);
    }

    // 配列の各要素を登録する
    void build(const vector<MonoidTp> &array) {
        assert((int)array.size() == n_);
        for(int index = 0; index < n_; index++) {
            node_[index + size_] = array[index];
        }
        for(int index = size_ - 1; index > 0; index--) {
            node_[index] =
                combine_node_f_(node_[index << 1], node_[index << 1 | 1]);
        }
    }

    // [区間更新] 位置 [L, R) (0-indexed) を値 value で更新
    void update(int L, int R, OperatorTp value) {
        // トップダウンに遅延データの値を子に伝播させる
        propagate_from_top(L);
        propagate_from_top(R - 1);

        // 入力に対応する区間について遅延データを更新
        int L_lazy = L + size_;
        int R_lazy = R + size_;
        while(L_lazy < R_lazy) {
            if(L_lazy & 1) {
                lazy_[L_lazy] = combine_lazy_f_(lazy_[L_lazy], value);
                L_lazy++;
            }
            if(R_lazy & 1) {
                R_lazy--;
                lazy_[R_lazy] = combine_lazy_f_(lazy_[R_lazy], value);
            }
            L_lazy >>= 1;
            R_lazy >>= 1;
        }

        // 値データをボトムアップに更新
        update_from_bottom(L);
        update_from_bottom(R - 1);
    }

    // [区間取得] 区間 [l, r) (0-indexed) 内の要素について、
    // l 番目から順に combine_node_f を適用した結果を返す
    // (交換法則が前提になくても良い)
    MonoidTp fold(int L, int R) {
        // トップダウンに遅延データの値を子に伝播させる
        propagate_from_top(L);
        propagate_from_top(R - 1);

        // 入力に対応する区間について値を取得して合成
        L += size_;
        R += size_;
        MonoidTp value_L = identity_e_node_;
        MonoidTp value_R = identity_e_node_;
        while(L < R) {
            if(L & 1) {
                value_L = combine_node_f_(value_L, reflect_lazy(L));
                L++;
            }
            if(R & 1) {
                R--;
                value_R = combine_node_f_(reflect_lazy(R), value_R);
            }
            L >>= 1;
            R >>= 1;
        }
        return combine_node_f_(value_L, value_R);
    }

  private:
    // 遅延データの値を値データに反映させたときの結果を返す
    MonoidTp reflect_lazy(int index) {
        return reflect_f_(node_[index], lazy_[index]);
    }

    // [遅延評価] index 番目 (0-indexed) の要素を含む区間について
    // 遅延データを伝播。根に近いものから処理される
    void propagate_from_top(int index) {
        index += size_;
        for(int h = height_; h > 0; h--) {
            int i = index >> h;
            if(lazy_[i] != identity_e_lazy_) {
                // 遅延データの情報を子に伝播させる
                // 左の子
                lazy_[i << 1] = combine_lazy_f_(lazy_[i << 1], lazy_[i]);
                // 右の子
                lazy_[i << 1 | 1] =
                    combine_lazy_f_(lazy_[i << 1 | 1], lazy_[i]);

                // 遅延データの情報を値データに反映させ、遅延データの値をリセット
                node_[i] = reflect_lazy(i);
                lazy_[i] = identity_e_lazy_;
            }
        }
    }

    // index 番目 (0-indexed) の要素を表す葉から順に値データを確定させる
    // (正確には葉に対しては行っておらず、葉の親から順に確定させている)
    void update_from_bottom(int index) {
        index = (index + size_) >> 1;
        while(index > 0) {
            node_[index] = combine_node_f_(reflect_lazy(index << 1),
                                           reflect_lazy(index << 1 | 1));
            index >>= 1;
        }
    }

    // メンバ変数
    int n_;
    int size_;
    int height_;
    MonoidTp identity_e_node_;
    OperatorTp identity_e_lazy_;
    MMtoM combine_node_f_;
    OOtoO combine_lazy_f_;
    MOtoM reflect_f_;
    vector<MonoidTp> node_;
    vector<OperatorTp> lazy_;
};

using MonoidTp = tuple<int, int, long long>;
using OperatorTp = int;

// セグメント木に使用する関数
// node は (0 の個数, 1 の個数, 転倒数) を、lazy は更新回数の偶奇を持っている
auto combine_node = [](MonoidTp lhs, MonoidTp rhs) {
    auto [l0, l1, l2] = lhs;
    auto [r0, r1, r2] = rhs;
    return make_tuple(l0 + r0, l1 + r1, l2 + r2 + 1LL * l1 * r0);
};

auto combine_lazy(OperatorTp lhs, OperatorTp rhs) { return lhs ^ rhs; };

auto reflect(MonoidTp node, OperatorTp lazy) {
    if(lazy == 0) {
        return node;
    } else {
        auto [n0, n1, n2] = node;
        return make_tuple(n1, n0, 1LL * n0 * n1 - n2);
    }
}

int main() {
    int N, Q;
    cin >> N >> Q;
    vector<int> A(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i];
    }

    LazySegmentTree<MonoidTp, OperatorTp> segtree(
        N, make_tuple(0, 0, 0LL), 0, combine_node, combine_lazy, reflect);

    // 数列の初期化 (要素を 1 つずつ見るとき、転倒数は 0 である)
    vector<MonoidTp> B(N);
    for(int i = 0; i < N; i++) {
        B[i] = make_tuple(A[i] == 0, A[i] == 1, 0LL);
    }
    segtree.build(B);

    // クエリを処理
    for(int i = 0; i < Q; i++) {
        int t, l, r;
        cin >> t >> l >> r;
        l--;
        if(t == 1) {
            segtree.update(l, r, 1);
        } else {
            cout << get<2>(segtree.fold(l, r)) << endl;
        }
    }
    return 0;
}

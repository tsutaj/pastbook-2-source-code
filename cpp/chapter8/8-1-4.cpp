#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
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

using MonoidTp = pair<int, int>;
using OperatorTp = int;

// セグメント木に使用する関数
auto combine_node = [](auto a, auto b) {
    return make_pair(a.first + b.first, a.second + b.second);
};

auto reflect = [](auto node, auto lazy) {
    if(lazy == 1) {
        swap(node.first, node.second);
    }
    return node;
};

int main() {
    int Q;
    cin >> Q;

    // (y 座標, x 座標の左端, x 座標の右端) の組
    vector<tuple<int, int, int>> queries;
    // 出現した x 座標を昇順に記録しておく (重複は除く)
    vector<int> xs;
    for(int i = 0; i < Q; i++) {
        int a, b, c, d;
        cin >> a >> b >> c >> d;
        xs.emplace_back(a);
        xs.emplace_back(c);
        queries.emplace_back(b, a, c);
        queries.emplace_back(d, a, c);
    }
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    sort(queries.begin(), queries.end());

    int N = xs.size();
    // セグメント木を作る
    // 値データは (黒色の数, 白色の数)
    // を、遅延データは反転をすべきかどうかを管理
    LazySegmentTree<MonoidTp, OperatorTp> segtree(
        N - 1, make_pair(0, 0), 0, combine_node,
        [](auto a, auto b) { return a ^ b; }, // xor
        reflect);

    // (黒色の数, 白色の数)
    vector<pair<int, int>> areas;
    for(int i = 0; i < (int)xs.size() - 1; i++) {
        areas.emplace_back(0, xs[i + 1] - xs[i]);
    }
    segtree.build(areas);

    // y 座標が小さい順にクエリを処理
    long long ans = 0;
    int i_query = 0;
    while(i_query < 2 * Q) {
        int y = get<0>(queries[i_query]);
        // y 座標が同じである限りクエリを見続ける
        while(i_query < 2 * Q && get<0>(queries[i_query]) == y) {
            int xL, xR;
            tie(std::ignore, xL, xR) = queries[i_query];
            int i_xL = lower_bound(xs.begin(), xs.end(), xL) - xs.begin();
            int i_xR = lower_bound(xs.begin(), xs.end(), xR) - xs.begin();
            segtree.update(i_xL, i_xR, 1);
            i_query++;
        }

        if(i_query < 2 * Q) {
            int nxt_y = get<0>(queries[i_query]);
            ans += 1LL * segtree.fold(0, N - 1).first * (nxt_y - y);
        }
    }
    cout << ans << endl;
    return 0;
}
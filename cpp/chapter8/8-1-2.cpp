#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

template <typename MonoidTp> class SegmentTree {

  public:
    SegmentTree(int n, MonoidTp identity_e,
                function<MonoidTp(MonoidTp, MonoidTp)> combine_f) {
        n_ = n;
        size_ = 1;
        while(size_ < n_) {
            size_ <<= 1;
        }
        identity_e_ = identity_e;
        combine_f_ = combine_f;
        node_.resize(2 * size_, identity_e_);
    }

    // 配列の各要素を登録する
    void build(const vector<MonoidTp> &array) {
        assert(array.size() == n_);
        for(int index = 0; index < n_; index++) {
            node_[index + size_] = array[index];
        }
        for(int index = size_ - 1; index >= 0; index--) {
            // 左の子の値と右の子の値を合成
            node_[index] = combine_f_(node_[index << 1], node_[index << 1 | 1]);
        }
    }

    // [一点更新] 位置 index (0-indexed) を値 value で更新
    void update(int index, MonoidTp value) {
        int i = size_ + index;
        node_[i] = value;
        while(i > 1) {
            i >>= 1;
            node_[i] = combine_f_(node_[i << 1], node_[i << 1 | 1]);
        }
    }

    // [区間取得] 区間 [l, r) (0-indexed) 内の要素について、
    // l 番目から順に combine_f を適用した結果を返す
    // (交換法則が前提になくてもよい)
    MonoidTp fold(int L, int R) {
        L += size_;
        R += size_;
        MonoidTp value_L = identity_e_;
        MonoidTp value_R = identity_e_;
        while(L < R) {
            if(L & 1) {
                value_L = combine_f_(value_L, node_[L]);
                L++;
            }
            if(R & 1) {
                R--;
                value_R = combine_f_(node_[R], value_R);
            }
            L >>= 1;
            R >>= 1;
        }
        return combine_f_(value_L, value_R);
    }

  private:
    int n_;
    int size_;
    MonoidTp identity_e_;
    function<MonoidTp(MonoidTp, MonoidTp)> combine_f_;
    vector<MonoidTp> node_;
};

// セグメント木のサイズ
const int SEGMENT_TREE_SIZE = 100010;
// プレゼントを表す型
using Present = pair<int, int>;

// 2 つのプレゼントの情報 a, b を比較し、ソート時にどの順番になるべきか決定する
bool compare_presents(Present &a, Present &b) {
    if(a.first != b.first) {
        return a.first < b.first; // w が小さいほど前に来る
    } else {
        return a.second > b.second; // h が大きいほど前に来る
    }
}

int main() {
    int N, Q;
    cin >> N >> Q;

    // C (色の情報) は、入力から 1 引いた状態で管理する
    vector<int> C(N);
    for(int i = 0; i < N; i++) {
        cin >> C[i], C[i]--;
    }

    // positions[c] := 玉の色が c であるインデックスのリスト
    // cursors[c] := positions[c] の何番目を参照するべきか
    vector<vector<int>> positions(N);
    vector<int> cursors(N);
    for(int i = 0; i < N; i++) {
        positions[C[i]].emplace_back(i);
    }

    // queries[L] := 与えられる区間の左端が L であるようなクエリのリスト
    vector<vector<pair<int, int>>> queries(N);
    for(int i = 0; i < Q; i++) {
        int L, R;
        cin >> L >> R;
        L--;
        queries[L].emplace_back(R, i);
    }

    // それぞれの色について、最左にあるものの場所を管理する
    SegmentTree<int> segtree(N, 0, [](auto a, auto b) { return a + b; });
    for(int color = 0; color < N; color++) {
        if(positions[color].size()) {
            segtree.update(positions[color][0], 1);
        }
    }

    // 区間の左端が L であるクエリを処理する
    vector<int> ans(Q);
    for(int L = 0; L < N; L++) {
        for(auto [R, i] : queries[L]) {
            ans[i] = segtree.fold(L, R);
        }

        // これより後には、区間の左端が L+1 以上のクエリしか見ない
        // L 番目の玉はもう見られないので、セグメント木を更新
        int color = C[L];
        segtree.update(L, 0);
        cursors[color]++;
        if(cursors[color] < (int)positions[color].size()) {
            segtree.update(positions[color][cursors[color]], 1);
        }
    }
    for(int i = 0; i < Q; i++) {
        cout << ans[i] << endl;
    }
    return 0;
}

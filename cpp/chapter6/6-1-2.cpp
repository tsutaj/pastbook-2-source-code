#include <algorithm>
#include <cassert>
#include <cstdio>
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

int main() {
    int N, Q;
    cin >> N >> Q;
    vector<int> A(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i];
    }

    SegmentTree<int> segment_tree(N, 0, [](int a, int b) { return a ^ b; });
    segment_tree.build(A);
    for(int i = 0; i < Q; i++) {
        int t, x, y;
        cin >> t >> x >> y;
        x--;
        if(t == 1) {
            A[x] ^= y;
            segment_tree.update(x, A[x]);
        } else {
            cout << segment_tree.fold(x, y) << endl;
        }
    }
    return 0;
}

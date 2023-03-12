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
    int N;
    cin >> N;
    vector<int> A(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i];
    }

    // B[v] := 値 v は、数列 A の何番目にあるか
    vector<int> B(N);
    for(int i = 0; i < N; i++) {
        B[A[i]] = i;
    }

    // 数列 A の転倒数を求める
    long long inversion = 0;
    SegmentTree<int> segment_tree(N, 0, [](int a, int b) { return a + b; });
    for(int position : B) {
        // その要素より右にあって、値が小さいものが何個あるか
        inversion += segment_tree.fold(position, N);
        segment_tree.update(position, +1);
    }

    // 数列を回転したときの転倒数を求める
    for(int i = 0; i < N; i++) {
        cout << inversion << endl;
        // 現在注目している要素 (数列の最も左にある要素) は A[i]
        // 転倒数の増加分 (その要素より大きいものの個数)
        inversion += N - A[i] - 1;
        // 転倒数の減少分 (その要素より小さいものの個数)
        inversion -= A[i];
    }
    return 0;
}

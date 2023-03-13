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

const long long INF = 1LL << 60;
int main() {
    int N, M;
    cin >> N >> M;
    vector<long long> A(N);
    for(int i = 0; i < N; i++) {
        cin >> A[i];
    }
    vector<long long> S_posi(N + 1); // 累積和
    vector<long long> S_nega(N + 1);
    for(int i = 0; i < N; i++) {
        S_posi[i + 1] = S_posi[i] + A[i];
        S_nega[i + 1] = S_nega[i] - A[i];
    }

    vector<tuple<int, int, int>> queries;
    for(int i = 0; i < M; i++) {
        int l, r, c;
        cin >> l >> r >> c;
        queries.emplace_back(r, l - 1, c);
    }
    sort(queries.begin(), queries.end());

    // seg0 の i 番目では、dp の i 番目の要素を管理
    SegmentTree<long long> seg0(N + 1, 0,
                                [](auto a, auto b) { return max(a, b); });
    // seg1 の i 番目では、dp の i 番目の要素から S[i] を引いたものを管理
    SegmentTree<long long> seg1(N + 1, -INF,
                                [](auto a, auto b) { return max(a, b); });
    seg1.build(S_nega);

    // 区間の右端が小さい順に処理
    for(auto [r, l, c] : queries) {
        // 区間と被らない場所から遷移
        long long v0 = seg0.fold(0, l) + S_posi[r] - S_posi[l] - c;
        // 区間と被る場所から遷移
        long long v1 = seg1.fold(l, r) + S_posi[r] - c;
        long long dp_value = max(v0, v1);

        // 動的計画法の配列の更新
        seg0.update(r, max(seg0.fold(r, r + 1), dp_value));
        seg1.update(r, max(seg1.fold(r, r + 1), dp_value - S_posi[r]));
    }
    cout << seg0.fold(0, N + 1) << endl;
    return 0;
}

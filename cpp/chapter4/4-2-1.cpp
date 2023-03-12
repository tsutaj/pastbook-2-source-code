#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

class UnionFind {
  public:
    // N 頂点で初期化
    UnionFind(int N) {
        size_.resize(N, 1);
        parent_.resize(N, -1);
    }

    // 代表値を求める (経路圧縮も行う)
    int find(int v) {
        if(parent_[v] == -1) {
            return v;
        } else {
            return parent_[v] = find(parent_[v]);
        }
    }

    // 2 つの集合を併合する
    // 実際に併合が起こったかどうかを返しておくと便利
    bool unite(int x, int y) {
        // 代表値 (木の根) を求める
        x = find(x);
        y = find(y);

        // すでに同じ集合に属するときは何もしない
        if(x == y) {
            return false;
        }

        // 木のサイズが小さいものを大きいものに併合
        if(size_[x] < size_[y]) {
            swap(x, y);
        }
        parent_[y] = x;
        size_[x] += size_[y];
        return true;
    }

    // 2 つの要素が同じ集合に属するか求める (あると便利)
    bool same(int x, int y) {
        x = find(x);
        y = find(y);
        return x == y;
    }

    // 集合のサイズを求める (あると便利)
    int size(int x) { return size_[find(x)]; }

  private:
    // 木の根 r に対して、size[r] は木の頂点数を表す
    // 根でない頂点に対しては不定 (参照しない)
    vector<int> size_;
    // 頂点 v に対して、parent[v] は v の親を表す (v が根なら -1)
    vector<int> parent_;
};

int main() {
    int N, Q;
    cin >> N >> Q;
    UnionFind uf(N);
    for(int i = 0; i < Q; i++) {
        int p, a, b;
        cin >> p >> a >> b;
        if(p == 0) {
            uf.unite(a, b);
        } else {
            if(uf.same(a, b)) {
                puts("Yes");
            } else {
                puts("No");
            }
        }
    }
    return 0;
}

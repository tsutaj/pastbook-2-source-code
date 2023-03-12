#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

int main() {
    int N, Y;
    cin >> N >> Y;

    for(int a = 0; a <= N; a++) {
        for(int b = 0; b <= N; b++) {
            // c の値は a, b, N から求められる (ループを回す必要がない)
            int c = N - a - b;
            // 枚数が負になることはない
            if(c < 0) {
                continue;
            }
            // 条件を満たしていれば直ちに終了する
            if(a * 10000 + b * 5000 + c * 1000 == Y) {
                cout << a << " " << b << " " << c << endl;
                exit(0);
            }
        }
    }

    // 条件を満たすものが存在しない
    puts("-1 -1 -1");
    return 0;
}

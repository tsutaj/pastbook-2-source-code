# 入力
N, K = map(int, input().split())
M = [list(map(int, input().split())) for i in range(N)]

# 定数
MOD = 1000000007
UNIT = [[1 if j == i else 0 for j in range(N)] for i in range(N)]  # 単位行列

# 行列乗算を実施する関数
def mul(A, B):
    N = len(A)
    C = [[0] * N for i in range(N)]
    for i in range(N):
        for j in range(N):
            for k in range(N):
                C[i][j] += A[i][k] * B[k][j]
                C[i][j] %= MOD
    return C

# M^K を計算する
R = UNIT  # 答え
T = M  # M^(2^n) の途中経過
while K > 0:
    if K % 2 == 1:
        R = mul(R, T)
    T = mul(T, T)
    K //= 2

# 答え
ans = 0
for i in range(N):
    ans += sum(R[i])
    ans %= MOD
print(ans)

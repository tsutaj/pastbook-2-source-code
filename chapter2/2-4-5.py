# 入力
H, R = map(int, input().split())
G = [list(map(int, input().split())) for i in range(R)]

# 定数
MOD = 1000000007
UNIT = [[1 if j == i else 0 for j in range(R)] for i in range(R)]  # 単位行列

# 行列 M を求める
M = [[0] * R for i in range(R)]
for s in range(R):
    # 配列 dp を定義する
    dp = [[0] * R for i in range(1 << R)]

    # 初期条件
    dp[(1 << s)][s] = 1

    # dp[S][i] を順に求めていく
    for S in range(1 << R):
        for i in range(R):
            if S & (1 << i) == 0:
                continue
            
            for j in range(R):
                if i == j or S & (1 << j) == 0:
                    continue
                if G[j][i] == 1:
                    dp[S][i] += dp[S & ~(1 << i)][j]
                    dp[S][i] %= MOD
    
    # M に反映させる
    for t in range(R):
        for S in range(1 << R):
            M[s][t] = (M[s][t] + dp[S][t]) % MOD

# 行列乗算を実施する関数
def mul(A, B):
    R = len(A)
    C = [[0] * R for i in range(R)]
    for i in range(R):
        for j in range(R):
            for k in range(R):
                C[i][j] += A[i][k] * B[k][j]
                C[i][j] %= MOD
    return C

# M^K を計算する
res = UNIT  # 答え
T = M  # M^(2^n) の途中経過
while H > 0:
    if H % 2 == 1:
        res = mul(res, T)
    T = mul(T, T)
    H //= 2

# 答え
print(res[0][0])

# 定数
MOD = 1000000007

# 入力
H, W = map(int, input().split())
S = [input() for r in range(H)]

# 配列 dp, X, Y, Z を定義する
dp = [[0] * W for r in range(H)]
X = [[0] * (W + 1) for r in range(H + 1)]
Y = [[0] * (W + 1) for r in range(H + 1)]
Z = [[0] * (W + 1) for r in range(H + 1)]

# 初期条件
dp[0][0] = 1
X[0][1] = Y[1][0] = Z[1][1] = 1

# dp[r][c] の値を順に求めていく
for r in range(H):
    for c in range(W):
        # マス (r, c) が壁であるとき
        if S[r][c] == '#':
            X[r][c + 1] = 0
            Y[r + 1][c] = 0
            Z[r + 1][c + 1] = 0
            continue
        
        # dp[r][c] の値を更新する
        dp[r][c] = (dp[r][c] + X[r][c] + Y[r][c] + Z[r][c]) % MOD

        # X, Y, Z を更新する
        X[r][c + 1] = (X[r][c] + dp[r][c]) % MOD
        Y[r + 1][c] = (Y[r][c] + dp[r][c]) % MOD
        Z[r + 1][c + 1] = (Z[r][c] + dp[r][c]) % MOD

# 出力
print(dp[H - 1][W - 1])

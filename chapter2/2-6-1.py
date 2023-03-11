# 定数
MOD = 1000000007

# 入力
K = input()
D = int(input())
N = len(K)

# 配列 dp を定義
dp = [[[0] * D for i in range(2)] for j in range(N + 1)]

# 初期条件
dp[0][0][0] = 1

# dp[i+1][j][r] を順に求めていく
for i in range(N):
    x = int(K[i])
    
    for r in range(D):
        # dp[i][0] から dp[i+1][0] への遷移
        dp[i+1][0][(r+x)%D] += dp[i][0][r]
        dp[i+1][0][(r+x)%D] %= MOD

        # dp[i][0] から dp[i+1][1] への遷移
        for j in range(x):
            dp[i+1][1][(r+j)%D] += dp[i][0][r]
            dp[i+1][1][(r+j)%D] %= MOD

        # dp[i][1] から　dp[i+1][1] への遷移
        for j in range(10):
            dp[i+1][1][(r+j)%D] += dp[i][1][r]
            dp[i+1][1][(r+j)%D] %= MOD

# 答え（0 の場合が含まれるので除外する）
print((dp[N][0][0] + dp[N][1][0] - 1) % MOD)

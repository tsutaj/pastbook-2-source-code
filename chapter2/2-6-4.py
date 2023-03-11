# 定数
MOD = 1000000007

# 入力
N = int(input())
s = input()

# 配列 dp を定義する
dp = [[0] * (N + 1) for i in range(N + 1)]

# 初期条件
for j in range(N):
    dp[1][j] = 1

# dp[i+1][j] を順に求めていく
for i in range(1, N):
    # dp[i] の累積和をとる
    sumdp = [0] * (N + 2)
    for j in range(N + 1):
        sumdp[j + 1] = sumdp[j] + dp[i][j]

    # dp[i+1][j] の値の更新
    for j in range(N - i + 1):
        if s[i-1] == '<':
            dp[i+1][j] = (sumdp[N-i+1] - sumdp[j+1]) % MOD
        else:
            dp[i+1][j] = (sumdp[j+1]) % MOD

# 答え
print(dp[N][0])

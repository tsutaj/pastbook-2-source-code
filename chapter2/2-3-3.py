# 入力
M, N = map(int, input().split())
S = input()
T = input()

# 十分大きい値
INF = max(M, N)

# DP
dp = [[INF] * (N + 1) for i in range(M + 1)]

# 初期条件
dp[0][0] = 0

# dp[i][j] の値を順に更新していく
for i in range(0, M + 1):
    for j in range(0, N + 1):
        # 場合 1：変更
        if i > 0 and j > 0:
            dp[i][j] = min(dp[i][j], dp[i - 1][j - 1] + (S[i - 1] != T[j - 1]))

        # 場合 2：削除
        if i > 0:
            dp[i][j] = min(dp[i][j], dp[i - 1][j] + 1)

        # 場合 3：挿入
        if j > 0:
            dp[i][j] = min(dp[i][j], dp[i][j - 1] + 1)

# 出力
print(dp[M][N])

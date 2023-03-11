# 十分大きい値
INF = 2 ** 60

# 入力
N, M, K = map(int, input().split())
A = [int(input()) for i in range(N)]

# 前処理
cost = [[0] * (M + 1) for i in range(N + 1)]
for i in range(N):
    min_value, max_value = INF, 0
    for j in range(i, min(N, M + i)):
        min_value = min(min_value, A[j])
        max_value = max(max_value, A[j])
        cost[i][j - i + 1] = K + (max_value - min_value) * (j - i + 1)

# DP
dp = [INF] * (N + 1)
dp[0] = 0
for i in range(1, N + 1):
    for j in range(max(i - M, 0), i):
        dp[i] = min(dp[i], dp[j] + cost[j][i - j])

print(dp[N])

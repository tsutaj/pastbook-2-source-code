# 入力
N = int(input())
a = list(map(int, input().split()))

# 累積和を求める
S = [0] * (N + 1)
for i in range(N):
    S[i + 1] = S[i] + a[i]

# 配列 dp を定義する
INF = 2 ** 60  # 十分大きい値
dp = [[INF] * (N + 1) for i in range(N + 1)]

# 初期条件
for i in range(N):
    dp[i][i + 1] = 0

# dp[i][j] の値を順に求めていく
for between in range(2, N + 1):
    for i in range(0, N):
        j = i + between  # 区間の右端
        if j > N:
            break
        
        # 区間 [i, j) の分割位置で場合分けする
        for k in range(i + 1, j):
            dp[i][j] = min(dp[i][j], dp[i][k] + dp[k][j] + S[j] - S[i])

# 答え
print(dp[0][N])

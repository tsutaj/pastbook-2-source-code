# 入力
N = int(input())
c = [list(map(int, input().split())) for i in range(N+1)]

# 配列 dp を定義
INF = 2 ** 60  # 無限大を表す値
dp = [INF] * (N+1)

# 初期値
dp[0] = 0

# i が小さい順に dp の値を求めていく
for i in range(1, N+1):
    for j in range(0, i):
        dp[i] = min(dp[i], dp[j] + c[j][i])

# 答えを求める
print(dp[N])

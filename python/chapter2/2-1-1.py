# 入力を受け取る
N, W = map(int, input().split())
vw = [list(map(int, input().split())) for i in range(N)]
w = [vw[i][0] for i in range(N)]
v = [vw[i][1] for i in range(N)]

# 配列 dp を定義
INF = 2 ** 60  # 十分大きい値
dp = [[-INF] * (W + 1) for _ in range(N + 1)]

# 初期値 (0 個の品物から重さ 0 で選んで、価値の総和は 0)
dp[0][0] = 0

# i が小さい順に dp の値を求めていく
for i in range(N):
    for j in range(W + 1):
        # 品物 i を選ばない場合
        dp[i+1][j] = max(dp[i+1][j], dp[i][j])

        # 品物 i を選ぶ場合
        if j >= w[i]:
            dp[i+1][j] = max(dp[i+1][j], dp[i][j-w[i]] + v[i])

# 答えを求める
print(max(dp[N]))

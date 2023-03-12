# 入力
N, H = map(int, input().split())
act = [tuple(map(int, input().split())) for i in range(N)]

# 活動を a[i] / b[i] が大きい順にソートする
act.sort(key=lambda x: x[1] / x[0])

# 配列 dp を定義する
dp = [[0] * (H + 1) for i in range(N + 1)]

# dp[i + 1] の値を順に求めていく
for i in range(N):
    a, b = act[i][0], act[i][1]
    for h in range(H + 1):
        dp[i + 1][h] = max(dp[i + 1][h], dp[i][h])
        dp[i + 1][max(h - b, 0)] = max(dp[i + 1][max(h - b, 0)], dp[i][h] + a * h)

# 答え
print(max(dp[N]))

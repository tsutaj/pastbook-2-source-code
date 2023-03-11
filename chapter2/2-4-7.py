from functools import cmp_to_key

# 入力
N = int(input())
act = [tuple(map(int, input().split())) for i in range(N)]

# 活動を所定の順序に並び替える
def func(act1, act2):
    if min(act1[1], act2[1] - act1[0]) <= min(act2[1], act1[1] - act2[0]):
        return 1
    else:
        return -1
act.sort(key=cmp_to_key(func))

# 配列 dp を定義する
MAX_S = 20000
dp = [[0] * (MAX_S + 1) for i in range(N + 1)]

# dp[i + 1] の値を順に求めていく
for i in range(N):
    w, s, v = act[i][0], act[i][1], act[i][2]
    for j in range(MAX_S + 1):
        dp[i + 1][j] = max(dp[i + 1][j], dp[i][j])
        if j <= s:
            dp[i + 1][j + w] = max(dp[i + 1][j + w], dp[i][j] + v)

# 答え
print(max(dp[N]))

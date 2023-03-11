# 入力
N = int(input())
A = [list(map(int, input().split())) for i in range(N)]

# 前計算によって、集合 X をグループ化したときのスコアを求める
score = [0] * (1 << N)
for S in range(1 << N):
    for i in range(N):
        for j in range(i + 1, N):
            if S & (1 << i) == 0 or S & (1 << j) == 0:
                continue
            score[S] += A[i][j]

# 配列 dp を定義する
dp = [0] * (1 << N)
for S in range(1 << N):
    T = S
    while T >= 0:
        T &= S
        dp[S] = max(dp[S], dp[T] + score[S - T])
        T -= 1

# 答え
print(dp[(1 << N) - 1])

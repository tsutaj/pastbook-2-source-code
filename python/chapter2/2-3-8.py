# 入力
N, M = map(int, input().split())
A = [int(input()) - 1 for i in range(N)]

# 前処理
num = [[0] * (N + 1) for i in range(M)]
for i in range(M):
    for j in range(N):
        num[i][j + 1] = num[i][j] + (A[j] == i)

# 配列 dp を宣言する
INF = 2 ** 60  # 十分大きい値
dp = [INF] * (1 << M)

# 初期条件
dp[0] = 0

# dp[S] の値を順に求めていく
for S in range(1, 1 << M):
    # r：数列 A の要素にうち、整数値 S で表される集合に含まれる個数
    right = 0
    for i in range(M):
        if (S & (1 << i)) > 0:
            right += num[i][N]

    # 整数値 S で表される集合の要素のうち、最後に並べる要素で場合分け
    for i in range(M):
        if S & (1 << i) == 0:
            continue

        # right のうち、要素値が i でないものの個数
        left = right - num[i][N]

        # 更新式
        add = right - left - (num[i][right] - num[i][left])
        dp[S] = min(dp[S], dp[S & ~(1 << i)] + add)

# 出力
print(dp[-1])

# 数え上げの法
MOD = 1000000007

# 入力
N = int(input())
a = [list(map(int, input().split())) for i in range(N)]

# 配列 dp を定義する
dp = [0] * (1 << N)

# 初期条件
dp[0] = 1

# dp[S] の値を順に求めていく
for S in range(1, 1 << N):
    # 整数値 S で表される集合の要素数
    i = bin(S).count('1')

    # 最後の男性 i - 1 に割り当てられる女性で場合分け
    for j in range(N):
        if a[i - 1][j] == 1 and (S & (1 << j)) > 0:
            dp[S] += dp[S & ~(1 << j)]
            dp[S] %= MOD

# 答え
print(dp[-1])

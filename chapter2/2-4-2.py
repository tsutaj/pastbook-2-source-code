# 定数
MOD = 1000000007

# 入力
N, K = map(int, input().split())
a = list(map(int, input().split()))

# 配列 dp, sum_dp を宣言する
dp = [[0] * (K + 1) for i in range(N + 1)]
sum_dp = [[0] * (K + 2) for i in range(N + 1)]

# 初期条件
dp[0][0] = 1

# dp[i+1][j] の値を順に求めていく
for i in range(0, N):
    # dp[i] の累積和を求める
    for j in range(0, K + 1):
        sum_dp[i][j + 1] = (sum_dp[i][j] + dp[i][j]) % MOD

    for j in range(0, K + 1):
        dp[i + 1][j] = (sum_dp[i][j + 1] - sum_dp[i][max(j - a[i], 0)]) % MOD

# 出力
print(dp[N][K])

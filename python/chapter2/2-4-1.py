# 定数
MOD = 1234567

# 入力
N, P = map(int, input().split())
H = [int(input()) for i in range(N)]

# 各 i に対して、数列 H の区間 [x, i) の総和が P 以下になる最小の x を求める
x = [0] * (N + 1)
left = 0
sum_interval = 0  # 現在考えている区間の総和
for right in range(0, N + 1):
    # 区間 [left, right) の総和が P より大きい限り、left を増やす
    while left < right and sum_interval > P:
        sum_interval -= H[left]
        left += 1
    
    # 解を記録
    x[right] = left

    # 区間の右端 right を増やすことに伴い、区間の総和に足す
    if right < N:
        sum_interval += H[right]

# メモリ節約のため、すでに不要になった H を削除
del H

# 配列 dp, sum_dp を宣言する
dp = [0] * (N + 1)
sum_dp = [0] * (N + 2)

# 初期条件
dp[0] = 1
sum_dp[1] = 1

# dp[i] の値を順に求めていく
for i in range(1, N + 1):
    dp[i] = (sum_dp[i] - sum_dp[x[i]]) % MOD
    sum_dp[i + 1] = (sum_dp[i] + dp[i]) % MOD

# 出力
print(dp[N])

# 定数
MOD = 1000000007
T = "atcoder"

# a に b を足して MOD をとる関数
def add(a, b):
    a += b
    if a >= MOD:
        a -= MOD
    return a

# 入力
N = int(input())
S = input()

# 配列 dp を宣言する
dp = [[0]*(len(T)+1) for _ in range(N+1)]

# 初期条件
dp[0][0] = 1

# dp[i + 1][j] の値を順に求めていく
for i in range(N):
    for j in range(0, len(T) + 1):
        # S[i] を選ばない場合
        dp[i + 1][j] = add(dp[i + 1][j], dp[i][j])

        # S[i] を選ぶ場合
        if j > 0 and S[i] == T[j - 1]:
            dp[i + 1][j] = add(dp[i + 1][j], dp[i][j - 1])

print(dp[N][len(T)])

# 入力
N = int(input())
S = input()
C = list(map(int, input().split()))
D = list(map(int, input().split()))

# 配列 dp を定義する
INF = 2 ** 60  # 十分大きい値
dp = [[INF] * (N + 1) for i in range(N + 1)]

# 初期条件
dp[0][0] = 0

# dp[i + 1][j] の値を順に求めていく
for i in range(N):
    for j in range(N + 1):
        # S[i] に何もしない場合
        if S[i] == '(' and j > 0:
            dp[i + 1][j] = min(dp[i + 1][j], dp[i][j - 1])
        if S[i] == ')' and j < N:
            dp[i + 1][j] = min(dp[i + 1][j], dp[i][j + 1])

        # S[i] を変更するとき    
        if S[i] == '(' and j < N:
            dp[i + 1][j] = min(dp[i + 1][j], dp[i][j + 1] + C[i])
        if S[i] == ')' and j > 0:
            dp[i + 1][j] = min(dp[i + 1][j], dp[i][j - 1] + C[i])

        # S[i] を削除するとき
        if S[i] == '(':
            dp[i + 1][j] = min(dp[i + 1][j], dp[i][j] + D[i])
        if S[i] == ')':
            dp[i + 1][j] = min(dp[i + 1][j], dp[i][j] + D[i])     

# 出力
print(dp[N][0])

# 入力
S = input()
N = len(S)

# 配列 dp を定義する
dp = [[0] * (N + 1) for i in range(N + 1)]

# dp[i][j] の値を順に更新していく
for between in range(2, N + 1):
    for i in range(0, N):
        j = i + between  # 区間の右端
        if j > N:
            break

        # 場合 1
        if S[i] == 'i' and S[j - 1] == 'i':
            for k in range(i + 1, j - 1):
                if S[k] == 'w' and \
                    dp[i + 1][k] * 3 == k - i - 1 and \
                    dp[k + 1][j - 1] * 3 == j - k - 2:
                    dp[i][j] = (j - i) // 3
        
        # 場合 2
        for k in range(i + 1, j - 1):
            dp[i][j] = max(dp[i][j], dp[i][k] + dp[k][j])

# 出力
print(dp[0][N])

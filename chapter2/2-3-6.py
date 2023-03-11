# 入力
N = int(input())
A = [list(map(int, input().split())) for i in range(N)]

# 配列 dp を定義する
INF = 2 ** 60  # 十分大きい値
dp = [[INF] * N for i in range(1 << N)]

# 初期条件
dp[1][0] = 0

# dp[S][i] の値を順に求めていく
for S in range(1 << N):
    for i in range(N):
        # 都市 i が S に含まれない場合はスキップ
        if S & (1 << i) == 0:
            continue
        
        # 都市 j で場合分け
        for j in range(N):
            if j == i or S & (1 << j) == 0:
                continue
        
            # 更新式
            dp[S][i] = min(dp[S][i], dp[S & ~(1 << i)][j] + A[j][i])

# 答え（最後に都市 0 に戻るケースをすべて探索する）
ans = INF
for i in range(1, N):
    ans = min(ans, dp[(1 << N) - 1][i] + A[i][0])
print(ans)

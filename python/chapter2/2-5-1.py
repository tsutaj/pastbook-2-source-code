# スタックオーバーフローを防ぐ
import sys

sys.setrecursionlimit(10 ** 6)

# 入力
N = int(input())
G = [[] for v in range(N)]
for i in range(N - 1):
    x, y = map(int, input().split())
    x, y = x - 1, y - 1
    G[x].append(y)
    G[y].append(x)

# 配列 dp を定義する
dp = [0] * N

# v: 現在探索中の頂点，p: v の親（v が根のときは p = -1）
def dfs(v, p):
    dp[v] = 1

    # v に隣接した頂点を見ていく
    for c in G[v]:
        # c が親である場合は除く
        if c == p:
            continue
        
        # 子頂点 c を再帰的に探索する
        dfs(c, v)
    
        # dp[v] の値を更新する
        dp[v] += dp[c]

# 根である頂点 0 を呼び出す
dfs(0, -1)

# 答え
for v in range(N):
    print(dp[v])

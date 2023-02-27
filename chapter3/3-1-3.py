def impossible():
    print(-1)
    exit()


# N, M は空白区切りで与えられる
N, M = map(int, input().split())
G = [[] for _ in range(N)]
edges = [[] for _ in range(M)]

# A, B, C は空白区切りで 1 行ずつ与えられる
for i in range(M):
    a, b, c = map(int, input().split())
    a -= 1
    b -= 1
    G[a].append([b, c])
    G[b].append([a, c])
    edges[i] = [a, b, c]

# 頂点 0 (0-indexed) に x を書き込んだとする。他の頂点は何を書き込めるか？
INF = 1e18
positive = [0 for _ in range(N)]
negative = [0 for _ in range(N)]
exists_posi = [False for _ in range(N)]
exists_nega = [False for _ in range(N)]
checked = [False for _ in range(N)]

# x としてあり得る範囲 [lbound, ubound]
lbound, ubound = 0, INF

# BFS で頂点を探索
queue = [0]
exists_posi[0] = True
while len(queue) > 0:
    u = queue.pop()
    for v, c in G[u]:
        updated = False
        # 頂点 u で positive[u] + x という値だったとき
        # 頂点 v では c - positive[u] - x となり、これが 0 以上になる
        # x <= c - positive[u]
        if exists_posi[u] and not exists_nega[v]:
            exists_nega[v] = True
            negative[v] = c - positive[u]
            ubound = min(ubound, c - positive[u])
            updated = True
        # 頂点 u で negative[u] - x という値だったとき
        # 頂点 v では c - negative[u] + x となり、これが 0 以上になる
        # x >= negative[u] - c
        if exists_nega[u] and not exists_posi[v]:
            exists_posi[v] = True
            positive[v] = c - negative[u]
            lbound = max(lbound, negative[u] - c)
            updated = True
        if updated:
            queue.append(v)

if lbound > ubound:
    impossible()

ans = [0 for _ in range(N)]
x = None

# negative[i] - x = positive[i] + x より、
# negative と positive の情報が両方あれば x の値は一意に定まる
for i in range(N):
    if exists_posi[i] and exists_nega[i]:
        if (negative[i] - positive[i]) % 2 != 0:
            impossible()
        x = (negative[i] - positive[i]) // 2
        break
if x is None:
    x = ubound

for i in range(N):
    if exists_posi[i]:
        ans[i] = positive[i] + x
    elif exists_nega[i]:
        ans[i] = negative[i] - x
    if ans[i] < 0:
        impossible()

# 必要十分ではないため、最後にチェック
for a, b, c in edges:
    if ans[a] + ans[b] != c:
        impossible()
for i in range(N):
    print(ans[i])

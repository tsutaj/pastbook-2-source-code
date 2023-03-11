import heapq
from math import sqrt

N, M = map(int, input().split())
PX = []
PY = []
for _ in range(N):
    x, y = map(int, input().split())
    PX.append(x)
    PY.append(y)
CX = []
CY = []
R = []
for _ in range(M):
    x, y, r = map(int, input().split())
    CX.append(x)
    CY.append(y)
    R.append(r)
# プリム法でグラフ G の最小全域木を求め、
# その重みの合計を返す関数
def prim(G):
    marked = [False] * len(G)
    marked_count = 0
    marked[0] = True
    marked_count += 1
    Q = []
    for j, c in G[0]:
        heapq.heappush(Q, (c, j))
    sum = 0
    while marked_count < len(G):
        c, i = heapq.heappop(Q)
        if marked[i]:
            continue
        marked[i] = True
        marked_count += 1
        sum += c
        for (j, c) in G[i]:
            if marked[j]:
                continue
            heapq.heappush(Q, (c, j))
    return sum


ans = []
# すべての環状交差点の部分集合について、最小全域木を求める
for subset in range(1 << M):
    S = []
    for i in range(M):
        if (subset >> i) & 1 == 1:
            S.append(i)
    # すべてのタワーと部分集合に含まれる環状交差点のグラフを作る
    G = [[] for _ in range(N + len(S))]
    # タワー i と j を結ぶ道路の長さ d を計算する
    for i in range(N):
        for j in range(N):
            d = sqrt((PX[i] - PX[j]) ** 2 + (PY[i] - PY[j]) ** 2)
            G[i].append((j, d))
    # タワー i と環状交差点 S[j] を結ぶ道路の長さ e を計算する
    for i in range(N):
        for j in range(len(S)):
            e = abs(sqrt((PX[i] - CX[S[j]]) ** 2 + (PY[i] - CY[S[j]]) ** 2) - R[S[j]])
            G[i].append((N + j, e))
            G[N + j].append((i, e))
    # 環状交差点 S[i] と S[j] を結ぶ道路の長さ f を計算する
    for i in range(len(S)):
        for j in range(len(S)):
            if R[S[i]] < R[S[j]]:
                continue
            c = sqrt((CX[S[i]] - CX[S[j]]) ** 2 + (CY[S[i]] - CY[S[j]]) ** 2)
            if R[S[i]] + R[S[j]] <= c:
                f = c - R[S[i]] - R[S[j]]
            elif R[S[i]] - R[S[j]] < c < R[S[i]] + R[S[j]]:
                f = 0
            else:
                f = R[S[i]] - R[S[j]] - c
            G[N + i].append((N + j, f))
            G[N + j].append((N + i, f))
    # グラフ G の最小全域木を求める
    ans.append(prim(G))
print(min(ans))

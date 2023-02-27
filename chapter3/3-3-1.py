from collections import deque
from heapq import heappush, heappop

Q = int(input())
# ヒープ
X = []
# キュー (実行速度のために deque モジュールを使用)
Y = deque()

for i in range(Q):
    query = list(map(int, input().split()))
    if query[0] == 1:
        Y.append(query[1])
    elif query[0] == 2:
        if len(X) > 0:
            print(heappop(X))
        else:
            print(Y.popleft())
    elif query[0] == 3:
        while len(Y) > 0:
            heappush(X, Y.popleft())

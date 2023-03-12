N, M = map(int, input().split())
A = [0] * N
B = [0] * N
for i in range(N):
    A[i], B[i] = map(int, input().split())
C = [0] * M
D = [0] * M
for i in range(M):
    C[i], D[i] = map(int, input().split())


def check(X):
    P = [0] * N
    for i in range(N):
        P[i] = B[i] - A[i] * X
    Q = [0] * M
    for i in range(M):
        Q[i] = D[i] - C[i] * X
    P.sort(reverse=True)
    Q.sort(reverse=True)
    sum1 = sum(P[:5])
    sum2 = sum(P[:4]) + Q[0]
    if sum1 >= 0 or sum2 >= 0:
        return True
    else:
        return False


X_ok = 0.0
# 組み合わせが存在することを確認した X の値
X_ng = 2e5
# 組み合わせが存在しないことを確認した X の値
# 多めに 100 回試行する
for _ in range(100):
    # 二分探索の 1 回の試行
    X = (X_ok + X_ng) / 2
    if check(X):
        # 合成後のモンスターの強さが X 以上となるような
        # モンスターの組み合わせは存在する
        X_ok = X
    else:
        # 合成後のモンスターの強さが X 以上となるような
        # モンスターの組み合わせは存在しない
        X_ng = X
print(X_ok)

N = int(input())
A = list(map(int, input().split()))
# 平均値が X 以上になるような選び方が存在するか調べる関数
def check_average(X):
    B = [0] * N
    for i in range(N):
        B[i] = A[i] - X
    S = [0] * (N + 1)
    T = [0] * (N + 1)
    for i in range(N):
        S[i + 1] = max(T[i], S[i]) + B[i]
        T[i + 1] = S[i]
    return max(S[N], T[N]) >= 0


# 平均値の最大値を二分探索で求める
X_ok = 0.0
X_ng = 1e10
for _ in range(100):
    X = (X_ok + X_ng) / 2.0
    if check_average(X):
        X_ok = X
    else:
        X_ng = X
max_average = X_ok
print(max_average)
# 中央値が X 以上になるような選び方が存在するか調べる関数
def check_median(X):
    B = [0] * N
    for i in range(N):
        if A[i] >= X:
            B[i] = 1
        else:
            B[i] = -1

    S = [0] * (N + 1)
    T = [0] * (N + 1)
    for i in range(N):
        S[i + 1] = max(T[i], S[i]) + B[i]
        T[i + 1] = S[i]
    return max(S[N], T[N]) > 0


# 中央値の最大値を二分探索で求める
X_ok = 0
X_ng = 10**10
while X_ng - X_ok > 1:
    X = (X_ok + X_ng) // 2
    if check_median(X):
        X_ok = X
    else:
        X_ng = X
max_median = X_ok
print(max_median)

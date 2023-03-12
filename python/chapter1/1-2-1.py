N, L = map(int, input().split())
A = list(map(int, input().split()))
S = [0] * (N + 1)
for i in range(N):
    S[i + 1] = S[i] + A[i]


def check(X):
    l = 0
    r = 0
    C = [0] * (N + 1)
    D = [0] * (N + 1)
    D[0] += 1
    D[1] -= 1
    for i in range(N):
        if i > 0:
            C[i] = C[i - 1] + D[i]
        else:
            C[i] = D[i]
        if C[i] == 0:
            continue
        while l <= N and S[l] - S[i] < X:
            l += 1
        if l > N:
            continue
        while r + 1 <= N and S[r + 1] - S[i] <= L:
            r += 1
        D[l] += 1
        if r + 1 <= N:
            D[r + 1] -= 1
    C[N] = C[N - 1] + D[N]
    return C[N] > 0


# check(X) が True となるような最大の X を求める二分探索
X_ok = 0
X_ng = L + 1
while X_ng - X_ok > 1:
    X = (X_ng + X_ok) // 2
    if check(X):
        X_ok = X
    else:
        X_ng = X
print(X_ok)

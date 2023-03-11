N, K = map(int, input().split())
A = list(map(int, input().split()))
# 累積和を表す配列
S = [0] * (N + 1)
for i in range(N):
    S[i + 1] = S[i] + A[i]
# S はソートしてしまってしまってよい
S = sorted(S)
# 条件を満たす (i,j) の個数が 2K 以上になるような X の最小値を
# 二分探索で求める
ok = 10**15
ng = -1
while ok - ng > 1:
    X = (ok + ng) // 2
    # 各 j について S[j]-X <= S[i] <= S[j]+X となる
    # i の数を数える
    count = 0
    L = 0
    R = 0
    for j in range(N + 1):
        while S[L] < S[j] - X:
            L += 1
        while R < N and S[R + 1] <= S[j] + X:
            R += 1
        count += R - L
    if count >= 2 * K:
        ok = X
    else:
        ng = X
print(ok)

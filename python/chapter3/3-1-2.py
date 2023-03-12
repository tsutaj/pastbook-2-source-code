N = int(input())

# 数列は空白区切りで 1 行で与えられる
# 昇順にソートしておく
A = sorted(list(map(int, input().split())))
B = sorted(list(map(int, input().split())))
C = sorted(list(map(int, input().split())))

# b より小さい最大の a と、b より大きい最小の c の位置を管理
# 数列はソート済みであるので、これらの値は単調に増加する
index_a = 0
index_c = 0

ans = 0
for i in range(N):
    while index_a < N and A[index_a] < B[i]:
        index_a += 1
    while index_c < N and C[index_c] <= B[i]:
        index_c += 1

    # a の取りうる範囲: [0, index_a)
    range_a = index_a
    # c の取りうる範囲: [index_c, N)
    range_c = N - index_c
    ans += range_a * range_c

print(ans)

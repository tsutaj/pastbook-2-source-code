N = int(input())
# 数列は空白区切りで与えられる
A = list(map(int, input().split()))

v_xor, v_sum = 0, 0
r = 0
ans = 0
for l in range(N):
    while r < N and v_xor ^ A[r] == v_sum + A[r]:
        v_xor ^= A[r]
        v_sum += A[r]
        r += 1
    ans += r - l

    # 長さが 1 ならば必ず条件を満たすので、長さが正かどうかの場合分けは不要
    v_xor ^= A[l]
    v_sum -= A[l]

print(ans)

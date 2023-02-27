S = input()
K = int(input())

N = len(S)
l, r = 0, 0
dots = 0
ans = 0
for l in range(N):
    # 区間の中の '.' の個数が K 以下なら、区間を伸ばして良い
    while r < N:
        is_dot = (S[r] == '.')
        if dots + is_dot > K:
            break
        dots += is_dot
        r += 1
    
    ans = max(ans, r - l)
    if r - l >= 1:
        dots -= (S[l] == '.')
    else:
        r += 1

print(ans)

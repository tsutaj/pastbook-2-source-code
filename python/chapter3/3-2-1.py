# N, K は空白区切りで与えられる
N, K = map(int, input().split())
# S は 1 行ずつ与えられる
S = [int(input()) for i in range(N)]

# 0 が含まれているときは、すべての要素を含んだ区間が答えとなる
if 0 in S:
    print(N)
    exit()

# 0-indexed で処理
l, r = 0, 0
prod = 1
ans = 0

# l は固定 (最も長い区間を求め終えるたびに 1 ずつ増やす)
for l in range(N):
    # 条件を満たしている限り、r を 1 ずつ増やし続ける (区間を伸ばす)
    while r < N and prod * S[r] <= K:
        prod *= S[r]
        r += 1
    # 左端を固定したときの、条件を満たす中で最も長い区間が求められた
    ans = max(ans, r - l)
    # l の値を 1 増やす (区間を縮める)
    # l と r が等しい場合に注意！
    if r - l >= 1:
        prod //= S[l]
    else:
        r += 1

print(ans)

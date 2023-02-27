N, Y = map(int, input().split())
for a in range(N + 1):
    for b in range(N + 1):
        # c の値は a, b, N から求められる (ループを回す必要がない)
        c = N - a - b
        # 枚数が負になることはない
        if c < 0:
            continue
        # 条件を満たしていれば直ちに終了する
        if a * 10000 + b * 5000 + c * 1000 == Y:
            print(a, b, c)
            exit(0)

# 条件を満たすものが存在しない
print("-1 -1 -1")

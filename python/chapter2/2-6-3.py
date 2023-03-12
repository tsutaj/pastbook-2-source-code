# 自身と上下左右を表す配列
DX = [0, 1, 0, -1, 0]
DY = [0, 0, 1, 0, -1]

# 入力を受け取り、整数値に直す
row_grid = [input() for i in range(4)]
grid = 0
for i in range(4):
    for j in range(4):
        if row_grid[i][j] == "#":
            grid |= 1 << (i * 4 + j)

# 配列 dp を定義する
INF = 1 << 60  # 十分大きい値
dp = [INF] * (1 << 16)

# 初期条件
dp[0] = 0.0

# dp[S] の値を順に求めていく
for S in range(2 ** 16):
    # 狙うマスを順に試す
    for i in range(4):
        for j in range(4):
            # 自身と周囲 5 マスにボールが当たった後のグリッドを求める
            S2s = []

            # ボールが当たるマスを順に調べる
            for dx, dy in zip(DX, DY):
                i2, j2 = i + dx, j + dy

                # ボールがグリッド外に出る場合は変化しない
                if i2 < 0 or i2 >= 4 or j2 < 0 or j2 >= 4:
                    continue

                # ボールが的のないマスに当たる場合も変化しない
                if S & (1 << (i2 * 4 + j2)) == 0:
                    continue

                # 新たなグリッド
                S2 = S & ~(1 << (i2 * 4 + j2))
                S2s.append(S2)
    
            # dp[S] を求める
            if len(S2s) > 0:
                dp[S] = min(dp[S], sum(dp[x] for x in S2s) / len(S2s) + 5.0 / len(S2s))

# 答え
print(dp[grid])

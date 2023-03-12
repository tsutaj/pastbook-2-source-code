# あまりを求めるための法
MOD = 10007

# 入力を受け取る
N = int(input())
responsible = input()

# 配列 dp を定義
dp = [[0] * 8 for _ in range(N + 1)]

# 初期値 (左上マスが黒色であることを表す)
dp[0][(1 << 0)] = 1

# d が小さい順に dp の値を求めていく
for d in range(N):
    for S in range(8):
        # d 列目は S[d] を責任者に持つ必要がある
        if responsible[d] == 'J' and (S & (1 << 0)) == 0:
            continue
        if responsible[d] == 'O' and (S & (1 << 1)) == 0:
            continue
        if responsible[d] == 'I' and (S & (1 << 2)) == 0:
            continue

        # d-1 列目の塗られ方で場合分けする
        for T in range(8):
            # 左右に黒色マスが隣接している箇所があるかを確認
            if S & T != 0:
                dp[d+1][S] = (dp[d+1][S] + dp[d][T]) % MOD

# 答えを求める
print(sum(dp[N]) % MOD)

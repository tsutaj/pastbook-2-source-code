N = int(input())
A = list(map(int, input().split()))

# 番兵
A.append(0)
N += 1

# stack では (長方形の高さ、左端の添字) を管理する
stack = []
ans = 0
for i in range(N):
    left_index = i
    while len(stack) > 0 and stack[-1][0] > A[i]:
        value, left_index = stack[-1]
        length = i - left_index
        stack.pop()
        ans = max(ans, value * length)
    stack.append([A[i], left_index])

print(ans)

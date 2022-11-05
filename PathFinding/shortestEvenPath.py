
def even_path(graph, s, t):
    if s == t: return 0
    n = len(graph) + 1
    G = [[]] * 2 * n
    for v0 in range(n-1):
        for u in graph[v0]:
            v = v0 + 1
            G[2*v-1] = G[2*v-1].copy() + [2 * u]
            G[2*v] = G[2*v].copy() + [2 * u - 1]
    visited = [0] * 2 * n
    level, flag = 1, 0
    queue = [2*s-1]
    end = 2*s-1
    visited[2*s-1] = 1
    while queue != []:
        v = queue.pop(0)
        if v == end:
            flag = 1
        for u in G[v]:
            if visited[u] == 0:
                if u == 2 * t - 1:
                    print('visited: ', visited)
                    print('graph: ', G)
                    return level / 2
                else:
                    visited[u] = 1
                    queue.append(u)
                    if flag == 1:
                        end = u
        if flag == 1:
            level += 1
            flag = 0
    print('visited: ', visited)
    print('graph: ', G)
    return -1

"""
graph = [[2], [1, 3], [2, 4, 5, 6], [3, 5], [3, 4], [3]]
s, t = 1, 6

graph = [[2, 5], [1, 3], [2, 4], [3, 5], [1, 4]]
s, t = 1, 5
"""

graph = [[2], [1, 3], [2, 4], [2, 3]]
s, t = 1, 2

print(even_path(graph, s, t))



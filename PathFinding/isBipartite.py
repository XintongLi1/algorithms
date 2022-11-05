def isBipartite(graph: list[list[int]]) -> bool:
        n = len(graph)
        visited = [0 for i in range(n)]
        # 0: unvisited, 1: bipartition A, -1: bipartition B
        queue = []
        for i in range(n):
            if visited[i] == 0:
                queue.append(i)
                visited[i] = 1
            while queue != []:
                node = queue.pop(0)
                for j in graph[node]:
                    if visited[j] == 0:
                        queue.append(j)
                        visited[j] = -visited[node]
                    elif visited[j] == visited[node]:
                        return False
        return True

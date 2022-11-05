def findOrder(numCourses: int, prerequisites):
        finished = []
        is_finished = [0] * numCourses
        visited = [0] * numCourses
        graph = [[] for _ in range(numCourses)]
        for l in prerequisites:
            graph[l[1]].append(l[0])
            
        def dfs(v):
            visited[v] = 1
            flag = True
            for u in graph[v]:
                if not visited[u]:
                    if not dfs(u):
                        flag = False
                elif not is_finished[u]:  # back edge, which leads to cycle, is not permitted
                    return False
            finished.append(v)
            is_finished[v] = 1
            return flag
            
        flag = True
        for i in range(numCourses):
            if not visited[i]:
                if not dfs(i):
                    flag = False
        if not flag:
            return []
        finished.reverse()
        return finished


from queue import Queue

def findOrder2(numCourses: int, prerequisites):
    indegree = [0] * numCourses
    queue = Queue()
    res = []
    graph = [[] for _ in range(numCourses)]
    for l in prerequisites:
        graph[l[1]].append(l[0])
        indegree[l[0]] += 1
    
    for i in range(numCourses):
        if not indegree[i]:
            queue.put(i)
    
    while not queue.empty():
        v = queue.get()
        res.append(v)
        for u in graph[v]:
            indegree[u] -= 1
            if not indegree[u]:
                queue.put(u)
    
    for i in indegree:
        if i:
            return []
    return res

    
print(findOrder2(2, [[0, 1], [1, 0]]))
print(findOrder2(4, [[1,0],[2,0],[3,1],[3,2]]))

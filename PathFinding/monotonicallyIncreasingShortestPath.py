
int_max = 10**10
int_min = -10**10


class Node:
    def __init__(self, u, v, w):
        self._from = u  
        self.to = v
        self.weight = w
        self.next = None


def shortest_path(n, m, s, G):  # G: a list of tuple
    res = [(int_max, [])] * n # (total_weight, path)
    res[s] = (0, [])
    start = [ Node(None, None, None) for _ in range(n)] 
    from_edges = [start[i] for i in range(n)] 
    MD1 = [[(int_max, []) for _ in range(n)] for _ in range(n)]
    MD2 = [[(int_max, []) for _ in range(n)] for _ in range(n)]
    parity = True
    flag = 1
    for u, v, w in G:
        from_edges[v].next = Node(u, v, w)
        from_edges[v] = from_edges[v].next
        if u == s:
            MD1[u][v] = (w, [s])  


    while flag == 1:
        flag = 0
        if parity: MD, MDP = MD2, MD1
        else: MD, MDP = MD1, MD2
        for i in range(n):
            for j in range(n):
                MD[i][j] = MDP[i][j] 
        for u, v, w in G:
            print("=====================================")
            print("u =", u, " v =", v, " w =", w)
            node = start[u].next
            while node is not None:
                if node.weight < w and MDP[node._from][u][0] != int_max and w + MDP[node._from][u][0] < MD[u][v][0]:
                    MD[u][v] = (w + MDP[node._from][u][0], MDP[node._from][u][1] + [u])
                    flag = 1
                    print("node: ", node._from, node.to, node.weight)
                    print("MD[" + str(node._from) + "," + str(u) + "] =", MDP[node._from][u])
                    print("MD[" + str(u) + "," + str(v) + "] =", MD[u][v])
                node = node.next

            print("=====================================")
        parity = not parity
        for i in range(n):
            for j in range(n):
                print(MD[i][j], end=" ")
            print(" ")


    if parity: MD = MD1
    else: MD = MD2
    for v in range(n):
        node = start[v].next
        while node is not None:
            if MD[node._from][v][0] < res[v][0]:
                res[v] = MD[node._from][v]
            node = node.next
     
    return res


def main():
    n = 4
    m = 9
    s = 3
    G = [(2, 3, -2), (0, 2, -3), (0, 1, -6), (1, 2, 2), (3, 1, -7), (1, 0, -3), (1, 3, -5),  (3, 2, -9), (2, 1, -3)]
    res = shortest_path(n, m, s, G)
    for i, (w, p) in enumerate(res):
        if w == int_max :
            print("inf")
        elif i == s:
            print("src")
        else:
            print(str(w) + " : " + ' '.join(map(str, p)), i)


main() 

"""
    G = []
    n, m, s = [int(i) for i in input().split()]
    for _ in range(m):
        u, v, w = [int(i) for i in input().split()]
        G.append((u, v, w))
     
"""


# class Node:
#     def __init__(self, e, w, p = []):
#         self.e = e  # weight of the last edge of the path
#         self.w = w  # total weight of the path
#         self.p = p  # vertices in the path
#         self.next = None


# def shortest_path(n, m, s, G):  # G: a list of tuple
#     res = [(int_min, int_max, [])] * n # (last_weight, total_weight, path)
#     past = [ Node(int_max, int_max) for _ in range(n) ] 
#     res[s] = (int_min, 0, [])
#     past[s].next = Node(int_min, 0)
#     check = [[ past[u] for _ in range(n) ] for u in range(n) ]
#     past[s] = past[s].next
#     flag = 1
#     while flag == 1:
#         flag = 0
#         for u, v, w in G:
#             print("=====================================")
#             print("u =", u, " v =", v, " w =", w)
#             node = check[u][v].next
#             while node is not None:
#                 e, weights, p = node.e, node.w, node.p
#                 print("node: past[" + str(u) + "] = (", e, ",", weights, ",", p, ")")    
#                 if e < w:
#                     if w + weights < res[v][1]: 
#                         past[v].next = Node(w, w + weights, p + [u])
#                         past[v] = past[v].next
#                         res[v] = (w, w + weights, p + [u])
#                         flag = 1
#                         print("res[" + str(v) + "] =", res[v])

#                     elif w < res[v][0]:
#                         past[v].next = Node(w, w + weights, p + [u])
#                         past[v] = past[v].next
#                         flag = 1
#                         print("add: past[" + str(v) + "] = (", past[v].e, ",", past[v].w, ",", past[v].p, ")")
#                 node = node.next
#             check[u][v] = past[u]
#             print("=====================================")
            
#     return res


# def main():
#     n = 4
#     m = 6
#     s = 3
#     G = [(2, 3, -2), (0, 2, -3), (0, 1, -6), (1, 2, 2), (3, 1, -7), (1, 0, -3)]
#     res = shortest_path(n, m, s, G)
#     for i, (_, w, p) in enumerate(res):
#         if w == int_max :
#             print("inf")
#         elif i == s:
#             print("src")
#         else:
#             print(str(w) + " : " + ' '.join(map(str, p)), i)


# main() 






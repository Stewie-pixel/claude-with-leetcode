from collections import deque

class Solution(object):
    def assignEdgeWeights(self, edges, queries):
        MOD = 10**9 + 7
        n = len(edges) + 1
        
        adj = [[] for _ in range(n + 1)]
        for u, v in edges:
            adj[u].append(v)
            adj[v].append(u)
            
        depth = [-1] * (n + 1)
        LOG = 18
        up = [[0] * LOG for _ in range(n + 1)]
        
        queue = deque([1])
        depth[1] = 0
        
        while queue:
            curr = queue.popleft()
            for neighbor in adj[curr]:
                if depth[neighbor] == -1:
                    depth[neighbor] = depth[curr] + 1
                    up[neighbor][0] = curr
                    queue.append(neighbor)
                    
        for j in range(1, LOG):
            for i in range(1, n + 1):
                up[i][j] = up[up[i][j-1]][j-1]
                
        def get_lca(u, v):
            if depth[u] < depth[v]:
                u, v = v, u
                
            diff = depth[u] - depth[v]
            for j in range(LOG):
                if (diff >> j) & 1:
                    u = up[u][j]
                    
            if u == v:
                return u
                
            for j in range(LOG - 1, -1, -1):
                if up[u][j] != up[v][j]:
                    u = up[u][j]
                    v = up[v][j]
                    
            return up[u][0]
            
        pow2 = [1] * (n + 1)
        for i in range(1, n + 1):
            pow2[i] = (pow2[i-1] * 2) % MOD
            
        answer = []
        for u, v in queries:
            if u == v:
                answer.append(0)
            else:
                lca = get_lca(u, v)
                L = depth[u] + depth[v] - 2 * depth[lca]
                answer.append(pow2[L - 1])
                
        return answer

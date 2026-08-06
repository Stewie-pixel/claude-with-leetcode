class Solution(object):
    def catMouseGame(self, graph):
        """
        :type graph: List[List[int]]
        :rtype: int
        """
        n = len(graph)
        result = [[[0] * 2 for _ in range(n)] for _ in range(n)]
        degree = [[[0] * 2 for _ in range(n)] for _ in range(n)]

        for m in range(n):
            for c in range(n):
                degree[m][c][0] = len(graph[m])
                degree[m][c][1] = len([x for x in graph[c] if x != 0])

        from collections import deque
        q = deque()

        for c in range(n):
            for turn in range(2):
                result[0][c][turn] = 1
                q.append((0, c, turn, 1))

        for m in range(n):
            for turn in range(2):
                result[m][m][turn] = 2
                q.append((m, m, turn, 2))

        while q:
            m, c, turn, res = q.popleft()

            if turn == 0:
                prev_turn = 1
                for prev_c in graph[c]:
                    if prev_c == 0: 
                        continue
                    if result[m][prev_c][prev_turn] == 0:
                        if res == 2:
                            result[m][prev_c][prev_turn] = 2
                            q.append((m, prev_c, prev_turn, 2))
                        else:
                            degree[m][prev_c][prev_turn] -= 1
                            if degree[m][prev_c][prev_turn] == 0:
                                result[m][prev_c][prev_turn] = 1
                                q.append((m, prev_c, prev_turn, 1))
            else:
                prev_turn = 0
                for prev_m in graph[m]:
                    if result[prev_m][c][prev_turn] == 0:
                        if res == 1:
                            result[prev_m][c][prev_turn] = 1
                            q.append((prev_m, c, prev_turn, 1))
                        else:
                            degree[prev_m][c][prev_turn] -= 1
                            if degree[prev_m][c][prev_turn] == 0:
                                result[prev_m][c][prev_turn] = 2
                                q.append((prev_m, c, prev_turn, 2))

        return result[1][2][0]
import operator
from types import *

class path_node_removal(object):
    """docstring for path_node_removal."""
    def __init__(self):
        super(path_node_removal, self).__init__()
        self.order = []

    def get_most_frequent_el(self, A):
        src, dest = A[0][0], A[0][-1]
        node_counts = {}
        path_counts = {}
        # print "lenA on start: {}".format(len(A))
        # print A
        for i in range(len(A)):
            for j in range(len(A[i]) - 1):
                tmp = (A[i][j], A[i][j + 1])
                if tmp not in path_counts:
                    path_counts[tmp] = 1
                else:
                    path_counts[tmp] += 1

        for lst in A:
            for node in lst:
                if node not in node_counts:
                    node_counts[node] = 1
                else:
                    node_counts[node] += 1

        del node_counts[src]
        del node_counts[dest]


        max_node = max(node_counts.iteritems(), key = operator.itemgetter(1))
        max_path = max(path_counts.iteritems(), key = operator.itemgetter(1))

        max_nodes = []
        max_paths = []
        # max_nodes.append(max_node)
        # max_paths.append(max_path)
        for k, v in node_counts.iteritems():
            if v == max_node[1]:
                max_nodes.append(k)

        for k, v in path_counts.iteritems():
            if v == max_path[1]:
                max_paths.append(k)

        # node_count = max_node[1]
        # path_count = max_path[1]
        return max_nodes, max_node[1], max_paths, max_path[1]

    def remove_node(self, A, rm_node):
        i = 0
        lenA = len(A)
        removed_paths = []
        while i < lenA:
            if rm_node in A[i]:
                removed_paths.append(A[i])
                del A[i]
                lenA -= 1
            else:
                i += 1
        return removed_paths

    def remove_path(self, A, rm_path):
        i = 0
        lenA = len(A)
        removed_paths = []
        while i < lenA:
            lst = A[i]
            found = False
            for j in range(len(lst) - 1):
                if lst[j] == rm_path[0] and lst[j + 1] == rm_path[1]:
                    if i < lenA:
                        found = True
                        removed_paths.append(A[i])
                        del A[i]
                        lenA -= 1
                    break
            if not found:
                i += 1

        return removed_paths


    def removal(self, A, removals = []):
        # count how many times a node appears in the ksp
        # node_counts = {}
        # path_counts = {}
        # print "lenA on start: {}".format(len(A))
        # print A
        if len(A) == 0:
            self.order.append(list(removals))
            return

        max_nodes, node_count, max_paths, path_count = self.get_most_frequent_el(A)

        # consider both
        max_candidates = []
        if node_count >= path_count:
            max_candidates.extend(max_nodes)

        if node_count <= path_count:
            max_candidates.extend(max_paths)


        for candidate in max_candidates:
            removals.append(candidate)
            if type(candidate) == IntType:
                removed_paths = self.remove_node(A, candidate)
            else:
                removed_paths = self.remove_path(A, candidate)
            self.removal(A, removals)
            A.extend(removed_paths)
            del removals[-1]


        # if max_node[1] > max_path[1]:
            # rm_node = max_node[0]
            # # print "node: {}".format(rm_node)
            # self.order.append(rm_node)
            #
            # i = 0
            # lenA = len(A)
            # while i < lenA:
            #     if rm_node in A[i]:
            #         del A[i]
            #         lenA -= 1
            #     else:
            #         i += 1
        # else:
            # rm_path = max_path[0]
            # # print rm_path
            # self.order.append(rm_path)
            #
            # i = 0
            # lenA = len(A)
            # while i < lenA:
            #     lst = A[i]
            #     found = False
            #     for j in range(len(lst) - 1):
            #         if lst[j] == rm_path[0] and lst[j + 1] == rm_path[1]:
            #             if i < lenA:
            #                 found = True
            #                 del A[i]
            #                 lenA -= 1
            #             break
            #     if not found:
            #         i += 1

        # print "lenA on end: {}".format(len(A))
        return

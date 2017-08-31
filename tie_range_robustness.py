# code adapted from gist: https://gist.github.com/ALenfant/5491853
from igraph import *
import Queue

class k_shortest(object):
    """docstring for k_shortest."""
    def __init__(self, url):
        super(k_shortest, self).__init__()
        self.g = Graph.Read_GraphML(url)

    def path_cost(self, path, weights=None):

        pathcost = 0
        if weights is None:
            pathcost = len(path)-1
        else:
            for i in range(len(path)):
                if i > 0:
                    edge = self.g.es.find(_source=min(path[i-1], path[i]),
                                         _target=max(path[i-1], path[i]))
                    pathcost += edge[weights]

        return pathcost


    def in_lists(self, list1, list2):

        result = False
        node_result = -1

        if len(list1) < len(list2):
            toIter = list1
            toRefer = list2
        else:
            toIter = list2
            toRefer = list1

        for element in toIter:
            result = element in toRefer
            if result:
                node_result = element
                break

        return result, node_result


    def yen_algo(self, source, target, num_k, weights=None):


        #Shortest path from the source to the target
        A = [self.g.get_shortest_paths(source,
                                      to=target,
                                      weights=weights,
                                      output="vpath")[0]]
        A_costs = [self.path_cost(A[0], weights)]

        #Initialize the heap to store the potential kth shortest path
        B = Queue.PriorityQueue()

        for k in range(1, num_k):
            # The spur node ranges from the first node to the next to last node in
            # the shortest path
            for i in range(len(A[k-1])-1):
                #Spur node is retrieved from the previous k-shortest path, k - 1
                spurNode = A[k-1][i]
                # The sequence of nodes from the source to the spur node of the
                # previous k-shortest path
                rootPath = A[k-1][:i]

                #We store the removed edges
                removed_edges = []

                for path in A:
                    if len(path) - 1 > i and rootPath == path[:i]:
                        # Remove the links that are part of the previous shortest
                        # paths which share the same root path
                        edge = self.g.es.select(_source=min(path[i], path[i+1]),
                                               _target=max(path[i], path[i+1]))
                        if len(edge) == 0:
                            continue
                        edge = edge[0]
                        removed_edges.append((path[i],
                                         path[i+1],
                                         edge.attributes()))
                        edge.delete()

                #Calculate the spur path from the spur node to the sink
                while True:
                    spurPath = self.g.get_shortest_paths(spurNode,
                                                    to=target,
                                                    weights=weights,
                                                    output="vpath")[0]
                    [is_loop, loop_element] = self.in_lists(spurPath, rootPath)

                    if not is_loop:
                        break
                    else:
                        loop_index = spurPath.index(loop_element)
                        edge = self.g.es.select(_source=min(spurPath[loop_index],
                                                           spurPath[loop_index-1]),
                                               _target=max(spurPath[loop_index],
                                                           spurPath[loop_index-1]))

                        if len(edge) == 0:
                            continue

                        edge = edge[0]
                        removed_edges.append((spurPath[loop_index],
                                             spurPath[loop_index-1],
                                             edge.attributes()))
                        edge.delete()

                #Add back the edges that were removed from the self.g
                for removed_edge in removed_edges:
                    node_start, node_end, cost = removed_edge
                    self.g.add_edge(node_start, node_end)
                    edge = self.g.es.select(_source=min(node_start, node_end),
                                       _target=max(node_start, node_end))[0]
                    edge.update_attributes(cost)

                if len(spurPath) > 0:
                    #Entire path is made up of the root path and spur path
                    totalPath = rootPath + spurPath
                    totalPathCost = self.path_cost(totalPath, weights)
                    #Add the potential k-shortest path to the heap
                    B.put((totalPathCost, totalPath))

            #Sort the potential k-shortest paths by cost
            #B is already sorted
            #Add the lowest cost path becomes the k-shortest path.
            while True:
                if B.qsize() == 0:
                    break
                cost_, path_ = B.get()
                if path_ not in A:
                    #We found a new path to add
                    A.append(path_)
                    A_costs.append(cost_)
                    break

            if not len(A) > k:
                break

        return A, A_costs

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

        import operator
        max_node = max(node_counts.iteritems(), key = operator.itemgetter(1))
        max_path = max(path_counts.iteritems(), key = operator.itemgetter(1))

        max_nodes = []
        max_paths = []
        # max_nodes.append(max_node)
        # max_paths.append(max_path)
        for k, v in node_counts:
            if v == max_node[1]:
                max_nodes.append(k)

        for k, v in path_counts:
            if v == max_path[1]:
                max_paths.append(k)

        # node_count = max_node[1]
        # path_count = max_path[1]
        return max_nodes, max_node[1], max_paths, max_path[1]

    self.paths_removed = []


    def removal(self, A):
        # count how many times a node appears in the ksp
        # node_counts = {}
        # path_counts = {}
        # print "lenA on start: {}".format(len(A))
        # print A

        max_nodes, node_count, max_paths, path_count = self.get_most_frequent_el(A)

        # consider both
        # if node_count == path_count:
        #     removed = []
        #     for node in max_nodes:
        #

        max_node = max_nodes[0]
        max_path = max_paths[0]
        if max_node[1] > max_path[1]:
            rm_node = max_node[0]
            # print "node: {}".format(rm_node)
            self.order.append(rm_node)

            i = 0
            lenA = len(A)
            while i < lenA:
                if rm_node in A[i]:
                    del A[i]
                    lenA -= 1
                else:
                    i += 1
        else:
            rm_path = max_path[0]
            # print rm_path
            self.order.append(rm_path)

            i = 0
            lenA = len(A)
            while i < lenA:
                lst = A[i]
                found = False
                for j in range(len(lst) - 1):
                    if lst[j] == rm_path[0] and lst[j + 1] == rm_path[1]:
                        if i < lenA:
                            found = True
                            del A[i]
                            lenA -= 1
                        break
                if not found:
                    i += 1

        # print "lenA on end: {}".format(len(A))
        if len(A) > 0:
            return self.removal(A)
        return

ks = k_shortest("data/simmons81.graphml")

A, A_costs = ks.yen_algo(4, 8, 10)

rm = path_node_removal()
rm.removal(A)

print rm.order

# for i in range(len(A)):
#     print "[path]: "
#     print A[i]
#     print "\n[length]"
#     print A_costs[i]

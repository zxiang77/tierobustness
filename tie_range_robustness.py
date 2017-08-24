from igraph import *

class igraph_node(object):
    """docstring for igraph_node."""
    def __init__(self, vert, k):
        super(igraph_node, self).__init__()
        self.vert = vert
        self.k = k

    def __lt__(self, other):
        return self.vert['k'] < other['k']

    def __gt__(self, other):
        return self.vert['k'] > other['k']

    def __eq__(self, other):
        return self.vert['k'] == other['k']

url = "data/simmons81.graphml"
g = Graph.Read_GraphML(url)
summary(g)
# g['betweenness'] = g.edge_betweenness()
print g.vs['student_fac'][-1]

print g.get_shortest_paths(5, 5)
print g.st_mincut(5, 6, capacity=None)
"""
    param: g graph
    param: s source
    param: t dest
"""

# def k_shortest(g, s, t):

# st_mincut(source, target, capacity=None)

# shortest_paths_dijkstra(source=None, target=None, weights=None, mode=OUT)

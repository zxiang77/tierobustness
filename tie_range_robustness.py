# code adapted from gist: https://gist.github.com/ALenfant/5491853
from igraph import *
from k_shortest import *
from path_node_removal import *

def main():

    ks = k_shortest("data/simmons81.graphml")

    A, A_costs = ks.yen_algo(5, 12, 21)

    rm = path_node_removal()
    rm.removal(A)

    print "input A: {}".format(A)

    i = 0
    min_len = 10000
    possible_len = set()
    for path in rm.order:
        min_len = min(min_len, len(path))
        possible_len.add(len(path))
        # print "path {}: {}".format(i, path)
        i += 1

    print "1sp: {}, num paths: {}, min_len: {}, all_len: {}".format(len(A[0]), i, min_len, possible_len)

if __name__ == "__main__":
    main()

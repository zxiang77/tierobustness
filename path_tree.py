import YenKSP

class path_tree(object):
    """docstring for path_tree."""
    def __init__(self, arg):
        super(path_tree, self).__init__()
        self.arg = arg

class path_tree_node(object):
    """docstring for path_tree."""
    def __init__(self, path, parent):
        super(path_tree, self).__init__()
        self.path = path
        self.parent = parent
        self.children = []

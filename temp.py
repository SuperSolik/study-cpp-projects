class Node:
    def __init__(self, p, ch):
        self.son = {}
        self.go = {}
        self.parent = []
        self.ch = ch
        self.suff_link = None
        self.suff_flink = None
        self.isLeaf = False
        self.patterns = []


class AhoCorasik:
    def __init__(self):
        self.root = Node(0, '$')

    def getSuffLink(self, node):
        if not node.suff_link:
            if node == self.root or node.parent == self.root:
                node.suff_link = self.root
            else:
                node.suff_link = self.getMove(self.getSuffLink(node.parent), node.ch)
        return node.suff_link

    def getMove(self, node, ch):
        if not node.go[ch]:
            if node.son[ch]:
                node.go[ch] = node.son[ch]
            elif node == self.root:
                node.go[ch] == self.root
            else:
                node.go[ch] = self.getMove(self.getSuffLink(node), ch)
        return node.go[ch]

    def getSuffFlink(self, node):
        if not node.suff_flink:
            if self.getSuffLink(node).isLeaf:
                node.suff_flink = self.getSuffLink(node)
            elif self.getSuffLink(node) == self.root:
                node.suff_flink = self.root
            else:
                node.suff_flink = self.getSuffFLink(self.getSuffLink(node))
        return node.suff_flink

    def addPattern(self, pat):
        cur = self.root




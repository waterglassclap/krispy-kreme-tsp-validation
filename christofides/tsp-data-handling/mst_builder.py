""" build mst using scipy lib and tsp datas from : http://comopt.ifi.uni-heidelberg.de

    References:
    http://docs.scipy.org/doc/scipy-0.15.1/reference/generated/scipy.sparse.csgraph.minimum_spanning_tree.html
    http://stackoverflow.com/questions/31139580/mst-clustering-using-python

"""
import os
from tsp_fetcher import TSPFetcher
from scipy.sparse import csr_matrix
from scipy.sparse.csgraph import minimum_spanning_tree


def makedir_safe(path):
    """ if dir not exists, make dir. """
    if not os.path.exists(path):
        os.makedirs(path)

class MSTBuilder(object):
    """ mst builder for build mst using fully-connected graph infos """
    def __init__(self):
        self.outdir_root = os.path.abspath('../../datas')
        self.outdir_tsp_full = os.path.join(self.outdir_root, 'tsp-full')
        self.outdir_tsp_mst = os.path.join(self.outdir_root, 'tsp-mst')
 
        makedir_safe(self.outdir_root)
        makedir_safe(self.outdir_tsp_full)
        makedir_safe(self.outdir_tsp_mst)


    def build_all(self):
        """ build all mst. """
        tsp_configs = TSPFetcher().list()
        for config in tsp_configs:
            self.build(config)


    def build(self, config):
        """ build target mst from given config """
        fully_matrix = self._get_fully_matrix(config)
        self._build_mst(config, fully_matrix)


    def _get_fully_matrix(self, config):
        fully_file_name = os.path.join(self.outdir_tsp_full, config["name"] + ".txt")

        with open(fully_file_name, "r") as infile:
            line = infile.readline()
            node_count, _ = tuple([int(x) for x in line.split(' ')])
            fully_matrix = [[0 for i in range(node_count)] for j in range(node_count)]

            while line:
                line = infile.readline()
                if line:
                    start, end, weight = tuple([int(x) for x in line.split(' ')])
                    fully_matrix[start][end] = weight

        return fully_matrix


    def _build_mst(self, config, fully_matrix):
        mst_file_name = os.path.join(self.outdir_tsp_mst, config["name"] + ".txt")
        with open(mst_file_name, "w") as outfile:
            # header : node_count edge_count
            outfile.write("%d %d" % (config["node_count"], config["node_count"] - 1))
            # build mst
            tcsr_arr = minimum_spanning_tree(csr_matrix(fully_matrix)).toarray().astype(int)
            # contents : start_node end_node weight
            for row_idx, row in enumerate(tcsr_arr):
                for col_idx, item in enumerate(row):
                    if item:
                        outfile.write("\n%d %d %d" % (row_idx, col_idx, item))


def sample():
    """ sample for MSTBuilder """
    configs = TSPFetcher().list()
    builder = MSTBuilder()
    #builder.build(configs[0])

    for i in range(0, len(configs)):
        try:
            print i
            print configs[i]
            builder.build(configs[i])
        except Exception:
            print "no file for " + str(i) + ". proceed."
    #builder.build_all()


if __name__ == "__main__":
    sample()

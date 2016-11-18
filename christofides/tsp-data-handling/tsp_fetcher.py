""" fetch tsp datas from : http://comopt.ifi.uni-heidelberg.de """
from StringIO import StringIO
from xml.etree import ElementTree
import re
import os
import json
import zipfile
import requests
import pycurl

def simply_curl(url, buf):
    """ simply curl from given url. """
    c = pycurl.Curl()
    c.setopt(c.URL, url)
    c.setopt(c.WRITEDATA, buf)
    c.perform()
    c.close()


def makedir_safe(path):
    """ if dir not exists, make dir. """
    if not os.path.exists(path):
        os.makedirs(path)


class TSPFetcher(object):
    """ tsp fetcher which fetch tsp datas from : http://comopt.ifi.uni-heidelberg.de """
    def __init__(self):
        self.root_url = "http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95"
        self.list_url = self.root_url + "/STSP.html"
        self.tsp_url = self.root_url + "/XML-TSPLIB/instances/%s.xml.zip"      

        self.list_item_pattern = re.compile(r"<big>([a-zA-Z\-\_]*)([0-9]*) : (\d+)</big></li>")

        self.outdir_root = os.path.abspath('../../datas')
        self.outdir_raws = os.path.join(self.outdir_root, 'raws')
        self.outdir_tsp_full = os.path.join(self.outdir_root, 'tsp-full')
 
        makedir_safe(self.outdir_root)
        makedir_safe(self.outdir_raws)
        makedir_safe(self.outdir_tsp_full)


    def list(self, **kwargs):
        """ list all stsp configs which contains name and optimal cost. """
        buf = StringIO()
        simply_curl(self.list_url, buf)
        raw_lists = self.list_item_pattern.findall(buf.getvalue())
        configs = [{"name" : item[0] + item[1], "node_count" : int(item[1]), "optimal" : item[2]}
                   for item in raw_lists]

        if "file" in kwargs and kwargs["file"]:
            outfile_path = os.path.join(self.outdir_root, "list.json")
            with open(outfile_path, "w") as outfile:
                outfile.write(json.dumps(configs))
        else:
            return configs


    def fetch_all(self):
        """ fetch all stsp datas. """
        configs = self.list()
        for config in configs:
            self.fetch(config)


    def fetch(self, config):
        """ fetch target stsp datas from given config """
        # Download xml data file from remote url
        req = requests.get(self.tsp_url % config["name"], stream=True)
        zpfile = zipfile.ZipFile(StringIO(req.content))
        zpfile.extractall(self.outdir_raws)

        # parse xml file and save it as blossomV input form, which has integer weight
        rawfile_name = os.path.join(self.outdir_raws, config["name"] + ".xml")
        doc = ElementTree.parse(rawfile_name)
        graph = doc.getroot().find("graph")

        outfile_path = os.path.join(self.outdir_tsp_full, config["name"] + ".txt")
        with open(outfile_path, "w") as outfile:
            edge_count = config["node_count"] * (config["node_count"] - 1)
            # header : node_count edge_count
            outfile.write("%d %d" % (config["node_count"], edge_count))
            # contents : start_node end_node weight
            for idx, vertex in enumerate(graph.iter("vertex")):
                for edge in vertex.iter("edge"):
                    outfile.write("\n%d %d %d" %
                                  (idx, int(edge.text), int(float(edge.attrib["cost"]))))


def sample():
    """ sample for TSPFetcher """
    fetcher = TSPFetcher()
    configs = fetcher.list()
    # except brd14051 : 9
    # except d15112 : 20
    # except fnl4461 : 29
    # except linhp318 : 55 - no zip
    # except pla7397 : 62
    # except pla33810 : 63
    # except pla85900 : 64
    # except rl5915 : 85
    # except rl5934 : 86
    # except rl11849 : 87
    # except usa13509 : 105
    for i in range(106, len(configs)):
        print i
        print configs[i]
        fetcher.fetch(configs[i])
    #fecther.fetch_all()


if __name__ == "__main__":
    sample()

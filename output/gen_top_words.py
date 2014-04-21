#!/usr/bin/python2
#-*- coding: utf-8 -*-

import sys, collections

if __name__ == "__main__":
    phi_file = sys.argv[1]
    codewords_file = sys.argv[2]

    codewords = collections.defaultdict(int)
    with open(codewords_file) as fp:
        for line in fp:
            codewords[int(line.strip().split(":")[1])] = line.split(":")[0]

    matrix = []
    with open(phi_file) as fp:
        for line in fp:
            matrix.append(sorted([(float(value), index) \
                    for index, value in enumerate(line.split())], reverse = True))

    topk = 10
    for line in matrix:
        print " ".join([codewords[item[1]] for item in line[:topk]])

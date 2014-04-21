#!/usr/bin/python2
#-*- coding: utf-8 -*-

import os, sys, collections

def load_stopwords(stopwords_file):
    with open(stopwords_file) as fp:
        stopwords = [line.strip() for line in fp]
    return stopwords

def load_documents(document_dir, stopwords):
    documents = []
    for root, dirs, files in os.walk(document_dir):
        for filename in files:
            with open(root + "/" + filename) as fp:
                documents.append(map(lambda x: x.lower(), \
                        filter(lambda x: x.lower() not in stopwords \
                        and len(x) > 1, \
                        "".join(map(lambda x: x if x.isalpha() else " ", \
                        " ".join([line.strip() for line in fp]))).split())))
    return documents

def gen_codewords(documents):
    codewords = collections.defaultdict(int)
    for document in documents:
        for word in document:
            codewords[word] = 1
    for index, word in enumerate(codewords):
        codewords[word] = index
    return codewords

def gen_code_documents(documents, codewords):
    codedocuments = []
    for document in documents:
        codedoc = collections.defaultdict(int)
        for word in document:
            codedoc[codewords[word]] += 1
        codedocuments.append(codedoc)
    return codedocuments

def output_codewords(output_dir, codewords):
    with open(output_dir + "/codewords.txt", "w") as fp:
        fp.write("\n".join(["%s:%d" % (item[0], item[1]) \
                for item in codewords.items()]))

def output_codedocuments(output_dir, codedocuments):
    with open(output_dir + "/codedocuments.txt", "w") as fp:
        for document in codedocuments:
            fp.write("%d %s\n" % (len(document), \
                    " ".join(["%d:%d" % (item[0], item[1]) \
                    for item in document.items()])))

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print "usage: python gen_input.py <document dir> <stopwords file> \
                <output dir>"
        exit(0)

    document_dir = sys.argv[1]
    stopwords_file = sys.argv[2]
    output_dir = sys.argv[3]

    stopwords = load_stopwords(stopwords_file)
    documents = load_documents(document_dir, stopwords)
    codewords = gen_codewords(documents)
    codedocuments = gen_code_documents(documents, codewords)

    output_codewords(output_dir, codewords)
    output_codedocuments(output_dir, codedocuments)

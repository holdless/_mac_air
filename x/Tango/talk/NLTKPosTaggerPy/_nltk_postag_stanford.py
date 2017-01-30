import os
from nltk.tag import StanfordPOSTagger
CLASSPATH = '/Users/hiroshi/Downloads/stanford-postagger-2014-08-27/'
STANFORD_MODELS = '/Users/hiroshi/Downloads/stanford-postagger-2014-08-27/models/'

st = StanfordPOSTagger('english-left3words-distsim.tagger')
#print parser.raw_batch_parse(("Hello, My name is Melroy.", "What is your name?"))

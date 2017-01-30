import nltk
from nltk import pos_tag

def findtags(tag, TAG):
    n = len(tag)
    for i in range(0,n-1):
        if TAG == tag[i][1]:
            return i
     #   else:
#            return -1
        

def loadTagger():
    _POS_TAGGER = 'taggers/maxent_treebank_pos_tagger/english.pickle'
#    _POS_TAGGER = 'taggers/averaged_perceptron_tagger/averaged_perceptron_tagger.pickle'
#    _POS_TAGGER = 'taggers/hmm_treebank_pos_tagger/treebank.tagger.pickle'
    
    tagger = nltk.load(_POS_TAGGER)
    return tagger

tagger = loadTagger()
text = nltk.word_tokenize("turn on the light")
print text
tagged_text = tagger.tag(text)
print tagged_text
tagged_text = pos_tag(text)
print tagged_text
#tag = nltk.pos_tag(text)
#n = len(tag)

#print tag

#p = findtags(tag, 'WP')
#print p
#print text[p]


    

import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.positives = []
        self.negatives = []
        with open(positives, "r") as lines:
            for line in lines:
                l = line.strip()
                if not l.startswith(";") and l:
                    self.positives.append(l)
        self.positives = set(self.positives)
        
        with open(negatives, "r") as lines:
            for line in lines:
                l = line.strip()
                if not l.startswith(";") and l:
                    self.negatives.append(l)
        self.negatives = set(self.negatives)
        self.tokenizer = nltk.tokenize.TweetTokenizer()

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        tokens = self.tokenizer.tokenize(text)
        tokens = list(map(lambda tok: tok.lower(), tokens))
        result = []
        for token in tokens:
            if token in self.positives:
                result.append(1.0)
            elif token in self.negatives:
                result.append(-1.0)
            else:
                result.append(0.0)
        
        return sum(result)
        


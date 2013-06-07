// exports.native = require ('./build/Release/fuzzy');

var fuzzy = require ('./build/Release/fuzzy');

var add = exports.add = function (word) {
	if (typeof word === 'string' && word.length > 0) {
		fuzzy.add (word);
	}
};

var dice = exports.dice = function (word1, word2) {
	if (typeof word1 === 'string' && typeof word2 === 'string') {
		if (word1.length == 0 && word2.length == 0) return 0;
		return fuzzy.dice (word1, word2);
	}
}

var levenshtein = exports.levenshtein = function (word1, word2) {
	if (typeof word1 === 'string' && typeof word2 === 'string' && word1.length > 0 && word2.length > 0) {
		return fuzzy.levenshtein (word1, word2);
	}
}

var query = exports.query = function (word, addons) {
	var words = fuzzy.query (word) || [word].concat (new Array (addons.length));
	if (arguments.length === 1) return words;
	var addons = Array.prototype.slice.call (arguments, 1);
	var ret = new Array (words.length);
	for (var i=0; i<words.length; i++) {
		var result = new Array (addons.length+1);
		result[0] = words[i];
		for (var j=0; j<addons.length; j++) {
			result[j+1] = addons[j] (word, words[i]);
		}
		ret[i] = result;
	}
	return ret;
};
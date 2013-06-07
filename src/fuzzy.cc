#include <node.h>
#include <v8.h>
#include <string>
#include <map>
#include <vector>
#include <set>

using namespace std;
using namespace v8;

multimap <string, string> dictionary;

set<string> ngrams (string word, unsigned int n = 2) {
	set<string> result;
	if (word.length() <= n) {
		result.insert (word);
	} else {
		for (unsigned int i = 0; i<(word.length()-(n-1)); i++) {
			result.insert (word.substr(i, n));
		}
	}
	return result;
}

set<string> query_impl (string word) {
	set<string> result;
	set<string> grams = ngrams (word);
	for (set<string>::iterator iter = grams.begin(); iter != grams.end(); iter++) {
		pair<multimap<string,string>::iterator, multimap<string,string>::iterator> query = dictionary.equal_range (*iter);
		if (query.first != query.second) {
			for (multimap<string,string>::iterator jter = query.first; jter != query.second; ++jter) {
				result.insert (jter->second);
			}
		}
	}
	return result;
}

// http://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Dice's_coefficient#C.2B.2B
float dice_impl(string string1, string string2)
{
 
        set<string> string1_bigrams = ngrams (string2);
        set<string> string2_bigrams = ngrams (string1);
 
        int intersection = 0;
 
        // find the intersection between the two sets
        for(set<string>::iterator IT = string2_bigrams.begin(); 
            IT != string2_bigrams.end(); 
            IT++)
        {    
                intersection += string1_bigrams.count((*IT));
        }
 
        // calculate dice coefficient
        int total = string1_bigrams.size() + string2_bigrams.size();
        float dice = (float)(intersection * 2) / (float)total;
 
        return dice;
}

// http://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C.2B.2B
template<class T>
unsigned int levenshtein_impl(const T &s1, const T & s2) {
        const size_t len1 = s1.size(), len2 = s2.size();
        vector<unsigned int> col(len2+1), prevCol(len2+1);
 
        for (unsigned int i = 0; i < prevCol.size(); i++)
                prevCol[i] = i;
        for (unsigned int i = 0; i < len1; i++) {
                col[0] = i+1;
                for (unsigned int j = 0; j < len2; j++)
                        col[j+1] = min( min( 1 + col[j], 1 + prevCol[1 + j]),
                                                                prevCol[j] + (s1[i]==s2[j] ? 0 : 1) );
                col.swap(prevCol);
        }
        return prevCol[len2];
}

Handle<Value> Add (const Arguments& args) {
	HandleScope scope;
	String::AsciiValue param1 (args[0]->ToString());
	set<string> grams = ngrams (*param1);
	for (set<string>::iterator iter = grams.begin(); iter != grams.end(); iter++) {
		dictionary.insert (multimap<string, string>::value_type (*iter, *param1));
	}
	return scope.Close (Undefined());
}

Handle<Value> Query (const Arguments& args) {
	HandleScope scope;
	try {
		String::AsciiValue param1 (args[0]->ToString());
		set<string> query = query_impl (*param1);
		if (!query.empty()) {
			Handle<Array> result = Array::New (-1);
			unsigned int i = 0;
			for (set<string>::iterator iter = query.begin(); iter != query.end(); iter++) {
				if ((float)dice_impl(*iter, string(*param1)) > 0.7) {
					result->Set (i++, String::New (iter->data(), iter->size()));
				}
			}
			return scope.Close (result);
		} else {
			return scope.Close (Undefined());
		}
	} catch (...) {
		ThrowException(Exception::Error (String::New("Undefined exception, m'kay.")));
		scope.Close (Undefined());
	}
}

Handle<Value> Dice (const Arguments& args) {
	HandleScope scope;
	String::AsciiValue param1 (args[0]->ToString());
	String::AsciiValue param2 (args[1]->ToString());
	Handle<Number> result = Number::New(dice_impl (string(*param1), string(*param2)));
	return scope.Close (result);
}

Handle<Value> Levenshtein (const Arguments& args) {
	HandleScope scope;
	String::AsciiValue param1 (args[0]->ToString());
	String::AsciiValue param2 (args[1]->ToString());
	Handle<Number> result = Number::New(levenshtein_impl<string> (string(*param1), string(*param2)));
	return scope.Close (result);
}

void init (Handle<Object> exports) {
	exports->Set (String::NewSymbol ("add"), FunctionTemplate::New (Add)->GetFunction ());
	exports->Set (String::NewSymbol ("query"), FunctionTemplate::New (Query)->GetFunction ());
	exports->Set (String::NewSymbol ("dice"), FunctionTemplate::New (Dice)->GetFunction());
	exports->Set (String::NewSymbol ("levenshtein"), FunctionTemplate::New (Levenshtein)->GetFunction());
}

NODE_MODULE(fuzzy, init);
#define JDEBUG

#include<string>
#include<cstring>
#include<iostream>
#include<set>
using namespace std;

set<string> permutation;
int len;
string substring[8];
string fullstr;
bool visited[8];

void recur(int cur) {
	if (cur == len) {
		// compelete a full string consist of len substrings
		permutation.insert(fullstr);
	} else {
		for (int i = 0; i < len; ++i) {
			if (!visited[i]) {
				int lastEnd = fullstr.size();
				
				// add another substring
				fullstr += substring[i];
				visited[i] = true;
				
				recur(cur + 1);
				
				// remove the substring added in this level
				int curEnd = fullstr.size();
				fullstr.erase(lastEnd, curEnd);
				visited[i] = false;
			}
		}
	}
}

int main(void) {
#ifdef JDEBUG
    freopen("1198.in", "r", stdin);
    freopen("1198.out", "w", stdout);
#endif
	int t;
	
	cin >> t;
	while(t--) {
		cin >> len;
		
		// initialization
		fullstr.clear();
		permutation.clear();
		memset(visited, false, sizeof(visited));
		
		for (int i = 0; i < len; ++i) {
			cin >> substring[i];
		}
		
		recur(0);
	 
	 	// use the lexicographically smallest full string
		cout << *(permutation.begin()) << '\n';
	}
	return 0;
}
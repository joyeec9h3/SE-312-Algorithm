//#define JDEBUG
//#define TRACK

#include<iostream>
#include<cstdio>
#include<string>
#include<algorithm>
using namespace std;

bool cmp(string a, string b) {
	return a.size() > b.size();
}

bool match(string a, string b) {
	int len = a.size();
	for (int i = 0; i < len; ++i) {
		if (a[i] == 'A' && b[i] != 'T')
			return false;
		if (a[i] == 'T' && b[i] != 'A')
			return false;
		if (a[i] == 'C' && b[i] != 'G')
			return false;
		if (a[i] == 'G' && b[i] != 'C')
			return false;
	}
	return true; 
}


int main(void) {
#ifdef JDEBUG
	freopen("1035.in", "r", stdin);
	freopen("1035.out", "w", stdout);
#endif
	int t, n;
	string str[101];
	int len[101];
	bool used[101];

	// for each test case: (t<=20)
  	cin >> t;
	while(t--) {
		// scan and store n strings(n <= 100)
		cin >> n;
		for (int i = 0; i < n; ++i) {
			cin >> str[i];
		}
		
		// sort by length
		sort(str, str + n, cmp);

		for (int i = 0; i < n; ++i) {
			// store length
			len[i] = str[i].size();
			// init used
			used[i] = false;
		}
		
		int count = 0;
		
		for (int i = 0; i < n; ++i) {
			if (!used[i]) {
				for (int j = i + 1; j < n && len[j] == len[i]; ++j) {
					if (match(str[i], str[j])) {
						count++;
						used[i] = used[j] = true;
						break;
					}
				}
			}
		}
		
		cout << count << '\n';
	}

	return 0;
} 
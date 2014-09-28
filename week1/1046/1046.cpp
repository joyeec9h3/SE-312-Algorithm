#define JDEBUG

#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <cmath>
#define eps 1e-6

struct period {
	int start, end;
	int len;
	double avg;  /* plane per quarter */
	bool operator<(const period& other) const {
		if (fabs(avg - other.avg) < eps) {  // this->avg == other.avg
			if (fabs(len - other.len) < eps) {  // this->len == other.len
				return end < other.end;
			} else {
				return len > other.len;
			} 
		} else {
			return avg > other.avg;
		}
	} 
} periods[310 * 310];

int main(void) {
#ifdef JDEBUG
    freopen("1046.in", "r", stdin);
    freopen("1046.out", "w", stdout);
#endif
	int ppq[310];  // plane per quater
	int sum[310];  // planes in ppq[1~i]
	int t;
	scanf("%d", &t);
	
	for (int i = 1; i <= t; ++i) {
		sum[0] = 0;  // bound
		int total; // number of quarters
		int requested; // number of requested best periods
		int available; // minimum number of quarters spent on spotting planes
		
		scanf("%d %d %d", &total, &requested, &available);
		
		for (int j = 1; j <= total; ++j) {
			scanf("%d", &ppq[j]);
			sum[j] = sum[j-1] + ppq[j]; 
		}

		int counter = 0;

		// for every [start, end] in [1, total]
		// where start - end + 1 >= available	
		for (int start = 1; start + available - 1 <= total; ++start) {
			for (int end = start + available - 1; end <= total; ++end) {
				periods[counter].start = start;
				periods[counter].end = end;
				periods[counter].len = end - start + 1;
				periods[counter].avg = 
					(double)(sum[end] - sum[start - 1]) / periods[counter].len; 
				counter++;
			}
		}

		std::sort(periods, periods + counter);
		
		printf("Result for run %d:\n", i);
		for (int p = 0; p < requested && p < counter; ++p) {
			printf("%d-%d\n", periods[p].start, periods[p].end);
		}
	}
	
	return 0;
}
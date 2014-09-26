#define JDEBUG

#include<cstdio>
#include<cstring>
#include<algorithm>

int cards[1001];
int state[1001][1001];

/**
 * Get the scores won by a in [l, r]
 *
 * @param l   start of the interval
 * @param r   end of the interval
 * @return  the scores won by a in [l, r]
 */
int dp(int l, int r) {	
    // reach the end
    if (l > r)
        return 0;
    // one card
    if (l == r)
        return cards[l];
    // [Memoization] searched
    if (state[l][r] != -1)
        return state[l][r];

    int takeLeft = 0, takeRight = 0;
    
    // check what happens if a takes left
    // cards[r] > cards[l+1], so b would take right
    // narrowdown to [l+1, r-1]
    if (cards[r] > cards[l + 1]) {
        takeLeft = dp(l + 1, r - 1) + cards[l];
    } else {  // cards[r] <= cards[l+1], so b would take next left
    // narrow down to [l+2, r]
        takeLeft = dp(l + 2, r) + cards[l];
    }

    // check what happens if a takes right
    // cards[r-1] > cards[l], so b would take next right
    // narrow down to [l, r-2]
    if (cards[r - 1] > cards[l]) {
        takeRight = dp(l, r - 2) + cards[r];
    } else {  // cards[r-1] <= cards[l], so b would take left
    // narrow down to [l+1, r-1]
        takeRight = dp(l + 1, r - 1) + cards[r];
    }

	// return the best outcome
    return state[l][r] = (takeLeft > takeRight) ? takeLeft : takeRight;
}

int main(void) {
#ifdef JDEBUG
    freopen("1176.in", "r", stdin);
    freopen("1176.out", "w", stdout);
#endif

    int n = 0;
    int game = 1;
    while(scanf("%d", &n) && n != 0) {
    	// initialization
        int sum = 0;
        memset(cards, -1, sizeof(cards));
        memset(state, -1, sizeof(state));

        for(int i = 0; i < n; i++) {
            scanf("%d", &cards[i]);
            sum += cards[i];
        }

		int scoreOfA = dp(0, n - 1);
		int scoreOfB = sum - scoreOfA;
        printf("In game %d, the greedy strategy might lose by as many as %d points.\n",
            game++, scoreOfA - scoreOfB);
    }
}
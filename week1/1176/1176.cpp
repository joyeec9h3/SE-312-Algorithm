#define JDEBUG
#include<cstdio>
#include<cstdlib>
#include<cstring>

int cards[1001];
int state[1001][1001];

/**
 * Bottom up DP.
 *
 * @param  n number of cards
 * @return   score by which b will lose
 */
int dp(int n) {
    // base case: in [i, i+1], a would take the larger one,
    // so b lose by abs(cards[i] - cards[i + 1])
    for (int i = 0; i < n - 1; i++) {
        state[i][i + 1] = abs(cards[i] - cards[i + 1]);
    }

    // dp starts from [l, l+3] since [l, l+1] is known
    // iterate: when [l, l+intvl] are left
    for (int intvl = 3; intvl < n; intvl++) {
        for (int l = 0; l < n - intvl; l++) {
            int r = l + intvl;
            int takeLeft = 0, takeRight = 0;

            // check what happens if a takes left
            // cards[r] > cards[l+1], so b would take right
            if (cards[r] > cards[l + 1]) {
                takeLeft = state[l + 1][r - 1] + cards[l] - cards[r];
            } else {  // cards[r] <= cards[l+1], so b would take next left
                takeLeft = state[l + 2][r] + cards[l] - cards[l + 1];
            }

            // check what happens if a takes right
            // cards[r-1] > cards[l], so b would take next right
            if (cards[r - 1] > cards[l]) {
                takeRight = state[l][r - 2] + cards[r] - cards[r - 1];
            } else {  // cards[r-1] <= cards[l], so b would take left
                takeRight = state[l + 1][r - 1] + cards[r] - cards[l];
            }

            // use the one with the best outcome
            state[l][r] = takeLeft > takeRight ? takeLeft : takeRight;
        }
    }

    return state[0][n - 1];
}

int main(void) {
#ifdef JDEBUG
    freopen("1176.in", "r", stdin);
    freopen("1176.out", "w", stdout);
#endif
    int n = 0;
    int game = 1;

    while (scanf("%d", &n) && n != 0) {
        // store the card numbers
        for (int i = 0; i < n; i++) {
            scanf("%d", &cards[i]);
        }

        memset(state, 0, sizeof(state));
        printf("In game %d, the greedy strategy might lose by as many as %d points.\n",
               game++, dp(n));
    }

    return 0;
}
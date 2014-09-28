#define JDEBUG

#include <cstdio>
	
int main(void) {
#ifdef JDEBUG
    freopen("1051.in", "r", stdin);
    freopen("1051.out", "w", stdout);
#endif
	const double pi = 3.1415927;
	const int ipm = 12 * 5280;  // inches per mile
	const int sph = 60 * 60;  // seconds per hour

	double diameter;  // in inches
	int revolution;
	double time;  // seconds
	int count = 1;
	while (scanf("%lf %d %lf", &diameter, &revolution, &time) && revolution != 0) {
		double miles = diameter * pi * revolution / ipm;
		double hours = time / sph;
		double mph = miles / hours;
		printf("Trip #%d: %.2lf %.2lf\n", count++, miles, mph);
	}
	
	return 0;
}

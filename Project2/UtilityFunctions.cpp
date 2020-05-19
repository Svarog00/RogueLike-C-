#include <math.h>
#include "UtilityFunctions.h"

double Distance(int px, int py, int ex, int ey)
{
	double dist = sqrt( (pow(((double)px - (double)ex), 2)) + (pow(((double)py - (double)ey), 2)) );
	return dist;
}


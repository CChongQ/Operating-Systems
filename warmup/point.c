#include <assert.h>
#include "common.h"
#include "point.h"
#include <math.h>

void
point_translate(struct point *p, double x, double y)
{
    point_set(p, point_X(p)+x, point_Y(p)+y);
}

double
point_distance(const struct point *p1, const struct point *p2)
{
    return sqrt((point_X(p1) - point_X(p2)) * (point_X(p1) - point_X(p2)) + 
            (point_Y(p1) - point_Y(p2)) * (point_Y(p1) - point_Y(p2)));
}

int
point_compare(const struct point *p1, const struct point *p2)
{
    struct point p0;
    point_set(&p0, 0.0, 0.0);
    
    double p1Euclidean = point_distance(p1, &p0);
    double p2Euclidean = point_distance(p2, &p0);
    
    if(p1Euclidean > p2Euclidean)
        return 1;
    else if(p1Euclidean < p2Euclidean)
        return -1;
    else
        return 0;
}

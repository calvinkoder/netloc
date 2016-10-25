#include <stdio.h>
#include <math.h>
#include <string.h>

#define M_TAU M_PI/2

typedef struct Point{float x; float y;} Point;
typedef struct Line{struct Point p0; struct Point p1;} Line;

float line_dist(Line l){
	return sqrt(pow(l.p1.x - l.p0.x, 2) + pow(l.p1.y - l.p0.y, 2));
}
Line line_create(float x0, float y0, float x1, float y1){
	return (Line) { 
			.p0 = (Point) { .x = x0, .y = y0 }, 
			.p1 = (Point) {.x = x1, .y = y1}
		};
}
float get_opp_angle(float d0, float d1, float d2){
	return acos(
		(pow(d0, 2) + pow(d1, 2) - pow(d2, 2))
		/ (2*d0*d1)
		);
}

Point point_trilat(Line l0, Line l1, Line l2){
	float d0 = line_dist(l0);
	float d1 = line_dist(l1);
	float d2 = line_dist(l2);

	float angle = get_opp_angle(d0, d1, d2);
	return (Point) {
		.x = l0.p1.x + d0 * cos(angle),
		.y = l0.p1.y + d0 * sin(angle)
		};
}

int main( ){
	Line l0 = line_create(1, 1, 0, 0);
	Line l1 = line_create(0, 0, 1, 0);
	Line l2 = line_create(1, 0, 1, 1);

	Point p0 = point_trilat(l0, l1, l2);
	Point p1 = point_trilat(l1, l2, l0);
	Point p2 = point_trilat(l2, l0, l1);

	printf("%f, %f\n", p0.x, p0.y);
	printf("%f, %f\n", p1.x, p1.y);
	printf("%f, %f\n", p2.x, p2.y);
	
	return 0;
}

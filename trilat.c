#include <stdio.h>
#include <math.h>
#include <string.h>

#define M_TAU M_PI/2

typedef struct Point{float x; float y;} Point;
typedef struct Line{struct Point p0; struct Point p1;} Line;

void print_angle(float a){
	printf("%f", a / M_TAU);
}

void print_point(Point p){
	printf("%f, %f", p.x, p.y);
}

void print_line(Line l){
	print_point(l.p0);
	printf(" , ");
	print_point(l.p1);
}

float line_dist(Line l){
	return sqrt(pow(l.p1.x - l.p0.x, 2) + pow(l.p1.y - l.p0.y, 2));
}

Line line_create(float x0, float y0, float x1, float y1){
	return (Line) { 
			.p0 = (Point) { .x = x0, .y = y0 }, 
			.p1 = (Point) {.x = x1, .y = y1}
		};
}
Line line_create(Point p0, Point p1){
	return (Line) { .p0 = p0, .p1 = p1 };
}
float get_opp_angle(float a, float b, float c){
	return acos(
		(pow(a, 2) + pow(c, 2) - pow(b, 2))
		/ (2*a*c)
		);
}

float get_offset_angle(float a, Line bc){
	return acos(bc.p1.x/a);
}

Point point_trilat(float b, float c, Line bc){
	float a = line_dist(bc);

	float angle = get_opp_angle(a, b, c);
	printf("angle abc:");
	print_angle(angle);
	angle += get_offset_angle(a, bc);
	printf("\noffset of C:");
	print_angle(get_offset(a, bc));
	printf("\n");
	return (Point) {
		.x = bc.p0.x + c * cos(angle),
		.y = bc.p0.y + c * sin(angle)
		};
}

int main( ){
	Point A = { .x = 1, .y = 1};
	Point B = { .x = 0, .y = 0};
	Point C = { .x = 1, .y = 0};

	Line ab = line_create(A, B);
	Line bc = line_create(B, A);
	Line ca = line_create(C, A);

	float a = line_dist(bc);
	float b = line_dist(ca);
	float c = line_dist(ab);

	printf("Triangle with points:\nA:(");
	print_point(A);
	printf("), B:(");
	print_point(B);
	printf("), C:(");
	print_point(C);
	printf(")\nsides:\na:%f, b:%f, c:%f\n"
		a, b, c);

	Point A_ = point_trilat(b, c, bc);
	printf("A:(");
	print_point(A_);
	printf(")\n");
	Point B_ = point_trilat(c, a, ca);
	printf("B:(");
	print_point(B_);
	printf(")\n");
	Point C_ = point_trilat(a, b, ab);
	printf_("C:(");
	print_point(C_);
	printf(")\n");

	return 0;
}

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <float.h>

const float M_TAU = M_PI*2;

int nearly_equal(float a, float b, float epsilon){
	float diff = fabs(a - b);
	if (a == b){
		return 1;
	}
	else if ( a == 0 || diff < FLT_MIN){
		return diff < (epsilon * FLT_MIN);
	}
	else{
		return diff / fmin(fabs(a) + fabs(b), FLT_MAX) < epsilon;
	}
}
typedef struct Point{float x; float y;} Point;

void print_angle(float a){
	printf("%f", a / M_TAU);
}

void print_point(Point p){
	printf("%f, %f", p.x, p.y);
}

float dist(Point A, Point B){
	return sqrt(pow(B.x - A.x, 2) + pow(B.y - A.y, 2));
}

float get_opp_angle(float a, float b, float c){
	return acos(
			(pow(a, 2) + pow(c, 2) - pow(b, 2))
			/ 
			(2*a*c)
		);
}

float get_offset_angle(float a, Point C_){
	float angle = acos(C_.x/a);
	printf("\noffset angle:");
	print_angle(angle);
	printf("\na*sin(angle): %f, C_.y: %f", a*sin(angle), C_.y);
	if ( !nearly_equal(a*sin(angle), C_.y, FLT_EPSILON) ){
		printf("\nnot equal");	
		angle += M_TAU / 4;
	}
	return angle;
}

Point point_trilat(float b, float c, Point B, Point C){
	float a = dist(B, C);

	printf("sides: %f, %f, %f\n", a, b, c);

	Point C_ = { .x = C.x - B.x, .y = C.y - B.y };
	printf("C_:");
	print_point(C_);	
	float ABC = get_opp_angle(a, b, c);
	printf("\nangles:\nABC:");
	print_angle(ABC);	

	float C_B_C__ = get_offset_angle(a, C_);
	printf("\nC_B_C__:");
	print_angle(C_B_C__);

	float A_B_C__ = ABC + C_B_C__;
	printf("\nA_B_C__:");
	print_angle(A_B_C__);
	printf("\n");	
	
	return (Point) {
		.x = B.x + c * cos(A_B_C__),
		.y = B.y + c * sin(A_B_C__)
		};
}

int main( ){
	Point A = { .x = 1, .y = 1};
	Point B = { .x = 0, .y = 0};
	Point C = { .x = 1, .y = 0};

	float a = dist(B, C);
	float b = dist(C, A);
	float c = dist(A, B);

	printf("Triangle with points:\nA:(");
	print_point(A);
	printf("), B:(");
	print_point(B);
	printf("), C:(");
	print_point(C);
	printf(")\nsides:\na:%f, b:%f, c:%f\n",
		a, b, c);
	printf("-----------------\npoint A estimate:\n");
	Point A_ = point_trilat(b, c, B, C);
	printf("A:(");
	print_point(A_);
	printf(")\n----------------\npoint B estimate:\n");
	Point B_ = point_trilat(c, a, C, A);
	printf("B:(");
	print_point(B_);
	printf(")\n-----------------\npoint C estimate:\n");
	Point C_ = point_trilat(a, b, A, B);
	printf("C:(");
	print_point(C_);
	printf(")\n");

	return 0;
}

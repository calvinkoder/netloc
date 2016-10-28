#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <float.h>

const float M_TAU = M_PI*2;
const float CIRCLE_STEP = 0.1; 

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
unsigned int n_chose_k(unsigned int n, unsigned int k){
	unsigned int res = 1;	
	for (int i = n; i < k; --i){
		res *= n;
	} 
	return res; 
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

float angle_opp(float a, float b, float c){
	return acos(
		(pow(a, 2) + pow(c, 2) - pow(b, 2))
		/ 
		(2*a*c)
	);
}

float angle_offset(float a, Point C_){
	float angle = acos(C_.x/a);
	if ( !nearly_equal(a*sin(angle), C_.y, FLT_EPSILON) ){
		angle += M_TAU / 4;
	}
	return angle;
}
float angle_trilat(float b, float c, Point B, Point C){
	float a = dist(B, C);
	
	Point C_ = { .x = C.x - B.x, .y = C.y - B.y };
	float ABC = angle_opp(a, b, c);
	float C_B_C__ = angle_offset(a, C_);
	float A_B_C__ = ABC + C_B_C__;

	return A_B_C__;
}
void points_unilat(Point* points, size_t* points_size, 
	float* dist, Point* source, float step){

	*points_size = M_TAU / step;	
	points = (Point*) malloc(*points_size * sizeof(Point));	
	for ( int i = 0; i < *points_size; ++i){
		*(points+i) = (Point){
			.x = (*source).x + *dist * cos(i*step),
			.y = (*source).y + *dist * sin(i*step)
		};
	}
} 
void points_duolat(Point* points, size_t* points_size, 
	float* dists, Point* source_points){

	float A_B_C__ = angle_trilat(*dists, *(dists+1), *source_points, *(source_points+1));
	
	points = (Point*) malloc(2 * sizeof(Point));
	*points = (Point){
		.x = (*source_points).x + *(dists+1) * cos(A_B_C__),
		.y = (*source_points).y + *(dists+1) * sin(A_B_C__)
	};
	*(points+1) = (Point){
		.x = (*source_points).x + *(dists+1) * cos(-A_B_C__),
		.y = (*source_points).y + *(dists+1) * sin(-A_B_C__)
	};
	*points_size = 2;
}

void points_multilat(Point* points, size_t* points_size, 
	float* dists, Point* source_points, size_t input_size){

	size_t size = (size_t) n_chose_k(size, 2);
	Point** point_pairs[size];	
	float* _dist = (float*) malloc(2 * sizeof(float));
	Point* _source_points = (Point*) malloc(2 * sizeof(Point));

	for (int i = 0; i < input_size; ++i){
		*_dist = dists[i];
		*_source_points = source_points[i];

		for (int j = i+1; j< input_size; ++j){	
			*(_dist+1) = dists[j];
			*(_source_points+1) = source_points[j];

			points_duolat(points+i+j-1, points_size, 
				_dist, _source_points);
		}
	}

	for (int i = 0; i < size; ++i){
		for (int j = 0; j<2; j++){
			points[i*2+j] = *point_pairs[i][j];
		}
	}
	*points_size = size * 2;	
}

void points_find(Point* points, size_t* points_size, 
	float* dists, Point* source_points, size_t input_size){

	if (input_size == 1){
		points_unilat(points, points_size, 
			dists, source_points, CIRCLE_STEP);
	}else if (input_size == 2){
		points_duolat(points, points_size, 
			dists, source_points);
	}else{
		points_multilat(points, points_size, 
			dists, source_points, input_size);
	}
}	

int main( ){
	Point A = { .x = 1, .y = 1};
	
	Point points[2];
	points[0] = (Point){ .x = 0, .y = 0};
	points[1] = (Point){ .x = 1, .y = 0};
	
	float dists[2];
	dists[0] = dist(A, points[1] );
	dists[1] = dist(A, points[0]);
	Point* p;
	size_t* points_size;
	points_find(p, points_size, dists, points, 2);

	for (int i = 0; i < *points_size; ++i){
		printf("\nPoint %i:\n", i);	
		print_point(*(p+i));
	}
	return 0;
}

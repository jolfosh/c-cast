#include <windows.h>
#include <stdio.h>
#include <math.h>

const int map[10][10] = 
{
	{ 1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,1,1,0,1,0,0,1 },
	{ 1,0,0,1,0,0,1,0,0,1 },
	{ 1,0,0,1,0,0,1,0,0,1 },
	{ 1,0,0,1,0,1,1,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1 },
};

const double PI = 3.14;
const int PRECISION = 64;
const int sc_width = 78;
const int sc_height = 24;
const int sc_half_width = 39;
const int sc_half_height = 12;
const double p_fov = 60; 
const double p_half_fov = 30;
double p_angle = 180;
double p_x = 2;
double p_y = 2;
double r_inc = 1;

char sc[24][78] = {' '};
char buffer[1896] = {' '};

double degree_to_R(int degree) {
	return (degree * PI / 180);
}

void clear_sc() {
	for(int i = 0; i < sc_height; i++){
		for(int j = 0; j < sc_width; j++){
			sc[i][j] = ' ';
		}
	}
}

void cast() {
	double r_angle = p_angle - p_half_fov;
	for(int i = 0; i < sc_width; i++) {
		double r_x = p_x;
		double r_y = p_y;
		double r_cos = cos(degree_to_R(r_angle)) / PRECISION;
		double r_sin = sin(degree_to_R(r_angle)) / PRECISION;
		int wall_x = -1;
		int wall_y = -1;
		int wall = 0;
		while(wall == 0) {
			r_x += r_cos;
			r_y += r_sin;
			wall_x = floor(r_x);
			wall_y = floor(r_y);
			wall = map[wall_y][wall_x];
		}
		double distance = sqrt(pow(p_x - r_x, 2) + pow(p_y - r_y, 2));

		// distance = distance * cos(degree_to_R(r_angle - p_angle));

		double wall_h = floor(sc_half_height / distance);
		for(int j = (sc_half_height - wall_h); j < (sc_half_height + wall_h); j++){
			sc[j][i] = '#';
		}
		for(int j = (sc_half_height + wall_h); j < sc_height; j++){
			sc[j][i] = '.';
		}
		r_angle += r_inc;
	}
}

void draw() {
	int i = 0;
	for(int j = 0; j < sc_height; j++){
		for(int k = 0; k < sc_width; k++){
			buffer[i] = sc[j][k];
			i++;
		}
		buffer[i] = '\n';
		i++;
		// printf("%.78s\n", sc[j]);
	}
	printf("%.1896s", buffer);
}

int main(int argc, char** argv) {
	r_inc = p_fov / sc_width;

	while(1){
		cast();
		// Sleep(100);
		// printf("\033[2J");
		draw();
		clear_sc();
		p_angle += 0.05;
	}
	return 0;
}

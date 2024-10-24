#include <windows.h>
#include <stdio.h>
#include <conio.h>
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

const float PI = 3.14;
const int PRECISION = 64;
const int sc_width = 78;
const int sc_height = 24;
const int sc_half_width = 39;
const int sc_half_height = 12;
const float p_fov = 60; 
const float p_half_fov = 30;
float p_angle = 180;
float p_x = 2;
float p_y = 2;
float r_inc = 1;

char sc[24][78] = {' '};
char buffer[1896] = {' '};

float degree_to_R(int degree) {
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
	float r_angle = p_angle - p_half_fov;
	for(int i = 0; i < sc_width; i++) {
		float r_x = p_x;
		float r_y = p_y;
		float r_cos = cos(degree_to_R(r_angle)) / PRECISION;
		float r_sin = sin(degree_to_R(r_angle)) / PRECISION;
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
		float distance = sqrt(pow(p_x - r_x, 2) + pow(p_y - r_y, 2));

		// distance = distance * cos(degree_to_R(r_angle - p_angle));

		int wall_h = floor(sc_half_height / distance);
		if (wall_h >= sc_height){
			wall_h = sc_height - 1;
		}
		
		int j_start = sc_half_height - wall_h;
		if (j_start < 0)
		{
			j_start = 0;
		}

		for(int j = j_start; j < (sc_half_height + wall_h); j++){
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

void p_move(){ 
	float p_cos = cos(degree_to_R(p_angle)) * 0.2;
	float p_sin = sin(degree_to_R(p_angle)) * 0.2;
	int c_x = floor(p_x + p_cos);
	int c_y = floor(p_y + p_sin);
	if (map[c_y][c_x] == 0) {
		p_x += p_cos;
		p_y += p_sin;
	}
}

void keypressed() {
	if(_kbhit()){
		int k = _getch();
		if (k == 'a') {
			p_angle -= 2;	
		} else if (k == 'd') {
			p_angle += 2;
		} else if (k == 'w') {
			p_move();
		}
	}
}

int main(int argc, char** argv) {
	r_inc = p_fov / sc_width;

	while(1){
		cast();
		// Sleep(100);
		// printf("\033[2J");
		draw();
		keypressed();
		clear_sc();
		// p_angle += 0.05;
	}
	return 0;
}

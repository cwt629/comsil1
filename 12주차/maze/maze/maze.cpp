#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void GetInfo();

int width, height;
int* garo_wall, sero_wall, cell;
int num = 1;
FILE* file;

int main() {
	file = fopen("maze.maz", "wt");
	GetInfo();



	fclose(file);
}

void GetInfo() {
	printf("Width of the maze: ");
	scanf("%d", &width);
	printf("Height of the maze: ");
	scanf("%d", &height);
}
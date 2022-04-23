#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void GetInfo();
void print_maze_line(int i);
int width, height;
int** cell;
int num = 1;
int* break_wall;
FILE* file;

int main() {
	int i, j;
	int vertical_flag = 0;
	file = fopen("maze.maz", "wt");
	GetInfo();
	
	cell = (int**)malloc(sizeof(int*) *height);
	for (i = 0; i < height; i++) {
		cell[i] = (int*)malloc(sizeof(int) * width);
		for (j = 0; j < width; j++)
			cell[i][j] = 0;
	}
	break_wall = (int*)malloc(sizeof(int) * (width - 1));
	for (i = 0; i < width - 1; i++) {
		break_wall[i] = 0;
	}

	srand(time(NULL));
	// ù�ٿ� ���� ����
	for (i = 0; i < width; i++) {
		cell[0][i] = num++;
	}
	

	// ���� �ٿ� ���� ����
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) { // ������ �ձ�
			if (j < width - 1) {
				if (cell[i][j] != cell[i][j + 1]) {
					if (rand() % 2) {
						cell[i][j] = min(cell[i][j], cell[i][j + 1]);
						cell[i][j + 1] = cell[i][j];
						break_wall[j] = 1;
					}
					else if (!cell[i][j]) cell[i][j] = num++;
				}
			}
			else if (!cell[i][j]) cell[i][j] = num++;
		}
		if (i < height - 1) {// �Ʒ��� �ձ�
			j = 0;
			while (j < width) {
				do {
					if (rand() % 2) {
						cell[i + 1][j] = cell[i][j];
						vertical_flag = 1;
					}
					else cell[i + 1][j] = num++;
					j++;
				} while (cell[i][j-1] == cell[i][j]);

				if (!vertical_flag) {
					do {
						j--;
					} while (j > 0 && cell[i][j] == cell[i][j - 1]);
				}
				else vertical_flag = 0;
			}
		}
		else { // ������ ���� ��� ���� �㹰����
			for (j = 0; j < width - 1; j++) {
				if (cell[i][j] != cell[i][j + 1]) {
					cell[i][j] = min(cell[i][j], cell[i][j + 1]);
					cell[i][j + 1] = cell[i][j];
					break_wall[j] = 1;
				}
			}
		}
		
		print_maze_line(i);
		for (j = 0; j < width - 1; j++)
			break_wall[j] = 0;
	}
	
	// end
	for (i = 0; i < height; i++)
		free(cell[i]);
	free(cell);
	fclose(file);
}

void GetInfo() {
	printf("Width of the maze: ");
	scanf("%d", &width);
	printf("Height of the maze: ");
	scanf("%d", &height);
}

void print_maze_line(int i) {
	int j;
	if (i == 0) { // ���� ����
		for (j = 0; j < width; j++) {
			printf("+-");
		}
		printf("+\n");
	}
	for (j = 0; j < width; j++) { // ��� ���� ����
		if (j == 0) printf("|");
		else if (!break_wall[j-1]) printf("|");
		else printf(" ");
		printf(" ");
	}
	printf("|\n");
	if (i == height - 1) {
		for (j = 0; j < width; j++) {
			printf("+-");
		}
		printf("+\n");
	}
	else { // ��� �Ʒ��� ����
		for (j = 0; j < width; j++) {
			printf("+");
			if (cell[i][j] != cell[i + 1][j]) printf("-");
			else printf(" ");
		}
		printf("+\n");
	}
}
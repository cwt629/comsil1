#include "book.h"
#include <stdio.h>
// 20161255 장원태
void PrintingOutput(int *count){
	int i;
	for (i=0;i<10;i++){ // 0부터 9까지 출력 및 초기화 진행
	printf("%d ", count[i]); // 각 숫자의 개수 출력
	count[i]=0; // 다음 과정을 수행하기 위해 각 숫자의 개수를 초기화함
	}
	printf("\n"); // 모든 출력을 마친 뒤 줄바꿈
}

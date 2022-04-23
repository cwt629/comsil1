#include "book.h"
#include <stdio.h>
// 20161255 장원태
void GettingInput(int *input, int i){
	int num;
	scanf("%d", &num); // (i+1)번째 테스트케이스의 페이지수를 입력받음
	input[i]=num; // 입력받은 페이지수를 input 배열에 저장. 이 때, (i+1)번째 테스트케이스의 데이터라는 것을 저장하기 위해 i번째 인덱스에 저장
}

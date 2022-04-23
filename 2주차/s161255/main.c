#include "book.h"
// 20161255 장원태
int main(){
	int t, i, j, number;
	int *input; // 페이지수를 입력받을 배열
	int count[10]={0}; // 각 숫자의 개수를 입력할 배열
	scanf("%d", &t); // 테스트 케이스 입력
	
	input=(int*)malloc(sizeof(int)*t); // 테스트 케이스의 수만큼 input 배열을 할당받음을 통해, 각 테스트케이스의 입력값을 저장할 배열을 만듦
	
	for(i=0;i<t;i++){ // 입력받은 테스트케이스의 수만큼 scanf 사용
	GettingInput(input, i); // 각 테스트케이스의 페이지 수를 입력받음
	}
	
	for(i=0;i<t;i++){ // 입력받은 테스트케이스의 수만큼 과정 진행
	number=input[i]; // (i+1)번째 테스트케이스의 입력값을 받음
	DigitCounter(count, number); // 해당 테스트케이스의 입력값(number)을 넘겨주어 1부터 number까지의 모든 숫자를 세어, 각 숫자에 대해 count 배열에 그 개수를 입력
	PrintingOutput(count); // 해당 테스트케이스에 센 count를 출력한 뒤, count를 초기화
	}	
	free(input);		// malloc 함수 수행에 따라, 할당받은 배열을 반환함
}

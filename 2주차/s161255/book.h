#include <stdio.h>
#include <stdlib.h>
// 20161255 장원태
void GettingInput(int *input, int i); // 각 테스트케이스의 수를 입력받는 함수
void DigitCounter(int *count, int num); // 해당 테스트케이스에 대해 1부터 num페이지까지 존재하는 모든 페이지를 구성하는 각 숫자들의 개수를 세는 함수
void PrintingOutput(int *count); // 센 각 숫자들의 개수를 출력하고, 그 count를 초기화하는 함수

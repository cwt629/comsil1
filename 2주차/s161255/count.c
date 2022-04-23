#include "book.h"
#include <stdio.h>
// 20161255 장원태
void DigitCounter(int *count, int num){
	int i, j, remainder;
	for (i=1;i<=num;i++){ // 1부터 num까지 각 숫자를 셈
		for (j=i;j>0;j=j/10){ // 해당 수를 10씩 나누며, 그 값이 0이 될 때까지 진행함. 해당 수의 끝자리수부터 세어간다
		remainder=j%10; // 해당 수의 일의 자릿수를 가리키게 됨
		count[remainder]++; // 일의 자릿수에 대해 1회 count를 진행함
		}
	}
}

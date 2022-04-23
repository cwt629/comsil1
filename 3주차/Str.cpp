#include "Str.h"
#include <iostream>
#include <string.h>

using namespace std;

Str::Str(int leng){
	str = new char[leng]; // leng 길이만큼의 배열 할당
}

Str::Str(char *neyong){
	str = new char[strlen(neyong)]; // neyong의 길이만큼의 배열 할당
	strcpy(str, neyong); // str을 neyong으로 초기화
	len = strlen(neyong); // len에 string(neyong)의 길이 저장
}

Str::~Str(){
	delete []str; // 할당받은 배열을 제거하는 소멸자
}

int Str:: length(void){
	return len; // string의 길이를 리턴
}

char* Str::contents(void){
	return str; // string의 내용을 리턴
}

int Str::compare(class Str& a){
	return strcmp(str, a.contents()); // str과 a의 내용을 비교하는 strcmp 함수의 반환값을 리턴	
}

int Str::compare(char *a){
	return strcmp(str, a); // str과 a의 내용을 비교하는 strcmp 함수의 반환값을 리턴. 단, 이 때의 a는 배열에 대한 포인터, 즉 내용 자체를 가리키므로 a와 바로 비교할 수 있음
}

void Str::operator=(char *a){ // = 연산자에 대한 다중정의(인자에 포인터가 들어간 경우)
	int size = strlen(a); // a의 길이 저장
	strcpy(str, a); // str에 a의 내용을 덮어씌움
	len = size; // len에 a의 길이 저장
}

void Str::operator=(class Str& a){ // = 연산자에 대한 다중정의(인자에 a가 들어간 경우)
	int size = a.length(); // a의 길이 저장
	strcpy(str, a.contents()); // str에 a의 내용을 덮어씌움
	len = size;
} // len에 a의 길이 저장

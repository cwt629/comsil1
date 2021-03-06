#include "tetris.h"

static struct sigaction act, oact;
int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));
	

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		case MENU_RANK: rank(); break;
		case MENU_REC_PLAY: recommendedPlay(); break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	nextBlock[2]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	recRoot = (treepointer)malloc(sizeof(struct _RecNode));
	recRoot->lv = 0;
	recRoot->score = 0;
	recRoot->f = field;
	/*for (j=0;j<HEIGHT;j++){
		for (i=0; i<WIDTH; i++){
			recRoot->f[j][i] = 0;
		}
	}*/
	modified_recommend(recRoot);
	//recommend(recRoot);
	DrawOutline();
	DrawField();
	DrawBlockWithFeatures(blockY,blockX,nextBlock[0],blockRotate);
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);
	
	DrawBox(9, WIDTH+10, 4, 8);
	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15,WIDTH+10);
	printw("SCORE");
	DrawBox(16,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	case ' ':
		drawFlag = 1;
		while (drawFlag){
			if((drawFlag = CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX))){
			blockY++;
			DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
		}
		}
		
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(17,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
	for (i=0; i<4; i++){
		move(10+i,WIDTH+13);
		for (j=0; j<4; j++){
			if (block[nextBlock[2]][0][i][j] == 1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}	
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int i, j;
	for (i=0;i<BLOCK_HEIGHT;i++){
	for (j=0;j<BLOCK_WIDTH;j++){
			if (block[currentBlock][blockRotate][i][j] == 1){
				if (f[i+blockY][j+blockX] == 1 || i+blockY >= HEIGHT || j+blockX<0 || j+blockX>=WIDTH)
				return 0;
			}
		}
	}
	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int i, j;
	int tempR = blockRotate; int tempY = blockY; int tempX = blockX;
	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	switch(command){
		case KEY_UP:
			tempR = (tempR+3)%4;
			break;
		case KEY_DOWN:
			tempY--;
			break;
		case KEY_LEFT:
			tempX++;
			break;
		case KEY_RIGHT:
			tempX--;
			break;
	}
	int tempYshadow = tempY;
	while (CheckToMove(f, currentBlock, tempR, tempYshadow+1, tempX))
		tempYshadow++;
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	for (i=0; i<4; i++){
	for (j=0; j<4; j++){
		if (block[currentBlock][tempR][i][j] == 1 && i+tempY>=0){
			move(i+tempY+1, j+tempX+1);
			printw(".");
		}
		if (block[currentBlock][tempR][i][j] == 1 && i+tempYshadow>=0){
			move(i+tempYshadow+1, j+tempX+1);
			printw(".");
		}
	}
	}
	//3. 새로운 블록 정보를 그린다.
	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);

	move(HEIGHT, WIDTH+10); 
}

void BlockDown(int sig){
	// user code
	if (CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX) == 1){
	blockY++;
	DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}
	else{
		if (blockY == -1) gameOver = 1;
		score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score += DeleteLine(field);
		PrintScore(score);

		nextBlock[0] = nextBlock[1];
		nextBlock[1] = nextBlock[2];
		nextBlock[2] = rand()%7;

		recRoot->f = field; recRoot->lv = 0; recRoot->score = 0;
		modified_recommend(recRoot);
		//recommend(recRoot);
		blockRotate = 0; blockY = -1; blockX = (WIDTH/2)-2;
		
		DrawNextBlock(nextBlock);
		DrawField();
		DrawBlockWithFeatures(blockY, blockX, nextBlock[0], blockRotate);		
	}
	timed_out = 0;
	//강의자료 p26-27의 플로우차트를 참고한다.
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int i, j; int touched=0;
	//Block이 추가된 영역의 필드값을 바꾼다.
	for (i=0; i<BLOCK_HEIGHT; i++){
	for (j=0; j<BLOCK_WIDTH; j++){
		if (block[currentBlock][blockRotate][i][j] == 1){
			f[blockY+i][blockX+j] = 1;
			if ((f[blockY+i+1][blockX+j] == 1) || (blockY+i+1 == HEIGHT)) touched++;
		}
	}
	}
	return touched*10;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code
	int i, j, k;
	int check = 1;
	int deletedline = 0;
	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	for (i=1; i<HEIGHT; i++){
		check = 1;
		for (j=0; j<WIDTH; j++){
			if (f[i][j] == 0){
				check = 0;
				break;
			}
		}
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
	if (check){
		for (j=i; j>0; j--){
			for (k=0; k<WIDTH; k++)
				f[j][k] = f[j-1][k];
			}
		deletedline++;
		}
	}
	return deletedline*deletedline*100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
	while (CheckToMove(field, blockID, blockRotate, y+1, x))
		y++;
	DrawBlock(y, x, blockID, blockRotate, '/');
}

void createRankList(){
	// 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
	// 1. "rank.txt" 열기
	// 2. 파일에서 랭킹정보 읽어오기
	// 3. LinkedList로 저장
	// 4. 파일 닫기
	FILE *fp;
	int i, j, count, tempscore;
	char str[100], tempname[NAMELEN];
	head = NULL;
	nodepointer temp, last=NULL;
	// 1. 파일 열기
	fp = fopen("rank.txt", "r");
	if (fp == NULL) return;	
	// 2. 정보 읽어오기
	fscanf(fp, "%d", &count);
	score_number = count;
	fgetc(fp);
	// 3. LinkedList로 저장
	for (i=0; i<count; i++){
		fgets(str, 100, fp);
		sscanf(str, "%s %d", tempname, &tempscore);
		temp = (nodepointer)malloc(sizeof(struct _node));
		strcpy(temp->name, tempname);
		temp->score = tempscore;
		temp->link = NULL;
		if (!head) {head=temp; last=temp;}
		else {
		last->link = temp;
		last = temp;
		}
	}
	// 4. 파일 닫기
	fclose(fp);
}

void rank(){
	// 목적: rank 메뉴를 출력하고 점수 순으로 X부터 Y까지 출력함
	// 1. 문자열 초기화
	int X=1, Y=score_number, ch, i, j;
	nodepointer ptr;
	clear();

	// 2. printw()로 3개의 메뉴 출력
	move(0, 0);
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");
	// 3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
	ch = wgetch(stdscr);
	// 4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	// 4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y 사이의 rank 출력
	if (ch == '1'){
		createRankList();
		echo();
		printw("X: ");
		scanw("%d", &X);
		printw("Y: ");
		scanw("%d", &Y);
		if (!X) X=1;
		if (!Y) Y=score_number;
		printw("       name       |   score   \n");
		printw("------------------------------\n");
		if (X>Y){
			printw("search failure: no rank in the list\n");
		}
		else{
			ptr=head;
			for (i=1; i<X && ptr!=NULL; i++) ptr=ptr->link;
			for (j=0; j<=Y-i && ptr!=NULL; j++){
				printw(" %-17s|   %-8d\n", ptr->name, ptr->score);
				ptr=ptr->link;
			}
			
		}
		noecho();	
	}
	// 4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if (ch == '2'){
		char str[NAMELEN+1];
		int check = 0;
		createRankList();
		echo();
		printw("input the name: ");
		scanw("%s", &str);
		printw("       name       |   score   \n");
		printw("------------------------------\n");
		for (ptr=head; ptr!=NULL; ptr=ptr->link){
			if (!strcmp(str, ptr->name)){
				check = 1;
				printw(" %-17s|   %-8d\n", ptr->name, ptr->score);
			}
		}
		if (!check) printw("search failure: no name in the list\n");
		noecho();
	}
	// 4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if (ch == '3'){
		int num, count=1;
		int flag = 1;
		nodepointer prev=NULL;
		createRankList();
		echo();
		printw("input the rank: ");
		scanw("%d", &num);
		for (ptr=head; ptr!=NULL && count<num; count++){
			prev = ptr;
			ptr = ptr->link;
		}
		if (num<=0 || count!=num){
			printw("search failure: the rank not in the list\n");
			flag = 0;
		}
		else if (!prev){
			head = head->link;
			free(ptr);
		}
		else {
			prev->link = ptr->link;
			free(ptr);
		}
		if (flag){
		score_number--;
		writeRankFile();
		printw("result: the rank deleted\n");
		}
		noecho();
	}
	getch();
}

void writeRankFile(){
	// 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료
	int sn, i;
	nodepointer temp;
	// 1. "rank.txt" 연다
	FILE *fp = fopen("rank.txt", "r");
	if (fp==NULL) return;
	// 2. 랭킹 정보들의 수를 "rank.txt"에 기록
	fscanf(fp, "%d", &sn);
	fclose(fp);
	fp = fopen("rank.txt", "w");
	// 3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
	if (sn == score_number) return;
	else {
		fprintf(fp, "%d\n", score_number);
		temp = head;
		while (temp){
			fprintf(fp, "%s %d\n", temp->name, temp->score);
			temp = temp->link;
		}
	}
	fclose(fp);
}

void newRank(int score){
	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	char str[NAMELEN+1];
	int i, j;
	nodepointer new, ptr=head;
	clear();
	// 1. 사용자 이름을 입력받음
	printw("your name: ");
	echo();
	getstr(str);
	noecho();
	// 2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가
	new = (nodepointer)malloc(sizeof(struct _node));
	strcpy(new->name, str); new->score=score; new->link=NULL;
	if (score_number == 0) head=new;
	else {
		if (head->score <= new->score){
			new->link = head;
			head = new;
		}
		else{
			while (ptr->link != NULL && ptr->link->score > score)
				ptr=ptr->link;
			new->link = ptr->link;
			ptr->link = new;
		}
	}
	score_number++;
	writeRankFile();
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
	DrawBlock(y, x, blockID, blockRotate, 'R');
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
	DrawRecommend(recommendY, recommendX, blockID, recommendR);
	DrawShadow(y, x, blockID, blockRotate);
	DrawBlock(y, x, blockID, blockRotate, ' ');
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
	int rotate, Xpos, x, y; int Ypos=0;
	int i=0; int touched; int accumulatedScore;
	// user code
	for (rotate=0; rotate<NUM_OF_ROTATE; rotate++){
		for (Xpos=-1; Xpos<WIDTH; Xpos++){
			Ypos=0;
			while (Ypos<HEIGHT && CheckToMove(root->f, nextBlock[root->lv], rotate, Ypos, Xpos)) Ypos++;
			Ypos--;
			if (Ypos == -1) continue;
			root->c[i] = (treepointer)malloc(sizeof(struct _RecNode));
			root->c[i]->lv = root->lv+1;
			root->c[i]->f = (char(*)[WIDTH])malloc(sizeof(char)*HEIGHT*WIDTH);
			for (y=0; y<HEIGHT; y++){
			for (x=0; x<WIDTH; x++){
				root->c[i]->f[y][x] = root->f[y][x];
				}
			}
			touched = 0;
			//root->c[i]->score = root->score;
			/*Ypos=0;
			while (Ypos<HEIGHT && CheckToMove(root->c[i]->f, nextBlock[root->lv], rotate, Ypos, Xpos)) Ypos++;
			Ypos--;
			if (Ypos == -1) continue;*/
			
			for (y=0; y<4; y++){
			for (x=0; x<4; x++){
				if (block[nextBlock[root->lv]][rotate][y][x] == 1){
					if (Ypos+y+1 == HEIGHT || root->c[i]->f[Ypos+y+1][Xpos+x] == 1) touched++;
					root->c[i]->f[Ypos+y][Xpos+x] = 1;
				}
			}
			}
			//root->c[i]->score = DeleteLine(root->c[i]->f);
			accumulatedScore = root->score + touched*10;
			accumulatedScore += DeleteLine(root->c[i]->f);
			root->c[i]->score = accumulatedScore;
			if (root->c[i]->lv < VISIBLE_BLOCKS){
				accumulatedScore = recommend(root->c[i]);
				//root->c[i]->score += recommend(root->c[i]);
			}
			//accumulatedScore = root->c[i]->score + touched*10;
			if (max <= accumulatedScore){
				max = accumulatedScore;
				if (root->lv == 0){
				recommendR = rotate; recommendX = Xpos; recommendY = Ypos;}
			}
			i++;
		}
	}
	return max;
}

int modified_recommend(RecNode *root){

	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
	int rotate, Xpos, x, y; int Ypos=0;
	int i=0; int touched; int accumulatedScore;
	// user code
	for (rotate=0; rotate<NUM_OF_ROTATE; rotate++){
		for (Xpos=-1; Xpos<WIDTH; Xpos++){
			Ypos=0;
			while (Ypos<HEIGHT && CheckToMove(root->f, nextBlock[root->lv], rotate, Ypos, Xpos)) Ypos++;
			Ypos--;
			if (Ypos == -1) continue;
			root->c[i] = (treepointer)malloc(sizeof(struct _RecNode));
			root->c[i]->lv = root->lv+1;
			root->c[i]->f = (char(*)[WIDTH])malloc(sizeof(char)*HEIGHT*WIDTH);
			for (y=0; y<HEIGHT; y++){
			for (x=0; x<WIDTH; x++){
				root->c[i]->f[y][x] = root->f[y][x];
				}
			}
			touched = 0;
			for (y=0; y<4; y++){
			for (x=0; x<4; x++){
				if (block[nextBlock[root->lv]][rotate][y][x] == 1){
					if (Ypos+y+1 == HEIGHT || root->c[i]->f[Ypos+y+1][Xpos+x] == 1) touched++;
					root->c[i]->f[Ypos+y][Xpos+x] = 1;
				}
			}
			}
			accumulatedScore = root->score + touched*10;
			accumulatedScore += DeleteLine(root->c[i]->f);
			root->c[i]->score = accumulatedScore;
			if (root->lv == 0 && root->c[i]->score <= 10){ // 첫 블록이 닿을 때 10점이 나오면 해당 가지를 삭제(pruning tree)
				free(root->c[i]->f);
				free(root->c[i]);
				continue;
			}
			if (root->c[i]->lv < VISIBLE_BLOCKS-1){
				accumulatedScore = modified_recommend(root->c[i]);
			}
			if (max <= accumulatedScore){
				max = accumulatedScore;
				if (root->lv == 0){
				recommendR = rotate; recommendX = Xpos; recommendY = Ypos;}
			}
			i++;
		}
	}
	return max;
}

void recommendedPlay(){
	// user code
	int command;
	int not_moved = 1;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}
		not_moved = 1;
		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
		if (blockX > recommendX){
			ProcessCommand(KEY_LEFT);
			not_moved = 0;
		}
		else if (blockX < recommendX){
			ProcessCommand(KEY_RIGHT);
			not_moved = 0;
		}
		else if (recommendR != blockRotate){
			ProcessCommand(KEY_UP);
			not_moved = 0;
		}
		if (not_moved){
			ProcessCommand(' ');
		}
		
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

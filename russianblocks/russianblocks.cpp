#include <bits/stdc++.h>
#include <Windows.h>
using namespace std;

const int row = 32;
const int line = 25;
bool handycaps = 1;		//disable or enable handycap
int handycapn = 30;		//number of handycap
int slpt = 500;				//sleep time
//the numbers in background should reprisent as below.
//0 is for empty, printed as " "
//1 is for block, printed as []
//2 is for sides, printed as ##	(no longer used)
//3 is for debug, printed as N
//4 is for operating block, printed as []
//everything else is printed as N

//the numbers in type should reprisent as below.
//0 is for	1234	7 for 1
//						  2
//						  3
// 						  4							3
//										4			2
//1 is for	123		8 for 41	9 for 321	10 for  14
//			4  			   2
//						   3
// 							2			4		   3
//2 is for	213 	11 for 41	12 for 312	13 for 14
//			 4  			3					   2
// 
//3 is for	123
//			  4
//  
//4 is for	12
//			 34
//
//5 is for  12
//		   34
// 
//6 is for	12
//			34

/*
the map should behave like this:
	row 1		2		3		4		5		6......
line
1		[1][1]  [2][1]  [3][1]  [4][1]  [5][1]
2		[1][2]  [2][2]  ......
3		[1][3]  [2][3]  ......
......
*/

/*
printing will behave like this:
########################
##a					  ##
##b^c				  ##		note there is a blank between b and c
##					  ##
##					  ##
......
##					  ##
########################

the place of "a" would be background[1][1], "b" would be background[1][2], "c" would be background[2][2]
*/

/*
the printing operation is being changed
it will consist of a background and a upper movement layer of 4 symbols as block type is defined
*/
struct block
{
	int x;
	int y;
};
block a[6],d[6];		//use a[1] to a[4] , a[0] is not used, a[5] is for the turning center
bool needload = 0,gks = 0;
int background[row][line], dump[row][line];
int i, n, m;
int q, k, type, flag = 1, fulline = 0, level = 0;
int flevel = 0, plevel;
char gk;
int clearline;
int turnednum=0;		//debug use
char cheat;

void handcap()
{
	for (i = 0; i < handycapn; i++)
	{
		n = rand() % row-1;
		m = rand() % 4;
		if (m == 1)
			background[n][line-1] = 1;
		if (m == 2)
		{
			background[n][line-1] = 1;
			background[n][line-2] = 1;
		}
		if (m == 3)
		{
			background[n][line-1] = 1;
			background[n][line-2] = 1;
			background[n][line-3] = 1;
		}
	}
}

void cleanfull()
{
	for (i = 0; i < row; i++)
	{
		for (m = 0; m < line; m++)
		{
			dump[i][m] = background[i][m];
		}
	}
	for (m = 0; m < clearline; m++)
	{
		for (i = 0; i < row; i++)
		{
			background[i][m + 1] = dump[i][m];
		}
	}
	flevel++;
	plevel = level;
	level = flevel / 5;
}

void checkgame()		//check if loose and if full line
{
	for(i=0;i<row;i++)
	{
		if(background[i][1]==1)
		flag=0;
	}
	for (m = 0; m < line; m++)
	{
		n = 0;
		for (i = 0; i < row; i++)
		{
			if (background[i][m] == 1)
				n++;
		}
		if (n == row)
		{
			fulline = 1;
			clearline = m;
		}
		else
			fulline = 0;
		if (fulline == 1)
			cleanfull();
	}
}

void downblock()
{
	bool ff=1;
	for(i=1;i<=4;++i)
    {
	    if(background[a[i].x][a[i].y+1]==1||a[i].y==line-1)		//check below
	       	ff=0;
    }
	if(!ff) needload=1;
	if(ff)
		for(i=1;i<=5;++i) a[i].y++;
}

void leftblock()
{
	bool ff=1;
	for(i=1;i<=4;++i)
		if(background[a[i].x-1][a[i].y]==1||a[i].x==0)		//check left
			ff=0;
	if(ff)
		for(i=1;i<=5;++i) a[i].x--;
}

void rightblock()
{
	bool ff=1;
	for(i=1;i<=4;++i)
		if(background[a[i].x+1][a[i].y]==1||a[i].x==row-1)		//check right
			ff=0;
	if(ff)
		for(i=1;i<=5;++i) a[i].x++;
}

void solid()
{
	background[a[1].x][a[1].y] = 1;
	background[a[2].x][a[2].y] = 1;
	background[a[3].x][a[3].y] = 1;
	background[a[4].x][a[4].y] = 1;

}

void shape(int t,int tx,int ty)//a[5]is the centre
{
	if(t==0)											//^***				
	{	
		for(int i=1;i<=4;++i) a[i].y=ty,a[i].x=tx+i-1;
	}
	else if(t==1)										//^**
	{													//*
		for(int i=1;i<=3;++i) a[i].y=ty,a[i].x=tx+i-1;
		a[4].y=ty+1;a[4].x=tx;
	}
	else if(t==2)										//^**
	{													// *
		for(int i=1;i<=3;++i) a[i].y=ty,a[i].x=tx+i-1;
		a[4].y=ty+1;a[4].x=tx+1;
	}
	else if(t==3)										//^**
	{													//  *
		for(int i=1;i<=3;++i) a[i].y=ty,a[i].x=tx+i-1;
		a[4].x=tx+2;a[4].y=ty+1;
	}
	else if(t==4)										//^
	{													//***
		for(int i=1;i<=3;++i) a[i].y=ty+1,a[i].x=tx+i-1;
		a[4].y=ty;a[4].x=tx;
	}
	else if(t==5)										//^*
	{													//***
		for(int i=1;i<=3;++i) a[i].y=ty+1,a[i].x=tx+i-1;
		a[4].y=ty;a[4].x=tx+1;
	}
	else if(t==6)										//^ *
	{													//***
		for(int i=1;i<=3;++i) a[i].y=ty+1,a[i].x=tx+i-1;
		a[4].y=ty;a[4].x=tx+2;
	}
	else if(t==7)										//^*
	{													// **
		a[1].y=a[2].y=ty  ;a[1].x=tx  ;a[2].x=tx+1;	
		a[3].y=a[4].y=ty+1;a[3].x=tx+1;a[4].x=tx+2;
	}
	else if(t==8)										//^**
	{													//**
		a[1].y=a[2].y=ty+1;a[1].x=tx  ;a[2].x=tx+1;
		a[3].y=a[4].y=ty  ;a[3].x=tx+1;a[4].x=tx+2;
	}
	else if(t==9)										//^*
	{													//**
		a[1].y=a[2].y=ty  ;a[1].x=tx;a[2].x=tx+1;
		a[3].y=a[4].y=ty+1;a[3].x=tx;a[4].x=tx+1;
	}
	else if(t==10)										//^*
	{													// *
		for(int i=1;i<=3;++i) a[i].y=ty+i-1,a[i].x=tx+1;// *
		a[4].y=ty;a[4].x=tx;
	}
	else if(t==11)										//^*
	{													//**
		for(int i=1;i<=3;++i) a[i].y=ty+i-1,a[i].x=tx+1;// *
		a[4].y=ty+1;a[4].x=tx;	
	}
	else if(t==12)										//^*
	{													// *
		for(int i=1;i<=3;++i) a[i].y=ty+i-1,a[i].x=tx+1;//**
		a[4].y=ty+2;a[4].x=tx;
	}
	else if(t==13)										//^*
	{													//*
		for(int i=1;i<=3;++i) a[i].y=ty+i-1,a[i].x=tx;	//*
		a[4].y=ty;a[4].x=tx+1;
	}
	else if(t==14)										//^
	{													//**
		for(int i=1;i<=3;++i) a[i].y=ty+i-1,a[i].x=tx;	//*
		a[4].y=ty+1;a[4].x=tx+1;
	}
	else if(t==15)										//^
	{													//*
		for(int i=1;i<=3;++i) a[i].y=ty+i-1,a[i].x=tx;	//**
		a[4].y=ty+2;a[4].x=tx+1;	
	}
	else if(t==16)										//^
	{													//**
		a[1].x=a[2].x=tx  ;a[1].y=ty  ;a[2].y=ty+1;		// *
		a[3].x=a[4].x=tx+1;a[3].y=ty+1;a[4].y=ty+2;
	}
	else if(t==17)										//^*
	{													//**
		a[1].x=a[2].x=tx+1;a[1].y=ty  ;a[2].y=ty+1;		//*
		a[3].x=a[4].x=tx  ;a[3].y=ty+1;a[4].y=ty+2;
	}
	else if(t==18)										//^
	{													//*
		for(int i=1;i<=4;++i) a[i].y=ty+i-1,a[i].x=tx;	//*
	}													//*
	a[5].y=ty;a[5].x=tx;
}

int wid(int t)		//width of shape(used when the shape is on the left/right)
{
	if(t==0) return 4;
	else if(t>=1&&t<=8) return 3;
	else if(t>9&&t<=17) return 2;
	else if(t==18) return 1;
	else return 0;
}


void blockload()
{
	int s,t=rand()%18+1;
	type = t;
	Sleep(500);
	s=rand()%(row-wid(t));
	shape(t,s,1);
	needload = 0;
}

void checkturn()
{
	int fg = 0;
	for(i=1;i<5;i++)
	{
		if(a[i].x<0||a[i].x>row-1)
			fg = 1;
		if(a[i].y<0||a[i].y>line-1)
			fg = 1;
		if(background[a[i].x][a[i].y]==1)
			fg = 1;
	}
	if(fg==1)		//if checkturn failure than rollback
	{
		for(i=1;i<6;i++)
		{
			a[i].x=d[i].x;
			a[i].y=d[i].y;
		}
	}
}

void turnblock()
{
	turnednum++;
	for(i=1;i<6;i++)
	{
		d[i].x=a[i].x;
		d[i].y=a[i].y;
	}
	if(type==0)
	{
		shape(18,d[5].x,d[5].y);
		checkturn();
		type=18;
	}
	else if(type==1)
	{
		shape(15,d[5].x,d[5].y);
		checkturn();
		type=15;
	}
	else if(type==2)
	{
		shape(14,d[5].x,d[5].y);
		checkturn();
		type=14;
	}
	else if(type==3)
	{
		shape(13,d[5].x,d[5].y);
		checkturn();
		type=13;
	}
	else if(type==4)
	{
		shape(12,d[5].x,d[5].y);
		checkturn();
		type=12;
	}
	else if(type==5)
	{
		shape(11,d[5].x,d[5].y);
		checkturn();
		type=11;
	}
	else if(type==6)
	{
		shape(10,d[5].x,d[5].y);
		checkturn();
		type=10;
	}
	else if(type==7)
	{
		shape(17,d[5].x,d[5].y);
		checkturn();
		type=17;
	}
	else if(type==8)
	{
		shape(16,d[5].x,d[5].y);
		checkturn();
		type=16;
	}
	else if(type==9)
	{
		shape(9,d[5].x,d[5].y);
		checkturn();
		type=9;
	}
	else if(type==10)
	{
		shape(1,d[5].x,d[5].y);
		checkturn();
		type=1;
	}
	else if(type==11)
	{
		shape(2,d[5].x,d[5].y);
		checkturn();
		type=2;
	}
	else if(type==12)
	{
		shape(3,d[5].x,d[5].y);
		checkturn();
		type=3;
	}
	else if(type==13)
	{
		shape(4,d[5].x,d[5].y);
		checkturn();
		type=4;
	}
	else if(type==14)
	{
		shape(5,d[5].x,d[5].y);
		checkturn();
		type=5;
	}
	else if(type==15)
	{
		shape(6,d[5].x,d[5].y);
		checkturn();
		type=6;
	}
	else if(type==16)
	{
		shape(8,d[5].x,d[5].y);
		checkturn();
		type=8;
	}
	else if(type==17)
	{
		shape(7,d[5].x,d[5].y);
		checkturn();
		type=7;
	}
	else if(type==18)
	{
		shape(0,d[5].x,d[5].y);
		checkturn();
		type=0;
	}
}

int generate()
{
	for (n = 0; n < line; n++)
	{
		for (i = 0; i < row; i++)
		{
			background[i][n] = 0;
		}
	}
	if (handycaps == 1)
	{
		handcap();
	}
	return 0;
}


void blocker()
{
	if(needload==1)
	{
		solid();
		blockload();
	}
	if(gk=='a')
		leftblock();
	else if(gk=='d')
		rightblock();
	else if(gk=='w')
		turnblock();
	else if(gk=='s')
	{
		downblock();
		downblock();
	}
	downblock();
}

void printer()
{
	system("cls");
	for (m = -1; m < line + 1; m++)
	{
		for (i = -1; i < row + 1; i++)
		{
			k = 0;
			for (n=1;n<5;n++)
			{
				if (i == a[n].x && m == a[n].y)
				{
					cout << "[]";
					k = 1;
				}
			}
			if (k == 0)
			{
				if (i == -1 || i == row || m == -1 || m == line)cout << "##";
				else if (background[i][m] == 0) cout << "  ";
				else if (background[i][m] == 1) cout << "[]";
				else cout << "NA";
			}
		}
		cout << endl;
	}
}

int main()
{
	srand((unsigned)time(NULL));
	cout << "press ENTER to start the game!";
	cin >> cheat;
	if (cheat == 'h')
	{
		cout << "input level number: ";
		cin >> level;
	}
	generate();
	blockload();
	printer();
	while (flag)
	{
		if (GetAsyncKeyState(65) || GetAsyncKeyState(37)) {gk = 'a'; gks = 1;}
		else if (GetAsyncKeyState(68) || GetAsyncKeyState(39)) {gk = 'd'; gks = 1;}
		else if (GetAsyncKeyState(83) || GetAsyncKeyState(40)) {gk = 's'; gks = 1;}
		else if (GetAsyncKeyState(87) || GetAsyncKeyState(38)) {gk = 'w'; gks = 1;}
		else if (GetAsyncKeyState(32)) {q = 1; gks = 0;}
		else
			gks = 0;
		if(gks == 0)
			gk = 'v';
		if (plevel < level)
		{
			slpt -= 200;
		}
		blocker();
		checkgame();
		printer();
		if (q == 1)
		{
			cout << "press ENTER to continue";
			while (getchar() != '\n')
				Sleep(30);
			q = 0;
		}
		Sleep(slpt);
	}
	cout << "Game Over" << endl;
	cin >> i;
}



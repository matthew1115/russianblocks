#include <bits/stdc++.h>
#include <Windows.h>
using namespace std;

const int row = 32;
const int line = 25;
bool handycaps = 1;		//disable or enable handycap
int handycapn = 90;		//number of handycap
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
block a[5];		//use a[1] to a[4] , a[0] is not used
bool needload = 0,gks = 0;
int background[row + 1][line + 1], dump[row + 1][line + 1];
int i, n, m;
int q, k, type, flag = 1, fulline = 0;
char gk;
int clearline;

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
	if(slpt>100)
		slpt-=5;
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
		for(i=1;i<=4;++i) a[i].y++;
}

void leftblock()
{
	bool ff=1;
	for(i=1;i<=4;++i)
		if(background[a[i].x-1][a[i].y]==1||a[i].x==0)		//check left
			ff=0;
	if(ff)
		for(i=1;i<=4;++i) a[i].x--;
}

void rightblock()
{
	bool ff=1;
	for(i=1;i<=4;++i)
		if(background[a[i].x+1][a[i].y]==1||a[i].x==row-1)		//check right
			ff=0;
	if(ff)
		for(i=1;i<=4;++i) a[i].x++;
}

void solid()
{
	background[a[1].x][a[1].y] = 1;
	background[a[2].x][a[2].y] = 1;
	background[a[3].x][a[3].y] = 1;
	background[a[4].x][a[4].y] = 1;

}

void blockload()
{
	int s,t=rand()%18+1;
	type = t;				//debug use
	Sleep(500);
	if(t==0)//****
	{
		s=rand()%(row-4)+1;
		for(int i=1;i<=4;++i)
			a[i].x=s+i-1,a[i].y=1;
	}
	else if(t==1)	//***
	{				//*
		s = rand() % (row - 3) + 1;
		for(int i=1;i<=3;++i)
			a[i].x=s+i-1,a[i].y=1;
		a[4].x=s;a[4].y=2;
	}
	else if(t==2)	//***
	{				// *
		s = rand() % (row - 3) + 1;
		for(int i=1;i<=3;++i)
			a[i].x=s+i-1,a[i].y=1;
		a[4].x=s+1;a[4].y=2;
	}
	else if(t==3)	//***
	{				//  *
		s = rand() % (row - 3) + 1;
		for(int i=1;i<=3;++i)
			a[i].x=s+i-1,a[i].y=1;
		a[4].x=s+2;a[4].y=2;
	}
	else if(t==4)	//*
	{				//***
		s = rand() % (row - 3) + 1;
		for(int i=1;i<=3;++i)
			a[i].x=s+i-1,a[i].y=2;
		a[4].x=s;a[4].y=1;
	}
	else if(t==5)	// *
	{				//***
		s = rand() % (row - 3) + 1;
		for(int i=1;i<=3;++i)
			a[i].x=s+i-1,a[i].y=2;
		a[4].x=s+1;a[4].y=1;
	}
	else if(t==6)	//  *
	{				//***
		s = rand() % (row - 3) + 1;
		for(int i=1;i<=3;++i)
			a[i].x=s+i-1,a[i].y=2;
		a[4].x=s+2;a[4].y=1;
	}
	else if(t==7)	//**
	{				// **
		s=rand()%(row-3)+1;
		a[1].y=a[2].y=1;a[1].x=s  ;a[2].x=s+1;
		a[3].y=a[4].y=2;a[3].x=s+1;a[4].x=s+2;
	}
	else if(t==8)	// **
	{				//**
		s=rand()%(row-3)+1;
		a[1].y=a[2].y=2;a[1].x=s  ;a[2].x=s+1;
		a[3].y=a[4].y=1;a[3].x=s+1;a[4].x=s+2;
	}
	else if(t==9)	//**
	{				//**
		s=rand()%(row-2)+1;
		a[1].y=a[2].y=1;a[1].x=s;a[2].x=s+1;
		a[3].y=a[4].y=2;a[3].x=s;a[4].x=s+1;
	}
	else if(t==10)								//*
	{											//*
		s=rand()%(row-1)+1;						//*
		for(i=1;i<=4;++i)						//*
			a[i].x=s,a[i].y=i;
	}
	else if(t==11)			//**
	{						// *
		s=rand()%(row-2)+1;	// *
		for(i=1;i<=3;++i)
			a[i].x=s+1,a[i].y=i;
		a[4].x=s;a[4].y=1;
	}
	else if(t==12)			// *
	{						//**
		s=rand()%(row-2)+1;	// *
		for(i=1;i<=3;++i)
			a[i].x=s+1,a[i].y=i;
		a[4].x=s;a[4].y=2;
	}
	else if(t==13)			// *
	{						// *
		s=rand()%(row-2)+1;	//**
		for(i=1;i<=3;++i)
			a[i].x=s+1,a[i].y=i;
		a[4].x=s;a[4].y=3;
	}
	else if(t==14)			//**
	{						//*
		s=rand()%(row-2)+1;	//*
		for(i=1;i<=3;++i)
			a[i].x=s,a[i].y=i;
		a[4].x=s+1;a[4].y=1;
	}
	else if(t==15)			//*
	{						//**
		s=rand()%(row-2)+1;	//*
		for(i=1;i<=3;++i)
			a[i].x=s,a[i].y=i;
		a[4].x=s+1;a[4].y=2;
	}
	else if(t==16)			//*
	{						//*
		s=rand()%(row-2)+1;	//**
		for(i=1;i<=3;++i)
			a[i].x=s,a[i].y=i;
		a[4].x=s+1;a[4].y=3;
	}
	else if(t==17)			//*
	{						//**
		s=rand()%(row-2)+1;	// *
		a[1].x=a[2].x=s  ;a[1].y=1;a[2].y=2;
		a[3].x=a[4].x=s+1;a[3].y=2;a[4].y=3;
	}
	else if(t==18)			// *
	{						//**
		s=rand()%(row-2)+1;	//*
		a[1].x=a[2].x=s+1;a[1].y=1;a[2].y=2;
		a[3].x=a[4].x=s  ;a[3].y=2;a[4].y=3;
	}
	needload = 0;
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
				else cout << "N";
			}
		}
		cout << endl;
	}
}

int main()
{
	srand((unsigned)time(NULL));
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
			gk = 'w';
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



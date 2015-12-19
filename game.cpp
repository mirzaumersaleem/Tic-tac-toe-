         /*
          Game author :Umer Saleem
         
         */
include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WindowFunc(HWND, UINT, WPARAM, LPARAM);
int displayboard(HDC hdc);
int displaypieces(HDC hdc, char board[9]);
int displaymove(HDC hdc, int space);
int detectwin(HWND hwnd, HDC hdc, char board[9]);

const char szWinName[]="MyWin";	/* name of window class */
const char pipe='|';
const char dash='-';
const char plus='+';

int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst, LPSTR lpszArgs, int nWinMode)
{
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wcl;

	/* Define a window class. */
	wcl.cbSize= sizeof(WNDCLASSEX);

	wcl.hInstance= hThisInst;		/* handle to this instance	*/
	wcl.lpszClassName= szWinName;	/* window class name		*/
	wcl.lpfnWndProc= WindowFunc;	/* window function			*/
	wcl.style= 0;					/* default style			*/

	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);	/* standard icon	*/
	wcl.hIconSm = LoadIcon(NULL, IDI_WINLOGO);		/* small icon		*/
	wcl.hCursor = LoadCursor(NULL, IDC_ARROW);		/* cursor style		*/

	wcl.lpszMenuName = NULL;	/* no menu						*/
	wcl.cbClsExtra = 0;			/* no extra information needed	*/
	wcl.cbWndExtra = 0;			/* no extra information needed	*/

	wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH); 	/* Make the window background white	*/


	/* Register the window class	*/
	if(!RegisterClassEx(&wcl)) return 0;

	/* Now that a window class has been registered, a window can be created */
	hwnd = CreateWindow(
		szWinName,				/* name of window class						*/
		"Tic Tac Toe",			/* title									*/
		WS_OVERLAPPEDWINDOW&~WS_SIZEBOX,	/* window style					*/
		(GetSystemMetrics(SM_CXSCREEN)/2)-160,			/* X coordinate - default	*/
		(GetSystemMetrics(SM_CYSCREEN)/2)-170,			/* Y coordinate - default	*/
		320,					/* width									*/
		340,					/* height									*/
		HWND_DESKTOP,			/* no parent window							*/
		NULL,					/* no menu									*/
		hThisInst,				/* handle of this instance of the program	*/
		NULL					/* no additional arguments					*/
	);

	/* Display the Window		*/
	ShowWindow(hwnd, nWinMode);
	UpdateWindow(hwnd);

	/* Create the message loop.	*/
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);	/* translate keyboard messages	*/
		DispatchMessage(&msg);	/* return control to Windows	*/
	}
	return msg.wParam;
}

/*	This function is called by Windows and is passed
	messages from the message queue.
*/

LRESULT CALLBACK WindowFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT paintstruct;
	static char board[9]={' ',' ',' ',' ',' ',' ',' ',' ',' '};
	static int space=4;
	static char turn='X';

	switch(message)	{
		case WM_CHAR:
			if ((char)wParam==13)
			{
				if (board[space]==' ')
				{
					board[space]=turn;
					if (turn=='X')
						turn='O';
					else
						turn='X';
					InvalidateRect(hwnd,NULL,0);
				}
			}
		case WM_KEYDOWN:			/* process keystroke			*/
			switch((char)wParam){
			case VK_UP:
				
				space-=3;
				if (space<0)
					space+=9;
				InvalidateRect(hwnd,NULL,0);
				break;
			case VK_DOWN:
				space+=3;
				if (space>8)
					space-=9;
				InvalidateRect(hwnd,NULL,0);
				break;
			case VK_LEFT:
				space-=1;
				if (space<0)
					space+=9;
				InvalidateRect(hwnd,NULL,0);
				break;
			case VK_RIGHT:
				space+=1;
				if (space>8)
					space-=9;
				InvalidateRect(hwnd,NULL,0);
				break;
			}
		case WM_PAINT:			/* process a repaint request	*/
			hdc = BeginPaint(hwnd, &paintstruct);	/* get DC			*/
			displayboard(hdc);
			displaypieces(hdc,board);
			displaymove(hdc,space);
			switch(detectwin(hwnd,hdc,board))
			{
				case IDYES:
				{
					for(int t=0;t<9;t++)
						board[t]=' ';
					space=4;
					turn='X';
					InvalidateRect(hwnd,NULL,0);
					break;
				}
				case IDNO:
				{
					PostQuitMessage(0);
					break;
				}
			}			
			EndPaint(hwnd, &paintstruct);			/* release DC		*/
			break;
		case WM_LBUTTONDOWN:
			break;
		case WM_DESTROY:	/* terminate the program		*/
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

int displayboard(HDC hdc)
{
	char str[255];
	TextOut(hdc,100,10,"Tic Tac Toe",11);
	TextOut(hdc,100,30,"Version 1.0",11);
	TextOut(hdc,100,60,&pipe,1);
	TextOut(hdc,200,60,&pipe,1);
	TextOut(hdc,100,80,&pipe,1);
	TextOut(hdc,200,80,&pipe,1);
	TextOut(hdc,100,100,&pipe,1);
	TextOut(hdc,200,100,&pipe,1);
	sprintf(str,"------------------------------------------------------");
	TextOut(hdc,20,120,str,strlen(str));
	TextOut(hdc,100,140,&pipe,1);
	TextOut(hdc,200,140,&pipe,1);
	TextOut(hdc,100,160,&pipe,1);
	TextOut(hdc,200,160,&pipe,1);
	TextOut(hdc,100,180,&pipe,1);
	TextOut(hdc,200,180,&pipe,1);
	TextOut(hdc,20,200,str,strlen(str));
	TextOut(hdc,100,220,&pipe,1);
	TextOut(hdc,200,220,&pipe,1);
	TextOut(hdc,100,240,&pipe,1);
	TextOut(hdc,200,240,&pipe,1);
	TextOut(hdc,100,260,&pipe,1);
	TextOut(hdc,200,260,&pipe,1);
	return 0;
}

int displaypieces(HDC hdc, char board[9])
{
	int t=0;
	int x=0;
	int y=0;
	char str;

	for(t=0;t<9;t++)
	{
		x=50+100*(t%3);
		y=80+((int)(t/3))*80;
		str=board[t];
		TextOut(hdc,x,y,&str,1);
		if (board[t]==' ')
			for(int u=1;u<10;u++)
				TextOut(hdc,x+u,y,&str,1);
	}
	return 0;
}

int displaymove(HDC hdc, int space)
{
	int x=0;
	int y=0;
	char str=' ';
	for(int t=0;t<9;t++)\
	{
		x=100*(t%3);
		y=80*((int)(t/3));
		for(int u=10;u<99;u++)
		{
			TextOut(hdc,x+u,y+60,&str,1);
			TextOut(hdc,x+u,y+100,&str,1);
		}
		for( int k=1;k<9;k++)
		{
			TextOut(hdc,x+90+k,y+80,&str,1);
			TextOut(hdc,x+10+k,y+80,&str,1);
		}

	}
	str='*';
	x=100*(space%3);
	y=80*((int)(space/3));
	for(int u=10;u<91;u+=10)
	{
		TextOut(hdc,x+u,y+60,&str,1);
		TextOut(hdc,x+u,y+100,&str,1);
	}
	TextOut(hdc,x+90,y+80,&str,1);
	TextOut(hdc,x+10,y+80,&str,1);

	return 0;
}

int detectwin(HWND hwnd, HDC hdc, char board[9])
{
	char str[255];
	/*horizontal ones*/
	if (board[0]==board[1]&&board[1]==board[2]&&board[0]!=' ')
	{
		sprintf(str,"%c wins!",board[0]);
		return MessageBox(hwnd,"Play again?",str,MB_YESNO|MB_ICONQUESTION);
	}
	if (board[3]==board[4]&&board[4]==board[5]&&board[3]!=' ')
	{
		sprintf(str,"%c wins!",board[3]);
		return MessageBox(hwnd,"Play again?",str,MB_YESNO|MB_ICONQUESTION);
	}
	if (board[6]==board[7]&&board[7]==board[8]&&board[6]!=' ')
	{
		sprintf(str,"%c wins!",board[6]);
		return MessageBox(hwnd,"Play again?",str,MB_YESNO|MB_ICONQUESTION);
	}
	/* vertical ones now*/
	if (board[0]==board[3]&&board[3]==board[6]&&board[0]!=' ')
	{
		sprintf(str,"%c wins!",board[0]);
		return MessageBox(hwnd,"Play again?",str,MB_YESNO|MB_ICONQUESTION);
	}
	if (board[1]==board[4]&&board[4]==board[7]&&board[1]!=' ')
	{
		sprintf(str,"%c wins!",board[1]);
		return MessageBox(hwnd,"Play again?",str,MB_YESNO|MB_ICONQUESTION);
	}
	if (board[2]==board[5]&&board[5]==board[8]&&board[2]!=' ')
	{
		sprintf(str,"%c wins!",board[2]);
		return MessageBox(hwnd,"Play again?",str,MB_YESNO|MB_ICONQUESTION);
	}
	/*diagonals*/
	if (board[0]==board[4]&&board[4]==board[8]&&board[0]!=' ')
	{
		sprintf(str,"%c wins!",board[0]);
		return MessageBox(hwnd,"Play again?",str,MB_YESNO|MB_ICONQUESTION);
	}
	if (board[2]==board[4]&&board[4]==board[6]&&board[2]!=' ')
	{
		sprintf(str,"%c wins!",board[2]);
		return MessageBox(hwnd,"Play again?",str,MB_YESNO|MB_ICONQUESTION);
	}
	/* make sure board isn't full */
	for(int t=0;t<9;t++)
		if (board[t]==' ')
			return 0;
	return MessageBox(hwnd,"Play again?","Draw!",MB_YESNO|MB_ICONQUESTION); /*board is full*/
}

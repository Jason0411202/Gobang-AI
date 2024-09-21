#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#define X 1
#define O 2

using namespace std;
/*
instructions:

w,a,s,d:picking the drop point
q:regret
b:Developer Mode

l(lower L):enter/leave Layout mode
1:drop O in the layout mode
2:drop X in the layout mode
3:remove this chess in the layout mode

*/

/*
操作說明:

w,a,s,d:選取落點
q:悔棋
b:開發人員模式

l(小寫的L):進入/離開佈局模式
1:在佈局模式中，佈下O
2:在佈局模式中，佈下X
3:在佈局模式中，移除該子

*/

const int chessboard_Width=15;//棋盤寬

const int computer_Take=O;

const int gamer_Take=X;

int close_Filter_1=1;//是否關掉filter 1的判斷,1為關掉

double filter_2_Strict=0.6;//第二種過濾方法的嚴格程度,0-1間,愈大愈嚴格

double filter_2_Strict_For_Recursive=0.99;//第二種過濾方法的嚴格程度,0-1間,愈大愈嚴格，專門用來提升遞迴效率

int open_filter_3_Step=7;//第幾步之後才使用filter 3判斷

int close_filter_3_Step=205;//第幾步之後關掉filter 3判斷

const int filter_3_When_Win=3000;

const int filter_3_No_Win_No_Lose=2000;

const int filter_3_When_Lose=1000;

const int filter_3_When_Attack=-1;

const int filter_3_When_Defend=-10;

int recursive_Step_times=16;//預測未來的幾步

int difficult=3;
//設定各類型的樣本個數
const int typ1_Data_Amount=2;
const int typ2_Data_Amount=6;
const int typ3_Data_Amount=32;
const int typ4a_Data_Amount=6;
const int typ4b_Data_Amount=6;
const int typ5a_Data_Amount=14;
const int typ5b_Data_Amount=4;
const int typ6_Data_Amount=5;
const int typ7_Data_Amount=5;

const int typ11_Data_Amount=2;
const int typ12_Data_Amount=6;
const int typ13_Data_Amount=32;
const int typ14a_Data_Amount=6;
const int typ14b_Data_Amount=6;
const int typ15a_Data_Amount=14;
const int typ15b_Data_Amount=4;
const int typ16_Data_Amount=5;
const int typ17_Data_Amount=5;

//設定各類型的權重

//    類型:     7  6  5.a  5.b  4.a  4.b  3  2  1   敵7  敵6  敵5.a  敵5.b  敵4.a  敵4.b  敵3  敵2 敵1
//
//Type[i]中的i: 9  8   7    6    5    4   3  2  1    19   18    17     16    15     14    13   12   11
const int type1_Weight=30;
const int type2_Weight=115;
const int type3_Weight=1000;
const int type4a_Weight=2220;
const int type4b_Weight=2420;
const int type5a_Weight=2999;
const int type5b_Weight=3000;
const int type6_Weight=8192;
const int type7_Weight=16400;

const int type11_Weight=15;
const int type12_Weight=100;
const int type13_Weight=150;
const int type14a_Weight=1100;
const int type14b_Weight=1200;
const int type15a_Weight=2799;
const int type15b_Weight=2800;
const int type16_Weight=4850;
const int type17_Weight=12000;
/*
0:空位
3:任意
O:O
X:X
*/
int type1_Data[typ1_Data_Amount][9]=//死二
{{3,3,3,O,0,0,0,0,3},
 {3,0,0,0,0,O,3,3,3}};

int type2_Data[typ2_Data_Amount][9]=//活二
{{3,3,0,O,0,0,0,3,3},
 {3,0,0,O,0,0,3,3,3},
 {3,0,O,0,0,0,3,3,3},
 {3,3,0,0,0,O,0,3,3},
 {3,3,3,0,0,O,0,0,3},
 {3,3,3,0,0,0,O,0,3}};

int type3_Data[typ3_Data_Amount][9]=//死三
{{3,O,O,0,0,0,3,3,3},
 {3,O,0,O,0,0,3,3,3},
 {3,O,0,0,0,O,3,3,3},
 {3,3,O,O,0,0,0,3,3},
 {3,3,O,0,0,O,0,3,3},
 {3,3,O,0,0,0,O,3,3},
 {3,3,3,O,0,O,0,0,3},
 {3,3,3,O,0,0,O,0,3},
 {3,3,3,O,0,0,0,O,3},
 {3,3,3,3,0,O,O,0,0},
 {3,3,3,3,0,O,0,O,0},
 {3,3,3,3,0,O,0,0,O},
 {3,3,3,3,0,0,O,O,0},
 {3,3,3,3,0,0,O,0,O},
 {3,3,3,3,0,0,0,O,O},
 {3,3,3,0,0,0,O,O,3},
 {3,3,3,0,0,O,0,O,3},
 {3,3,3,O,0,0,0,O,3},
 {3,3,0,0,0,O,O,3,3},
 {3,3,0,O,0,0,O,3,3},
 {3,3,O,0,0,0,O,3,3},
 {3,0,0,O,0,O,3,3,3},
 {3,0,O,0,0,O,3,3,3},
 {3,O,0,0,0,O,3,3,3},
 {0,0,O,O,0,3,3,3,3},
 {0,O,0,O,0,3,3,3,3},
 {O,0,0,O,0,3,3,3,3},
 {0,O,O,0,0,3,3,3,3},
 {O,0,O,0,0,3,3,3,3},
 {O,O,0,0,0,3,3,3,3},
 {3,3,3,3,0,0,O,0,O},
 {O,0,O,0,0,3,3,3,3}};

int type4a_Data[typ4a_Data_Amount][9]=//活三a
{
 {3,0,O,O,0,0,0,3,3},
 {0,0,O,O,0,0,3,3,3},
 {3,0,0,O,0,O,0,3,3},
 {3,3,0,O,0,O,0,0,3},
 {3,3,0,0,0,O,O,0,3},
 {3,3,3,0,0,O,O,0,0}};

 int type4b_Data[typ4b_Data_Amount][9]=//活三b
{{0,O,O,0,0,0,3,3,3},
 {0,O,0,O,0,0,3,3,3},
 {3,0,O,0,0,O,0,3,3},
 {3,3,0,O,0,0,O,0,3},
 {3,3,3,0,0,O,0,O,0},
 {3,3,3,0,0,0,O,O,0}};

 int type5a_Data[typ5a_Data_Amount][9]=//死四a
{{3,O,O,O,0,0,3,3,3},
 {O,0,O,O,0,3,3,3,3},
 {O,O,0,O,0,3,3,3,3},
 {3,3,3,O,0,O,O,0,3},
 {3,3,0,O,0,O,O,3,3},
 {0,O,O,O,0,3,3,3,3},
 {3,3,O,0,0,O,O,3,3},
 {3,3,3,0,0,O,O,O,3},
 {3,3,3,3,0,O,O,0,O},
 {3,3,3,3,0,O,0,O,O},
 {3,0,O,O,0,O,3,3,3},
 {3,3,O,O,0,O,0,3,3},
 {3,3,3,3,0,O,O,O,0},
 {3,3,O,O,0,0,O,3,3}};

int type5b_Data[typ5b_Data_Amount][9]=//死四b
{{O,O,O,0,0,3,3,3,3},
 {3,3,3,O,0,0,O,O,3},
 {3,3,3,3,0,0,O,O,O},
 {3,O,O,0,0,O,3,3,3}};

int type6_Data[typ6_Data_Amount][9]=//活四
{{0,O,O,O,0,0,3,3,3},
 {3,3,3,0,0,O,O,O,0},
 {3,3,0,O,0,O,O,0,3},
 {3,0,O,O,0,O,0,3,3},
 {3,O,0,O,0,O,0,O,3}};

int type7_Data[typ7_Data_Amount][9]=//活五
{{O,O,O,O,0,3,3,3,3},
 {3,O,O,O,0,O,3,3,3},
 {3,3,O,O,0,O,O,3,3},
 {3,3,3,O,0,O,O,O,3},
 {3,3,3,3,0,O,O,O,O}};

int type11_Data[typ11_Data_Amount][9]=//敵死二
{{3,3,3,X,0,0,0,0,3},
 {3,0,0,0,0,X,3,3,3}};

int type12_Data[typ12_Data_Amount][9]=//敵活二
{{3,3,0,X,0,0,0,3,3},
 {3,0,0,X,0,0,3,3,3},
 {3,0,X,0,0,0,3,3,3},
 {3,3,0,0,0,X,0,3,3},
 {3,3,3,0,0,X,0,0,3},
 {3,3,3,0,0,0,X,0,3}};

int type13_Data[typ13_Data_Amount][9]=//敵死三
{{3,X,X,0,0,0,3,3,3},
 {3,X,0,X,0,0,3,3,3},
 {3,X,0,0,0,X,3,3,3},
 {3,3,X,X,0,0,0,3,3},
 {3,3,X,0,0,X,0,3,3},
 {3,3,X,0,0,0,X,3,3},
 {3,3,3,X,0,X,0,0,3},
 {3,3,3,X,0,0,X,0,3},
 {3,3,3,X,0,0,0,X,3},
 {3,3,3,3,0,X,X,0,0},
 {3,3,3,3,0,X,0,X,0},
 {3,3,3,3,0,X,0,0,X},
 {3,3,3,3,0,0,X,X,0},
 {3,3,3,3,0,0,X,0,X},
 {3,3,3,3,0,0,0,X,X},
 {3,3,3,0,0,0,X,X,3},
 {3,3,3,0,0,X,0,X,3},
 {3,3,3,X,0,0,0,X,3},
 {3,3,0,0,0,X,X,3,3},
 {3,3,0,X,0,0,X,3,3},
 {3,3,X,0,0,0,X,3,3},
 {3,0,0,X,0,X,3,3,3},
 {3,0,X,0,0,X,3,3,3},
 {3,X,0,0,0,X,3,3,3},
 {0,0,X,X,0,3,3,3,3},
 {0,X,0,X,0,3,3,3,3},
 {X,0,0,X,0,3,3,3,3},
 {0,X,X,0,0,3,3,3,3},
 {X,0,X,0,0,3,3,3,3},
 {X,X,0,0,0,3,3,3,3},
 {3,3,3,3,0,0,X,0,X},
 {X,0,X,0,0,3,3,3,3}};

int type14a_Data[typ14a_Data_Amount][9]=//敵活三a
{
 {3,0,X,X,0,0,0,3,3},
 {0,0,X,X,0,0,3,3,3},
 {3,0,0,X,0,X,0,3,3},
 {3,3,0,X,0,X,0,0,3},
 {3,3,0,0,0,X,X,0,3},
 {3,3,3,0,0,X,X,0,0}};

int type14b_Data[typ14b_Data_Amount][9]=//敵活三b
{{0,X,X,0,0,0,3,3,3},
 {0,X,0,X,0,0,3,3,3},
 {3,0,X,0,0,X,0,3,3},
 {3,3,0,X,0,0,X,0,3},
 {3,3,3,0,0,X,0,X,0},
 {3,3,3,0,0,0,X,X,0}};

int type15a_Data[typ15a_Data_Amount][9]=//敵死四a
{{3,X,X,X,0,0,3,3,3},
 {X,0,X,X,0,3,3,3,3},
 {X,X,0,X,0,3,3,3,3},
 {3,3,3,X,0,X,X,0,3},
 {3,3,0,X,0,X,X,3,3},
 {0,X,X,X,0,3,3,3,3},
 {3,3,X,0,0,X,X,3,3},
 {3,3,3,0,0,X,X,X,3},
 {3,3,3,3,0,X,X,0,X},
 {3,3,3,3,0,X,0,X,X},
 {3,0,X,X,0,X,3,3,3},
 {3,3,X,X,0,X,0,3,3},
 {3,3,3,3,0,X,X,X,0},
 {3,3,X,X,0,0,X,3,3}};

 int type15b_Data[typ15b_Data_Amount][9]=//敵死四b
{{X,X,X,0,0,3,3,3,3},
 {3,3,3,X,0,0,X,X,3},
 {3,3,3,3,0,0,X,X,X},
 {3,X,X,0,0,X,3,3,3}};

int type16_Data[typ16_Data_Amount][9]=//敵活四
{{0,X,X,X,0,0,3,3,3},
 {3,3,3,0,0,X,X,X,0},
 {3,3,0,X,0,X,X,0,3},
 {3,0,X,X,0,X,0,3,3},
 {3,X,0,X,0,X,0,X,3}};

int type17_Data[typ17_Data_Amount][9]=//敵活五
{{X,X,X,X,0,3,3,3,3},
 {3,X,X,X,0,X,3,3,3},
 {3,3,X,X,0,X,X,3,3},
 {3,3,3,X,0,X,X,X,3},
 {3,3,3,3,0,X,X,X,X}};

struct Location
{
    int x;
    int y;
};

int checkerboard[chessboard_Width][chessboard_Width] =
        {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

int win_Chance_1[chessboard_Width][chessboard_Width] =
        {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

int win_Chance_2[chessboard_Width][chessboard_Width] =
        {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

int win_Chance_3[chessboard_Width][chessboard_Width] =
        {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
         {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

int st[]={0,0};
int step_Timer=1;
int all_counter;
int rand_x,rand_y;//第一子的位置
int is_Chess_manual;
int winning_times;
int future_Total_Steps=0;
int total_Future_Total_Step=0;
int filter_2_Max_Value;
void SetColor(int color = 7)
{
  HANDLE hConsole;
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole,color);
}
void gotoxy(int x,int y)
{
    COORD pos={x,y};
    HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut,pos);
}
void HideCursor()//隱藏光標
{
	CONSOLE_CURSOR_INFO cursor_info={1,0};
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}
Location tran(int position,int shape,int x,int y)
{
    Location temp;
    if(shape==1)//水平
    {
        temp.x=x;
        temp.y=y-4+position;
    }
    else if(shape==2)//垂直
    {
        temp.x=x-4+position;
        temp.y=y;
    }
    else if(shape==3)//斜 "/"
    {
        temp.x=x+4-position;
        temp.y=y-4+position;
    }
    else if(shape==4)//斜 "\"
    {
        temp.x=x-4+position;
        temp.y=y-4+position;
    }
    return temp;
}
int IsOutside(int x,int y)
{
    if(x<0 || y<0 || x>chessboard_Width-1 || y>chessboard_Width-1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
int Judge_Type(int x,int y,int shape)
{
    int paring_fail;
    int Loc_x,Loc_y;

    for(int i=0;i<typ7_Data_Amount;i++)//type 7(複製要改五個地方)
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type7_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type7_Data[i][j] || type7_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 9;
        }
    }

    for(int i=0;i<typ6_Data_Amount;i++)//type 6
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type6_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type6_Data[i][j] || type6_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 8;
        }
    }

    for(int i=0;i<typ5a_Data_Amount;i++)//type 5a
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type5a_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type5a_Data[i][j] || type5a_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 7;
        }
    }

    for(int i=0;i<typ5b_Data_Amount;i++)//type 5b
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type5b_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type5b_Data[i][j] || type5b_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 6;
        }
    }

    for(int i=0;i<typ4a_Data_Amount;i++)//type 4a
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type4a_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type4a_Data[i][j] || type4a_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 5;
        }
    }

    for(int i=0;i<typ4b_Data_Amount;i++)//type 4b
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type4b_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type4b_Data[i][j] || type4b_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 4;
        }
    }

    for(int i=0;i<typ3_Data_Amount;i++)//type 3
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type3_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type3_Data[i][j] || type3_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 3;
        }
    }

    for(int i=0;i<typ2_Data_Amount;i++)//type2
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type2_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type2_Data[i][j] || type2_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 2;
        }
    }


    for(int i=0;i<typ1_Data_Amount;i++)//type 1
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type1_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type1_Data[i][j] || type1_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 1;
        }
    }
    return 0;
}

int Judge_Defend_Type(int x,int y,int shape)
{
    int paring_fail;
    int Loc_x,Loc_y;

    for(int i=0;i<typ17_Data_Amount;i++)//type 17
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type17_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type17_Data[i][j] || type17_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 19;
        }
    }

    for(int i=0;i<typ16_Data_Amount;i++)//type 16
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type16_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type16_Data[i][j] || type16_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 18;
        }
    }

    for(int i=0;i<typ15a_Data_Amount;i++)//type 15a
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type15a_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type15a_Data[i][j] || type15a_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 17;
        }
    }

    for(int i=0;i<typ15b_Data_Amount;i++)//type 15b
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type15b_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type15b_Data[i][j] || type15b_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 16;
        }
    }

    for(int i=0;i<typ14a_Data_Amount;i++)//type 14a
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type14a_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type14a_Data[i][j] || type14a_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 15;
        }
    }

    for(int i=0;i<typ14b_Data_Amount;i++)//type 14b
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type14b_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type14b_Data[i][j] || type14b_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 14;
        }
    }

    for(int i=0;i<typ13_Data_Amount;i++)//type 13
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type13_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type13_Data[i][j] || type13_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 13;
        }
    }

    for(int i=0;i<typ12_Data_Amount;i++)//type 12
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type12_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type12_Data[i][j] || type12_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 12;
        }
    }

    for(int i=0;i<typ11_Data_Amount;i++)//type 11
    {
        paring_fail=0;
        for(int j=0;j<9;j++)
        {
            Loc_x=tran(j,shape,x,y).x;
            Loc_y=tran(j,shape,x,y).y;
            if(IsOutside(Loc_x,Loc_y)==1)
            {
                if(type11_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
            else
            {
                if(checkerboard[Loc_x][Loc_y]==type11_Data[i][j] || type11_Data[i][j]==3)
                {
                    continue;
                }
                else
                {
                    paring_fail=1;
                    break;
                }
            }
        }
        if(paring_fail==0)
        {
            return 11;
        }
    }

    return 0;
}
Location cvt(int x_move,int y_move,int type,int Notmirror)
{
    Location temp;
    if(type==1)
    {
        temp.x=rand_x-y_move;
        temp.y=rand_y+x_move;
    }
    else if(type==2)
    {
        temp.x=rand_x+x_move;
        temp.y=rand_y+y_move;
    }
    else if(type==3)
    {
        temp.x=rand_x+y_move;
        temp.y=rand_y-x_move;
    }
    else if(type==4)
    {
        temp.x=rand_x-x_move;
        temp.y=rand_y-y_move;
    }

    if(Notmirror==2)
    {
        temp.y=2*rand_y-temp.y;
    }
    return temp;
}
int database_Step1(int x,int y)
{
    if(x==rand_x && y==rand_y)
    {
        return 1;
    }
    return 0;
}
int database_Step3(int x,int y)
{
    /*

        X
        O

    */
    for(int j=1;j<=2;j++)
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==X)//設定局型
            {
                if(x==cvt(-1,1,i,j).x && y==cvt(-1,1,i,j).y)//設定可走的位置 //花月
                {
                    return 1;
                }
                if(x==cvt(1,1,i,j).x && y==cvt(1,1,i,j).y)//花月
                {
                    return 1;
                }
                if(x==cvt(1,0,i,j).x && y==cvt(1,0,i,j).y)//雨月
                {
                    return 1;
                }
                if(x==cvt(-1,0,i,j).x && y==cvt(-1,0,i,j).y)//雨月
                {
                    return 1;
                }
                if(x==cvt(2,-1,i,j).x && y==cvt(2,-1,i,j).y)//新月
                {
                    return 1;
                }
                if(x==cvt(-2,-1,i,j).x && y==cvt(-2,-1,i,j).y)//新月
                {
                    return 1;
                }
            }
        }
    }


    /*

          X
        O

    */
    for(int j=1;j<=2;j++)
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(1,1,i,j).x][cvt(1,1,i,j).y]==X)//設定局型
            {
                if(x==cvt(-1,1,i,j).x && y==cvt(-1,1,i,j).y)//設定可走的位置 //浦月
                {
                    return 1;
                }
                if(x==cvt(1,-1,i,j).x && y==cvt(1,-1,i,j).y)//浦月
                {
                    return 1;
                }
                if(x==cvt(2,-1,i,j).x && y==cvt(2,-1,i,j).y)//水月
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}
int database_Step5(int x,int y)
{
    /*

      X   X
        O
          O

    */
    for(int j=1;j<=2;j++)
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(1,1,i,j).x][cvt(1,1,i,j).y]==X && checkerboard[cvt(1,-1,i,j).x][cvt(1,-1,i,j).y]==O && checkerboard[cvt(-1,1,i,j).x][cvt(-1,1,i,j).y]==X)//設定局型
            {
                if(x==cvt(0,1,i,j).x && y==cvt(0,1,i,j).y)//設定可走的位置
                {
                    return 1;
                }
            }
        }
    }

    /*

      O X
      X O

    */
    for(int j=1;j<=2;j++)
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==X && checkerboard[cvt(-1,0,i,j).x][cvt(-1,0,i,j).y]==X && checkerboard[cvt(-1,1,i,j).x][cvt(-1,1,i,j).y]==O)//設定局型
            {
                if(x==cvt(1,2,i,j).x && y==cvt(1,2,i,j).y)//設定可走的位置
                {
                    return 1;
                }
                if(x==cvt(-2,-1,i,j).x && y==cvt(-2,-1,i,j).y)//設定可走的位置
                {
                    return 1;
                }
            }
        }
    }

    /*

        X
        O O X

    */
    for(int j=1;j<=2;j++)//雨月變化
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==X && checkerboard[cvt(1,0,i,j).x][cvt(1,0,i,j).y]==O && checkerboard[cvt(2,0,i,j).x][cvt(2,0,i,j).y]==X)//設定局型
            {
                if(x==cvt(1,-1,i,j).x && y==cvt(1,-1,i,j).y)//設定可走的位置
                {
                    return 1;
                }
            }
        }
    }

    /*

        X
      X O
            O

    */
    for(int j=1;j<=2;j++)//新月變化
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==X && checkerboard[cvt(2,-1,i,j).x][cvt(2,-1,i,j).y]==O && checkerboard[cvt(-1,0,i,j).x][cvt(-1,0,i,j).y]==X)//設定局型
            {
                if(x==cvt(1,2,i,j).x && y==cvt(1,2,i,j).y)//設定可走的位置
                {
                    return 1;
                }
                if(x==cvt(-2,-1,i,j).x && y==cvt(-2,-1,i,j).y)
                {
                    return 1;
                }
            }
        }
    }

    /*

      X X
        O
            O

    */
    for(int j=1;j<=2;j++)//新月變化
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==X && checkerboard[cvt(2,-1,i,j).x][cvt(2,-1,i,j).y]==O && checkerboard[cvt(-1,1,i,j).x][cvt(-1,1,i,j).y]==X)//設定局型
            {
                if(x==cvt(2,0,i,j).x && y==cvt(2,0,i,j).y)//設定可走的位置
                {
                    return 1;
                }
                if(x==cvt(2,1,i,j).x && y==cvt(2,1,i,j).y)
                {
                    return 1;
                }
            }
        }
    }

    /*

      X   X
        O
            O

    */
    for(int j=1;j<=2;j++)//水月變化
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(1,1,i,j).x][cvt(1,1,i,j).y]==X && checkerboard[cvt(2,-1,i,j).x][cvt(2,-1,i,j).y]==O && checkerboard[cvt(-1,1,i,j).x][cvt(-1,1,i,j).y]==X)//設定局型
            {
                if(x==cvt(0,-1,i,j).x && y==cvt(0,-1,i,j).y)//設定可走的位置
                {
                    return 1;
                }
                if(x==cvt(1,-1,i,j).x && y==cvt(1,-1,i,j).y)
                {
                    return 1;
                }
                if(x==cvt(1,-2,i,j).x && y==cvt(1,-2,i,j).y)
                {
                    return 1;
                }
            }
        }
    }

    /*

          X
        O
            O
          X

    */
    for(int j=1;j<=2;j++)//水月變化
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(1,1,i,j).x][cvt(1,1,i,j).y]==X && checkerboard[cvt(2,-1,i,j).x][cvt(2,-1,i,j).y]==O && checkerboard[cvt(1,-2,i,j).x][cvt(1,-2,i,j).y]==X)//設定局型
            {
                if(x==cvt(1,0,i,j).x && y==cvt(1,0,i,j).y)//設定可走的位置
                {
                    return 1;
                }
                if(x==cvt(2,0,i,j).x && y==cvt(2,0,i,j).y)
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}
int database_Step11(int x,int y)
{
    /*

        X
      X O X
        O O O X
          O
        X

    */
    /*

        X
      X O X
      X O O O
          O
        X

    */

    for(int j=1;j<=2;j++)
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(1,1,i,j).x][cvt(1,1,i,j).y]==X && checkerboard[cvt(1,-1,i,j).x][cvt(1,-1,i,j).y]==O && checkerboard[cvt(-1,1,i,j).x][cvt(-1,1,i,j).y]==X &&
               checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==O && checkerboard[cvt(0,2,i,j).x][cvt(0,2,i,j).y]==X && checkerboard[cvt(0,-2,i,j).x][cvt(0,-2,i,j).y]==X &&
               checkerboard[cvt(2,0,i,j).x][cvt(2,0,i,j).y]==O && checkerboard[cvt(1,0,i,j).x][cvt(1,0,i,j).y]==O && (checkerboard[cvt(3,0,i,j).x][cvt(3,0,i,j).y]==X ||
               checkerboard[cvt(-1,0,i,j).x][cvt(-1,0,i,j).y]==X))//設定局型
            {
                if(x==cvt(2,-1,i,j).x && y==cvt(2,-1,i,j).y)//設定可走的位置
                {
                    return 1;
                }
            }
        }
    }



    return 0;
}
int database_Step13(int x,int y)
{
    /*

        X
      X O X
        O O O X
          O O
        X     X

    */


    for(int j=1;j<=2;j++)
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(1,1,i,j).x][cvt(1,1,i,j).y]==X && checkerboard[cvt(1,-1,i,j).x][cvt(1,-1,i,j).y]==O && checkerboard[cvt(-1,1,i,j).x][cvt(-1,1,i,j).y]==X &&
               checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==O && checkerboard[cvt(0,2,i,j).x][cvt(0,2,i,j).y]==X && checkerboard[cvt(0,-2,i,j).x][cvt(0,-2,i,j).y]==X &&
               checkerboard[cvt(2,0,i,j).x][cvt(2,0,i,j).y]==O && checkerboard[cvt(1,0,i,j).x][cvt(1,0,i,j).y]==O && checkerboard[cvt(3,0,i,j).x][cvt(3,0,i,j).y]==X &&
               checkerboard[cvt(2,-1,i,j).x][cvt(2,-1,i,j).y]==O && checkerboard[cvt(3,-2,i,j).x][cvt(3,-2,i,j).y]==X)//設定局型
            {
                if(x==cvt(0,-1,i,j).x && y==cvt(0,-1,i,j).y)//設定可走的位置
                {
                    return 1;
                }
            }
        }
    }

    /*

      X X
      X O X
        O O O X
          O O
        X

    */


    for(int j=1;j<=2;j++)
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(1,1,i,j).x][cvt(1,1,i,j).y]==X && checkerboard[cvt(1,-1,i,j).x][cvt(1,-1,i,j).y]==O && checkerboard[cvt(-1,1,i,j).x][cvt(-1,1,i,j).y]==X &&
               checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==O && checkerboard[cvt(0,2,i,j).x][cvt(0,2,i,j).y]==X && checkerboard[cvt(0,-2,i,j).x][cvt(0,-2,i,j).y]==X &&
               checkerboard[cvt(2,0,i,j).x][cvt(2,0,i,j).y]==O && checkerboard[cvt(1,0,i,j).x][cvt(1,0,i,j).y]==O && checkerboard[cvt(3,0,i,j).x][cvt(3,0,i,j).y]==X &&
               checkerboard[cvt(2,-1,i,j).x][cvt(2,-1,i,j).y]==O && checkerboard[cvt(-1,2,i,j).x][cvt(-1,2,i,j).y]==X)//設定局型
            {
                if(x==cvt(4,-1,i,j).x && y==cvt(4,-1,i,j).y)//設定可走的位置
                {
                    return 1;
                }
            }
        }
    }
    /*

        X
      X O X
      X O O O
          O O
        X     X

    */


    for(int j=1;j<=2;j++)
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(1,1,i,j).x][cvt(1,1,i,j).y]==X && checkerboard[cvt(1,-1,i,j).x][cvt(1,-1,i,j).y]==O && checkerboard[cvt(-1,1,i,j).x][cvt(-1,1,i,j).y]==X &&
               checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==O && checkerboard[cvt(0,2,i,j).x][cvt(0,2,i,j).y]==X && checkerboard[cvt(0,-2,i,j).x][cvt(0,-2,i,j).y]==X &&
               checkerboard[cvt(2,0,i,j).x][cvt(2,0,i,j).y]==O && checkerboard[cvt(1,0,i,j).x][cvt(1,0,i,j).y]==O && checkerboard[cvt(-1,0,i,j).x][cvt(-1,0,i,j).y]==X &&
               checkerboard[cvt(2,-1,i,j).x][cvt(2,-1,i,j).y]==O && checkerboard[cvt(3,-2,i,j).x][cvt(3,-2,i,j).y]==X)//設定局型
            {
                if(x==cvt(4,-1,i,j).x && y==cvt(4,-1,i,j).y)//設定可走的位置
                {
                    return 1;
                }
            }
        }
    }
    /*

      X X
      X O X
      X O O O
          O O
        X

    */


    for(int j=1;j<=2;j++)
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(1,1,i,j).x][cvt(1,1,i,j).y]==X && checkerboard[cvt(1,-1,i,j).x][cvt(1,-1,i,j).y]==O && checkerboard[cvt(-1,1,i,j).x][cvt(-1,1,i,j).y]==X &&
               checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==O && checkerboard[cvt(0,2,i,j).x][cvt(0,2,i,j).y]==X && checkerboard[cvt(0,-2,i,j).x][cvt(0,-2,i,j).y]==X &&
               checkerboard[cvt(2,0,i,j).x][cvt(2,0,i,j).y]==O && checkerboard[cvt(1,0,i,j).x][cvt(1,0,i,j).y]==O && checkerboard[cvt(-1,0,i,j).x][cvt(-1,0,i,j).y]==X &&
               checkerboard[cvt(2,-1,i,j).x][cvt(2,-1,i,j).y]==O && checkerboard[cvt(-1,2,i,j).x][cvt(-1,2,i,j).y]==X)//設定局型
            {
                if(x==cvt(-1,-1,i,j).x && y==cvt(-1,-1,i,j).y)//設定可走的位置
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}
int database_Step15(int x,int y)
{
    /*

        X
      X O X
        O O O X
        O O O X
        X     X

    */


    for(int j=1;j<=2;j++)
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(1,1,i,j).x][cvt(1,1,i,j).y]==X && checkerboard[cvt(1,-1,i,j).x][cvt(1,-1,i,j).y]==O && checkerboard[cvt(-1,1,i,j).x][cvt(-1,1,i,j).y]==X &&
               checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==O && checkerboard[cvt(0,2,i,j).x][cvt(0,2,i,j).y]==X && checkerboard[cvt(0,-2,i,j).x][cvt(0,-2,i,j).y]==X &&
               checkerboard[cvt(2,0,i,j).x][cvt(2,0,i,j).y]==O && checkerboard[cvt(1,0,i,j).x][cvt(1,0,i,j).y]==O && checkerboard[cvt(3,0,i,j).x][cvt(3,0,i,j).y]==X &&
               checkerboard[cvt(2,-1,i,j).x][cvt(2,-1,i,j).y]==O && checkerboard[cvt(3,-2,i,j).x][cvt(3,-2,i,j).y]==X && checkerboard[cvt(0,-1,i,j).x][cvt(0,-1,i,j).y]==O &&
               checkerboard[cvt(3,-1,i,j).x][cvt(3,-1,i,j).y]==X)//設定局型
            {
                if(x==cvt(-1,0,i,j).x && y==cvt(-1,0,i,j).y)//設定可走的位置
                {
                    return 1;
                }
            }
        }
    }
    /*

        X
      X O X
        O O O X
      X O O O
        X     X

    */


    for(int j=1;j<=2;j++)
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(1,1,i,j).x][cvt(1,1,i,j).y]==X && checkerboard[cvt(1,-1,i,j).x][cvt(1,-1,i,j).y]==O && checkerboard[cvt(-1,1,i,j).x][cvt(-1,1,i,j).y]==X &&
               checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==O && checkerboard[cvt(0,2,i,j).x][cvt(0,2,i,j).y]==X && checkerboard[cvt(0,-2,i,j).x][cvt(0,-2,i,j).y]==X &&
               checkerboard[cvt(2,0,i,j).x][cvt(2,0,i,j).y]==O && checkerboard[cvt(1,0,i,j).x][cvt(1,0,i,j).y]==O && checkerboard[cvt(3,0,i,j).x][cvt(3,0,i,j).y]==X &&
               checkerboard[cvt(2,-1,i,j).x][cvt(2,-1,i,j).y]==O && checkerboard[cvt(3,-2,i,j).x][cvt(3,-2,i,j).y]==X && checkerboard[cvt(0,-1,i,j).x][cvt(0,-1,i,j).y]==O &&
               checkerboard[cvt(-1,-1,i,j).x][cvt(-1,-1,i,j).y]==X)//設定局型
            {
                if(x==cvt(2,-3,i,j).x && y==cvt(2,-3,i,j).y)//設定可走的位置
                {
                    return 1;
                }
            }
        }
    }
    /*

      X X
      X O X
        O O O X
          O O X O
        X

    */


    for(int j=1;j<=2;j++)
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(1,1,i,j).x][cvt(1,1,i,j).y]==X && checkerboard[cvt(1,-1,i,j).x][cvt(1,-1,i,j).y]==O && checkerboard[cvt(-1,1,i,j).x][cvt(-1,1,i,j).y]==X &&
               checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==O && checkerboard[cvt(0,2,i,j).x][cvt(0,2,i,j).y]==X && checkerboard[cvt(0,-2,i,j).x][cvt(0,-2,i,j).y]==X &&
               checkerboard[cvt(2,0,i,j).x][cvt(2,0,i,j).y]==O && checkerboard[cvt(1,0,i,j).x][cvt(1,0,i,j).y]==O && checkerboard[cvt(3,0,i,j).x][cvt(3,0,i,j).y]==X &&
               checkerboard[cvt(2,-1,i,j).x][cvt(2,-1,i,j).y]==O && checkerboard[cvt(-1,2,i,j).x][cvt(-1,2,i,j).y]==X &&checkerboard[cvt(4,-1,i,j).x][cvt(4,-1,i,j).y]==O &&
               checkerboard[cvt(3,-1,i,j).x][cvt(3,-1,i,j).y]==X)//設定局型
            {
                if(x==cvt(2,-3,i,j).x && y==cvt(2,-3,i,j).y)//設定可走的位置
                {
                    return 1;
                }
            }
        }
    }
    /*

        X
      X O X
      X O O O
          O O X O
        X     X

    */


    for(int j=1;j<=2;j++)
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(1,1,i,j).x][cvt(1,1,i,j).y]==X && checkerboard[cvt(1,-1,i,j).x][cvt(1,-1,i,j).y]==O && checkerboard[cvt(-1,1,i,j).x][cvt(-1,1,i,j).y]==X &&
               checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==O && checkerboard[cvt(0,2,i,j).x][cvt(0,2,i,j).y]==X && checkerboard[cvt(0,-2,i,j).x][cvt(0,-2,i,j).y]==X &&
               checkerboard[cvt(2,0,i,j).x][cvt(2,0,i,j).y]==O && checkerboard[cvt(1,0,i,j).x][cvt(1,0,i,j).y]==O && checkerboard[cvt(-1,0,i,j).x][cvt(-1,0,i,j).y]==X &&
               checkerboard[cvt(2,-1,i,j).x][cvt(2,-1,i,j).y]==O && checkerboard[cvt(3,-2,i,j).x][cvt(3,-2,i,j).y]==X &&  checkerboard[cvt(3,-1,i,j).x][cvt(3,-1,i,j).y]==X &&
               checkerboard[cvt(4,-1,i,j).x][cvt(4,-1,i,j).y]==O)//設定局型
            {
                if(x==cvt(2,1,i,j).x && y==cvt(2,1,i,j).y)//設定可走的位置
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}
int database_Step17(int x,int y)
{
    /*

      X X
      X O X
      X O O O
      O X O O
        X   O
            X

    */


    for(int j=1;j<=2;j++)
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(1,1,i,j).x][cvt(1,1,i,j).y]==X && checkerboard[cvt(1,-1,i,j).x][cvt(1,-1,i,j).y]==O && checkerboard[cvt(-1,1,i,j).x][cvt(-1,1,i,j).y]==X &&
               checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==O && checkerboard[cvt(0,2,i,j).x][cvt(0,2,i,j).y]==X && checkerboard[cvt(0,-2,i,j).x][cvt(0,-2,i,j).y]==X &&
               checkerboard[cvt(2,0,i,j).x][cvt(2,0,i,j).y]==O && checkerboard[cvt(1,0,i,j).x][cvt(1,0,i,j).y]==O && checkerboard[cvt(-1,0,i,j).x][cvt(-1,0,i,j).y]==X &&
               checkerboard[cvt(2,-1,i,j).x][cvt(2,-1,i,j).y]==O && checkerboard[cvt(-1,2,i,j).x][cvt(-1,2,i,j).y]==X && checkerboard[cvt(-1,-1,i,j).x][cvt(-1,-1,i,j).y]==O &&
               checkerboard[cvt(0,-1,i,j).x][cvt(0,-1,i,j).y]==X && checkerboard[cvt(2,-2,i,j).x][cvt(2,-2,i,j).y]==O && (checkerboard[cvt(2,-3,i,j).x][cvt(2,-3,i,j).y]==X || checkerboard[cvt(2,1,i,j).x][cvt(2,1,i,j).y]==X))//設定局型
            {
                if(x==cvt(3,-3,i,j).x && y==cvt(3,-3,i,j).y)//設定可走的位置
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}
int database_Step19(int x,int y)
{
    /*

      X X
      X O X
      X O O O
      O X O O
        X   O
            X O
                X
    */


    for(int j=1;j<=2;j++)
    {
        for(int i=1;i<=4;i++)//四種方向
        {
            if(checkerboard[cvt(1,1,i,j).x][cvt(1,1,i,j).y]==X && checkerboard[cvt(1,-1,i,j).x][cvt(1,-1,i,j).y]==O && checkerboard[cvt(-1,1,i,j).x][cvt(-1,1,i,j).y]==X &&
               checkerboard[cvt(0,1,i,j).x][cvt(0,1,i,j).y]==O && checkerboard[cvt(0,2,i,j).x][cvt(0,2,i,j).y]==X && checkerboard[cvt(0,-2,i,j).x][cvt(0,-2,i,j).y]==X &&
               checkerboard[cvt(2,0,i,j).x][cvt(2,0,i,j).y]==O && checkerboard[cvt(1,0,i,j).x][cvt(1,0,i,j).y]==O && checkerboard[cvt(-1,0,i,j).x][cvt(-1,0,i,j).y]==X &&
               checkerboard[cvt(2,-1,i,j).x][cvt(2,-1,i,j).y]==O && checkerboard[cvt(-1,2,i,j).x][cvt(-1,2,i,j).y]==X && checkerboard[cvt(-1,-1,i,j).x][cvt(-1,-1,i,j).y]==O &&
               checkerboard[cvt(0,-1,i,j).x][cvt(0,-1,i,j).y]==X && checkerboard[cvt(2,-2,i,j).x][cvt(2,-2,i,j).y]==O && (checkerboard[cvt(2,-3,i,j).x][cvt(2,-3,i,j).y]==X || checkerboard[cvt(2,1,i,j).x][cvt(2,1,i,j).y]==X) &&
               checkerboard[cvt(3,-3,i,j).x][cvt(3,-3,i,j).y]==O && checkerboard[cvt(4,-4,i,j).x][cvt(4,-4,i,j).y]==X)//設定局型
            {
                if(x==cvt(3,-2,i,j).x && y==cvt(3,-2,i,j).y)//設定可走的位置
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

int hor(int x, int y, int ob)
{
    int Count = 0;
    for(int m = y-4; m<=y+4; m++)
    {
        if(m<0 || m>chessboard_Width-1)
        {
            continue;
        }
        else
        {
            if(checkerboard[x][m]==ob)
            {
                Count++;
                if(Count==5)
                {
                    break;
                }
            }
            else
            {
                Count = 0;
            }
        }
    }
    if(Count >= 5)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int ver(int x, int y, int ob)
{
    int Count = 0;
    for(int m = x-4; m<=x+4; m++)
    {
        if(m<0 || m>chessboard_Width-1)
        {
            continue;
        }
        else
        {
            if(checkerboard[m][y]==ob)
            {
                Count++;
                if(Count==5)
                {
                    break;
                }
            }
            else
            {
                Count = 0;
            }
        }
    }
    if(Count >= 5)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int slash(int x, int y, int ob)
{
    int Count = 0;
    for(int m= x-4,n=y-4; m<= x+4; m++,n++)
    {
        if(m<0 || n<0 || m>chessboard_Width-1 || n>chessboard_Width-1)
        {
            continue;
        }
        else
        {
            if(checkerboard[m][n]==ob)
            {
                Count++;
                if(Count==5)
                {
                    break;
                }
            }
            else
            {
                Count = 0;
            }
        }
    }
    if(Count >= 5)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int backslash(int x, int y, int ob)
{
    int Count = 0;
    for(int m = x-4, n=y+4; m<=x+4; m++,n--)
    {
        if(m<0 || n<0 || m>chessboard_Width-1 || n>chessboard_Width-1)
        {
            continue;
        }
        else
        {
            if(checkerboard[m][n]==ob)
            {
                Count++;
                if(Count==5)
                {
                    break;
                }
            }
            else
            {
                Count = 0;
            }
        }

    }
    if(Count >= 5)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int Judge_Who_Win(int x,int y)
{
    if(hor(x,y,O)==1 || ver(x,y,O)==1|| slash(x,y,O)==1|| backslash(x,y,O)==1)
    {
        return 2;//O wins
    }

    if(hor(x,y,X)==1 || ver(x,y,X)==1|| slash(x,y,X)==1|| backslash(x,y,X)==1)
    {
        return 1;//X wins
    }

    return 0;
}

int Compute_Win_Chance(int x,int y)
{
    Location temp_Point;
    int Type[20]={0};

    if(step_Timer==1) //第一子落點
    {
        if(database_Step1(x,y)==1)
        {
            is_Chess_manual=1;
            return 50000;
        }
    }
    if(step_Timer==3) //第三子落點
    {
        if(database_Step3(x,y)==1)
        {
            is_Chess_manual=1;
            return 50000;
        }
    }
    if(step_Timer==5) //第五子落點
    {
        if(database_Step5(x,y)==1)
        {
            is_Chess_manual=1;
            return 50000;
        }
    }
    if(step_Timer==11) //第十一子落點
    {
        if(database_Step11(x,y)==1)
        {
            is_Chess_manual=1;
            return 50000;
        }
    }
    if(step_Timer==13) //第十三子落點
    {
        if(database_Step13(x,y)==1)
        {
            is_Chess_manual=1;
            return 50000;
        }
    }
    if(step_Timer==15) //第十五子落點
    {
        if(database_Step15(x,y)==1)
        {
            is_Chess_manual=1;
            return 50000;
        }
    }
    if(step_Timer==17) //第十七子落點
    {
        if(database_Step17(x,y)==1)
        {
            is_Chess_manual=1;
            return 50000;
        }
    }
    if(step_Timer==19) //第十九子落點
    {
        if(database_Step19(x,y)==1)
        {
            is_Chess_manual=1;
            return 50000;
        }
    }

    for(int i=1;i<=4;i++)//進攻
    {
        Type[Judge_Type(x,y,i)]++;
    }
    for(int i=1;i<=4;i++)//防守
    {
        Type[Judge_Defend_Type(x,y,i)]++;
    }

    if(Type[9]>=1)//一步殺
    {
        return 16384;
    }
    else if(Type[19]>=1)//敵方一步殺
    {
        return 8192;
    }
    else if(Type[8]>=1 || Type[7]+Type[6]>=2 || (Type[7]+Type[6]>=1 && Type[5]+Type[4]>=1))//兩步殺
    {
        return 4096;
    }
    else if(close_Filter_1==1)//關掉下面的判斷依據
    {
        return 1;
    }
    else if(Type[18]>=1 || Type[17]+Type[16]>=2 || (Type[17]+Type[16]>=1 && Type[15]+Type[14]>=1))//敵方兩步殺
    {
        return 1024;
    }
    else if(Type[5]+Type[4]>=2)//三步殺
    {
        return 512;
    }
    else if(Type[6]+Type[7]>=1)
    {
        return 500;
    }
    else if(Type[3]>=1 && Type[5]+Type[4]>=1)
    {
        return 256;
    }
    else if(Type[5]+Type[4]>=1)
    {
        return 192;
    }
    else if(Type[15]+Type[14]>=2)//敵方三步殺
    {
        return 96;
    }
    else if(Type[2]>=2)
    {
        return 48;
    }
    else if(Type[2]>=1)
    {
        return 40;
    }
    else if(Type[3]>=1)
    {
        return 35;
    }
    else if(Type[1]>=1)
    {
        return 30;
    }
    else if(Type[1]+Type[2]+Type[3]+Type[4]+Type[5]+Type[6]+Type[7]+Type[8]+Type[9]+Type[11]+Type[12]+Type[13]+Type[14]+Type[15]+Type[16]+Type[17]+Type[18]+Type[19]>=1)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

int Compute_Enemy_Win_Chance(int x,int y)
{
    Location temp_Point;
    int Type[20]={0};

    for(int i=1;i<=4;i++)//進攻
    {
        Type[Judge_Type(x,y,i)]++;
    }
    for(int i=1;i<=4;i++)//防守
    {
        Type[Judge_Defend_Type(x,y,i)]++;
    }

    if(Type[19]>=1)//一步殺
    {
        return 16384;
    }
    else if(Type[9]>=1)//敵方一步殺
    {
        return 8192;
    }
    else if(Type[18]>=1 || Type[17]+Type[16]>=2 || (Type[17]+Type[16]>=1 && Type[15]+Type[14]>=1))//兩步殺
    {
        return 4096;
    }
    else if(close_Filter_1==1)//關掉下面的判斷依據
    {
        return 1;
    }
    else if(Type[8]>=1 || Type[7]+Type[6]>=2 || (Type[7]+Type[6]>=1 && Type[5]+Type[4]>=1))//敵方兩步殺
    {
        return 1024;
    }
    else if(Type[15]+Type[14]>=2)//三步殺
    {
        return 512;
    }
    else if(Type[16]+Type[17]>=1)
    {
        return 500;
    }
    else if(Type[13]>=1 && Type[15]+Type[14]>=1)
    {
        return 256;
    }
    else if(Type[15]+Type[14]>=1)
    {
        return 192;
    }
    else if(Type[5]+Type[4]>=2)//敵方三步殺
    {
        return 96;
    }
    else if(Type[12]>=2)
    {
        return 48;
    }
    else if(Type[12]>=1)
    {
        return 40;
    }
    else if(Type[13]>=1)
    {
        return 35;
    }
    else if(Type[11]>=1)
    {
        return 30;
    }
    else if(Type[1]+Type[2]+Type[3]+Type[4]+Type[5]+Type[6]+Type[7]+Type[8]+Type[9]+Type[11]+Type[12]+Type[13]+Type[14]+Type[15]+Type[16]+Type[17]+Type[18]+Type[19]>=1)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

vector<Location> Filter_1(int is_For_Recursive)
{
    int win_Chance[chessboard_Width][chessboard_Width] =
        {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};

    vector<Location> final_Choose_Point;
    int Max=-1000;
    Location temp_Point;

    for(int i=0;i<chessboard_Width;i++)
    {
        for(int j=0;j<chessboard_Width;j++)
        {
            win_Chance[i][j]=Compute_Win_Chance(i,j);
            if(!is_For_Recursive)
            {
                win_Chance_1[i][j]=win_Chance[i][j];
            }

            if(win_Chance[i][j]>Max)
            {
                Max=win_Chance[i][j];
            }
        }
    }

    for(int i=0;i<chessboard_Width;i++)//將可選的點加入vector中
    {
        for(int j=0;j<chessboard_Width;j++)
        {
            if(win_Chance[i][j]>Max*0.5)
            {
                temp_Point.x=i;
                temp_Point.y=j;
                final_Choose_Point.push_back(temp_Point);
            }
        }
    }
    return final_Choose_Point;
}

vector<Location> Filter_1_For_Enemy(int is_For_Recursive)
{
    int win_Chance[chessboard_Width][chessboard_Width] =
        {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};

    vector<Location> final_Choose_Point;
    int Max=-1000;
    Location temp_Point;

    for(int i=0;i<chessboard_Width;i++)
    {
        for(int j=0;j<chessboard_Width;j++)
        {
            win_Chance[i][j]=Compute_Enemy_Win_Chance(i,j);
            if(!is_For_Recursive)
            {
                win_Chance_1[i][j]=win_Chance[i][j];
            }

            if(win_Chance[i][j]>Max)
            {
                Max=win_Chance[i][j];
            }
        }
    }

    for(int i=0;i<chessboard_Width;i++)//將可選的點加入vector中
    {
        for(int j=0;j<chessboard_Width;j++)
        {
            if(win_Chance[i][j]>Max*0.5)
            {
                temp_Point.x=i;
                temp_Point.y=j;
                final_Choose_Point.push_back(temp_Point);
            }
        }
    }
    return final_Choose_Point;
}

int Compute_Win_Chance_Level_2(int x,int y)
{
    Location temp_Point;
    int Type[20]={0};

    for(int i=1;i<=4;i++)//進攻
    {
        Type[Judge_Type(x,y,i)]++;
    }
    for(int i=1;i<=4;i++)//防守
    {
        Type[Judge_Defend_Type(x,y,i)]++;
    }

    if(Type[17]+Type[16]>=1 && Type[15]+Type[14]>=1)
    {
        return 4850;
    }
    return Type[1]*type1_Weight+Type[2]*type2_Weight+Type[3]*type3_Weight+Type[4]*type4b_Weight+Type[5]*type4a_Weight+Type[6]*type5b_Weight+Type[7]*type5a_Weight+Type[8]*type6_Weight+Type[9]*type7_Weight+Type[11]*type11_Weight+Type[12]*type12_Weight+Type[13]*type13_Weight+Type[14]*type14b_Weight+Type[15]*type14a_Weight+Type[16]*type15b_Weight+Type[17]*type15a_Weight+Type[18]*type16_Weight+Type[19]*type17_Weight+1;
}

int Compute_Enemy_Win_Chance_Level_2(int x,int y)
{
    Location temp_Point;
    int Type[20]={0};

    for(int i=1;i<=4;i++)//進攻
    {
        Type[Judge_Type(x,y,i)]++;
    }
    for(int i=1;i<=4;i++)//防守
    {
        Type[Judge_Defend_Type(x,y,i)]++;
    }

    if(Type[7]+Type[6]>=1 && Type[5]+Type[4]>=1)
    {
        return 4850;
    }
    return Type[11]*type1_Weight+Type[12]*type2_Weight+Type[13]*type3_Weight+Type[14]*type4b_Weight+Type[15]*type4a_Weight+Type[16]*type5b_Weight+Type[17]*type5a_Weight+Type[18]*type6_Weight+Type[19]*type7_Weight+Type[1]*type11_Weight+Type[2]*type12_Weight+Type[3]*type13_Weight+Type[4]*type14b_Weight+Type[5]*type14a_Weight+Type[6]*type15b_Weight+Type[7]*type15a_Weight+Type[8]*type16_Weight+Type[9]*type17_Weight+1;
}

vector<Location> Filter_2(vector<Location> choose_Point,int is_For_Recursive)
{
    int win_Chance[chessboard_Width][chessboard_Width] =
        {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};

    vector<Location> final_Choose_Point;
    int Max=-1000;
    Location temp_Point;

    for(auto x:choose_Point)
    {
        win_Chance[x.x][x.y]=Compute_Win_Chance_Level_2(x.x,x.y);
        if(!is_For_Recursive)
        {
            win_Chance_2[x.x][x.y]=win_Chance[x.x][x.y];
        }

        if(win_Chance[x.x][x.y]>Max)
        {
            Max=win_Chance[x.x][x.y];
        }
    }

    if(!is_For_Recursive)
    {
        filter_2_Max_Value=Max;
    }

    for(auto x:choose_Point)
    {
        if(!is_For_Recursive)
        {
            if(win_Chance[x.x][x.y]>=Max*filter_2_Strict)
            {
                temp_Point.x=x.x;
                temp_Point.y=x.y;
                final_Choose_Point.push_back(temp_Point);
            }
        }
        else
        {
            if(win_Chance[x.x][x.y]>=Max*filter_2_Strict_For_Recursive)
            {
                temp_Point.x=x.x;
                temp_Point.y=x.y;
                final_Choose_Point.push_back(temp_Point);
            }
        }
    }
    return final_Choose_Point;
}

vector<Location> Filter_2_For_Enemy(vector<Location> choose_Point,int is_For_Recursive)
{
    int win_Chance[chessboard_Width][chessboard_Width] =
        {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};

    vector<Location> final_Choose_Point;
    int Max=-1000;
    Location temp_Point;

    for(auto x:choose_Point)
    {
        win_Chance[x.x][x.y]=Compute_Enemy_Win_Chance_Level_2(x.x,x.y);
        if(!is_For_Recursive)
        {
            win_Chance_2[x.x][x.y]=win_Chance[x.x][x.y];
        }

        if(win_Chance[x.x][x.y]>Max)
        {
            Max=win_Chance[x.x][x.y];
        }
    }

    for(auto x:choose_Point)
    {
        if(!is_For_Recursive)
        {
            if(win_Chance[x.x][x.y]>=Max*filter_2_Strict)
            {
                temp_Point.x=x.x;
                temp_Point.y=x.y;
                final_Choose_Point.push_back(temp_Point);
            }
        }
        else
        {
            if(win_Chance[x.x][x.y]>=Max*filter_2_Strict_For_Recursive)
            {
                temp_Point.x=x.x;
                temp_Point.y=x.y;
                final_Choose_Point.push_back(temp_Point);
            }
        }
    }
    return final_Choose_Point;
}

int Compute_AD_Rate(int x,int y,int type)
{
    int AD_Rate=0;
    int Type[20]={0};

    checkerboard[x][y]=0;

    if(computer_Take==O)
    {
        for(int i=1;i<=4;i++)//進攻
        {
            Type[Judge_Type(x,y,i)]++;
        }
        for(int i=1;i<=4;i++)//防守
        {
            Type[Judge_Defend_Type(x,y,i)]++;
        }

        if(type==X)
        {
            if(Type[9]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if(Type[8]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if(Type[7]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if(Type[6]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if(Type[5]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if(Type[4]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }

            else if(Type[19]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if(Type[18]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if(Type[17]+Type[16]>=2)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if((Type[17]+Type[16]>=1) && (Type[15]+Type[14]>=1))
            {
                AD_Rate+=filter_3_When_Defend;
            }
        }
        /*
        else if(type==O)
        {
            if(Type[19]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if(Type[18]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if(Type[17]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if(Type[16]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if(Type[15]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if(Type[14]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }

            else if(Type[9]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if(Type[8]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if(Type[7]+Type[6]>=2)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if((Type[7]+Type[6]>=1) && (Type[5]+Type[4]>=1))
            {
                AD_Rate+=filter_3_When_Attack;
            }
        }
        */
    }
    else if(computer_Take==X)
    {
        for(int i=1;i<=4;i++)//進攻
        {
            Type[Judge_Type(x,y,i)]++;
        }
        for(int i=1;i<=4;i++)//防守
        {
            Type[Judge_Defend_Type(x,y,i)]++;
        }

        if(type==O)
        {
            if(Type[19]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if(Type[18]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if(Type[17]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if(Type[16]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if(Type[15]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if(Type[14]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }

            if(Type[9]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if(Type[8]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if(Type[7]+Type[6]>=2)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if((Type[7]+Type[6]>=1) && (Type[5]+Type[4]>=1))
            {
                AD_Rate+=filter_3_When_Defend;
            }
        }
        /*
        else if(type==X)
        {
            if(Type[9]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if(Type[8]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if(Type[7]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if(Type[6]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if(Type[5]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }
            else if(Type[4]>=1)
            {
                AD_Rate+=filter_3_When_Defend;
            }

            if(Type[19]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if(Type[18]>=1)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if(Type[17]+Type[16]>=2)
            {
                AD_Rate+=filter_3_When_Attack;
            }
            else if((Type[17]+Type[16]>=1) && (Type[15]+Type[14]>=1))
            {
                AD_Rate+=filter_3_When_Attack;
            }
        }
        */
    }

    if(type==X)
    {
        checkerboard[x][y]=O;
    }
    else if(type==O)
    {
        checkerboard[x][y]=X;
    }

    return AD_Rate;
}

Compute_Win_Chance_Level_3(int x,int y,int type,int iterations,int max_Iterations)
{
    /*
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            SetColor();
            if(checkerboard[i][j]==O)
            {
                SetColor(12);
                cout<<'O'<<" ";
            }
            else if(checkerboard[i][j]==X)
            {
                SetColor(14);
                cout<<'X'<<" ";
            }
            else
            {
                cout<<checkerboard[i][j]<<" ";
            }
        }
        cout<<endl<<endl;
    }
    if(iterations==max_Iterations)
    {
        cout<<"bottom"<<endl;
    }
    system("pause");
    */
    if(iterations==max_Iterations)
    {
        future_Total_Steps++;
        return filter_3_No_Win_No_Lose;
    }
    else if(Judge_Who_Win(x,y)==computer_Take)
    {
        future_Total_Steps++;
        return filter_3_When_Win;
    }
    else if(Judge_Who_Win(x,y)==gamer_Take)
    {
        future_Total_Steps++;
        return filter_3_When_Lose;
    }
    else
    {
        vector<Location> after_Filter1_Point;
        vector<Location> after_Filter2_Point;
        int sum=0;

        int AD_Rate=0;//Attack and Defend rate
        if(type==X)
        {
            after_Filter1_Point=Filter_1_For_Enemy(1);
            after_Filter2_Point=Filter_2_For_Enemy(after_Filter1_Point,1);
            for(auto x:after_Filter2_Point)
            {
                checkerboard[x.x][x.y]=X;
                step_Timer++;
                sum+=Compute_Win_Chance_Level_3(x.x,x.y,O,iterations+1,max_Iterations);
                checkerboard[x.x][x.y]=0;
                step_Timer--;
            }
        }
        else if(type==O)
        {
            after_Filter1_Point=Filter_1(1);
            after_Filter2_Point=Filter_2(after_Filter1_Point,1);

            for(auto x:after_Filter2_Point)
            {
                checkerboard[x.x][x.y]=O;
                step_Timer++;
                sum+=Compute_Win_Chance_Level_3(x.x,x.y,X,iterations+1,max_Iterations);
                checkerboard[x.x][x.y]=0;
                step_Timer--;
            }
        }
        AD_Rate=Compute_AD_Rate(x,y,type);
        return sum+AD_Rate;
    }
}

vector<Location> Filter_3(vector<Location> choose_Point)
{
    int win_Chance[chessboard_Width][chessboard_Width] =
        {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
         {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};

    vector<Location> final_Choose_Point;
    int Max=-1000;
    Location temp_Point;
    total_Future_Total_Step=0;
    for(auto x:choose_Point)
    {
        future_Total_Steps=0;
        checkerboard[x.x][x.y]=O;
        step_Timer++;
        win_Chance[x.x][x.y]=Compute_Win_Chance_Level_3(x.x,x.y,X,0,recursive_Step_times);
        win_Chance[x.x][x.y]=win_Chance[x.x][x.y]/future_Total_Steps;
        total_Future_Total_Step+=future_Total_Steps;
        win_Chance_3[x.x][x.y]=win_Chance[x.x][x.y];
        checkerboard[x.x][x.y]=0;
        step_Timer--;

        if(win_Chance[x.x][x.y]>Max)
        {
            Max=win_Chance[x.x][x.y];
        }
    }

    for(auto x:choose_Point)
    {
        if(win_Chance[x.x][x.y]==Max)
        {
            temp_Point.x=x.x;
            temp_Point.y=x.y;
            final_Choose_Point.push_back(temp_Point);
        }
    }
    return final_Choose_Point;
}

int find_win_Chance_Maxvalue(int type)
{
    int win_Chance_Maxvalue=-1000;
    if(type==1)
    {
        for(int i=0;i<chessboard_Width;i++)
        {
            for(int j=0;j<chessboard_Width;j++)
            {
                if(win_Chance_1[i][j]>win_Chance_Maxvalue && checkerboard[i][j]==0)
                {
                    win_Chance_Maxvalue=win_Chance_1[i][j];
                }
            }
        }
    }
    if(type==2)
    {
        for(int i=0;i<chessboard_Width;i++)
        {
            for(int j=0;j<chessboard_Width;j++)
            {
                if(win_Chance_2[i][j]>win_Chance_Maxvalue && checkerboard[i][j]==0)
                {
                    win_Chance_Maxvalue=win_Chance_2[i][j];
                }
            }
        }
    }
    if(type==3)
    {
        for(int i=0;i<chessboard_Width;i++)
        {
            for(int j=0;j<chessboard_Width;j++)
            {
                if(win_Chance_3[i][j]>win_Chance_Maxvalue && checkerboard[i][j]==0)
                {
                    win_Chance_Maxvalue=win_Chance_3[i][j];
                }
            }
        }
    }
    return win_Chance_Maxvalue;
}

int find_win_Chance_Minvalue(int type)
{
    int win_Chance_Minvalue=100000;
    if(type==1)
    {
        for(int i=0;i<chessboard_Width;i++)
        {
            for(int j=0;j<chessboard_Width;j++)
            {
                if(win_Chance_1[i][j]<win_Chance_Minvalue && checkerboard[i][j]==0)
                {
                    win_Chance_Minvalue=win_Chance_1[i][j];
                }
            }
        }
    }
    if(type==2)
    {
        for(int i=0;i<chessboard_Width;i++)
        {
            for(int j=0;j<chessboard_Width;j++)
            {
                if(win_Chance_2[i][j]<win_Chance_Minvalue && checkerboard[i][j]==0)
                {
                    win_Chance_Minvalue=win_Chance_2[i][j];
                }
            }
        }
    }
    if(type==3)
    {
        for(int i=0;i<chessboard_Width;i++)
        {
            for(int j=0;j<chessboard_Width;j++)
            {
                if(win_Chance_3[i][j]<win_Chance_Minvalue && checkerboard[i][j]==0)
                {
                    win_Chance_Minvalue=win_Chance_3[i][j];
                }
            }
        }
    }
    return win_Chance_Minvalue;
}

vector<Location> Choose_Filter_2_Max()
{
    int filter_2_Max=-1000;
    vector<Location> return_Choose_Points;
    Location temp_Point;

    for(int i=0;i<chessboard_Width;i++)//找出最大值
    {
        for(int j=0;j<chessboard_Width;j++)
        {
            if(win_Chance_2[i][j]>filter_2_Max)
            {
                filter_2_Max=win_Chance_2[i][j];
            }
        }
    }

    for(int i=0;i<chessboard_Width;i++)//將可選的點加入vector中
    {
        for(int j=0;j<chessboard_Width;j++)
        {
            if(win_Chance_2[i][j]==filter_2_Max)
            {
                temp_Point.x=i;
                temp_Point.y=j;
                return_Choose_Points.push_back(temp_Point);
            }
        }
    }
    return return_Choose_Points;
}

Location Computer_Step(int is_For_O)
{
    vector<Location> after_Filter1_Point;
    vector<Location> after_Filter2_Point;
    vector<Location> after_Filter3_Point;
    int Choose;

    for(int i=0;i<chessboard_Width;i++)//清空win_Chance_1,win_Chance_2,win_Chance_3
    {
        for(int j=0;j<chessboard_Width;j++)
        {
            win_Chance_1[i][j]=-1;
            win_Chance_2[i][j]=-1;
            win_Chance_3[i][j]=-1;
        }
    }

    if(is_For_O==1)
    {
        after_Filter1_Point=Filter_1(0);
        after_Filter2_Point=Filter_2(after_Filter1_Point,0);
        if(step_Timer>=open_filter_3_Step && step_Timer<close_filter_3_Step && find_win_Chance_Maxvalue(1)!=50000 && after_Filter2_Point.size()!=1)
        {
            after_Filter3_Point=Filter_3(after_Filter2_Point);
        }
    }
    else if(is_For_O==0)
    {
        after_Filter1_Point=Filter_1_For_Enemy(0);
        after_Filter2_Point=Filter_2_For_Enemy(after_Filter1_Point,0);
    }

    if(find_win_Chance_Maxvalue(1)==50000)
    {
        Choose=rand()%after_Filter1_Point.size();
        return after_Filter1_Point[Choose];
    }
    else if(find_win_Chance_Maxvalue(3)<=1000)
    {
        after_Filter2_Point=Choose_Filter_2_Max();
        Choose=rand()%after_Filter2_Point.size();
        return after_Filter2_Point[Choose];
    }
    else
    {
        Choose=rand()%after_Filter3_Point.size();
        return after_Filter3_Point[Choose];
    }

}

char symbol(int m,int n)
{
    if(checkerboard[m][n]==X)
    {
        return 'X';
    }
    if(checkerboard[m][n]==O)
    {
        return 'O';
    }
    else if(checkerboard[m][n]==0)
    {
        if((m==3 && n==3) || (m==3 && n==11) || (m==11 && n==3) || (m==11 && n==11))
        {
            return '+';
        }
        else
        {
            return '.';
        }
    }
    return ' ';
}

void Print_Map(int x,int y)
{
    for(int i=30;i<37;i++)
    {
        gotoxy(0,i);
        cout<<"                                                        "<<endl;

    }
    gotoxy(0,0);
    printf("Now is Your turn !!!\n");
    for(int m = 0; m < chessboard_Width; m++)//print the map
    {
        for(int n=0; n < chessboard_Width; n++)
        {
            if(m==st[0] && n==st[1])
            {
                cout<<"{ ";
                if(symbol(m,n)=='X')
                {
                    SetColor(10);
                }
                if(m==x && n==y && checkerboard[m][n]==O)
                {
                    SetColor(12);
                }
                cout<<symbol(m,n);
                SetColor();
                cout<<" }";
            }
            else
            {
                if(m==x && n==y && checkerboard[m][n]==O)
                {
                    SetColor(12);
                }
                if(symbol(m,n)=='X')
                {
                    SetColor(10);
                }
                printf("  %c  ", symbol(m,n));
                SetColor();
            }
        }
        printf("\n\n");
    }
    cout<<"The computer has predicted "<<total_Future_Total_Step<<" situations"<<endl;
}
void Debug_Mode(Location computer_Step)
{
    cout<<endl;
    gotoxy(80,31);
    cout<<"First filter"<<endl<<endl;
    for(int i=0;i<chessboard_Width;i++)
    {
        gotoxy(80,i*2+1);
        for(int j=0;j<chessboard_Width;j++)
        {
            if(checkerboard[i][j]==O && (i!=computer_Step.x || j!=computer_Step.y))
            {
                cout<<'O'<<"    ";
            }
            else if(checkerboard[i][j]==X)
            {
                SetColor(10);
                cout<<'X'<<"    ";
            }
            else
            {
                if(checkerboard[i][j]==O)
                {
                    SetColor(12);
                }
                else if(win_Chance_1[i][j]>win_Chance_1[computer_Step.x][computer_Step.y]*0.5)
                {
                    SetColor(6);
                }
                else if(win_Chance_1[i][j]>-1)
                {
                    SetColor(14);
                }
                else
                {
                    SetColor(8);
                }

                if(win_Chance_1[i][j]<0)
                {
                    cout<<win_Chance_1[i][j]<<"   ";
                }
                else if(win_Chance_1[i][j]/10000!=0)
                {
                    cout<<win_Chance_1[i][j]<<"";
                }
                else if(win_Chance_1[i][j]/1000!=0)
                {
                    cout<<win_Chance_1[i][j]<<" ";
                }
                else if(win_Chance_1[i][j]/100!=0)
                {
                    cout<<win_Chance_1[i][j]<<"  ";
                }
                else if(win_Chance_1[i][j]/10!=0)
                {
                    cout<<win_Chance_1[i][j]<<"   ";
                }
                else
                {
                    cout<<win_Chance_1[i][j]<<"    ";
                }
            }
            SetColor();
        }
        cout<<endl<<endl;
    }
    cout<<endl;
    gotoxy(0,34);
    system("pause");

    gotoxy(80,31);
    cout<<"Second filter"<<endl<<endl;
    for(int i=0;i<chessboard_Width;i++)
    {
        gotoxy(80,i*2+1);
        for(int j=0;j<chessboard_Width;j++)
        {
            if(checkerboard[i][j]==O && (i!=computer_Step.x || j!=computer_Step.y))
            {
                cout<<'O'<<"    ";
            }
            else if(checkerboard[i][j]==X)
            {
                SetColor(10);
                cout<<'X'<<"    ";
            }
            else
            {
                if(checkerboard[i][j]==O)
                {
                    SetColor(12);
                }
                else if(win_Chance_2[i][j]>=filter_2_Max_Value*filter_2_Strict && win_Chance_2[computer_Step.x][computer_Step.y]!=0)
                {
                    SetColor(6);
                }
                else if(win_Chance_2[i][j]>-1)
                {
                    SetColor(14);
                }
                else
                {
                    SetColor(8);
                }

                if(win_Chance_2[i][j]<0)
                {
                    cout<<win_Chance_2[i][j]<<"   ";
                }
                else if(win_Chance_2[i][j]/10000!=0)
                {
                    cout<<win_Chance_2[i][j]<<"";
                }
                else if(win_Chance_2[i][j]/1000!=0)
                {
                    cout<<win_Chance_2[i][j]<<" ";
                }
                else if(win_Chance_2[i][j]/100!=0)
                {
                    cout<<win_Chance_2[i][j]<<"  ";
                }
                else if(win_Chance_2[i][j]/10!=0)
                {
                    cout<<win_Chance_2[i][j]<<"   ";
                }
                else
                {
                    cout<<win_Chance_2[i][j]<<"    ";
                }
            }
            SetColor();
        }
        cout<<endl<<endl;
    }
    cout<<endl;
    gotoxy(0,34);
    system("pause");

    gotoxy(80,31);
    cout<<"Third filter "<<endl<<endl;
    for(int i=0;i<chessboard_Width;i++)
    {
        gotoxy(80,i*2+1);
        for(int j=0;j<chessboard_Width;j++)
        {
            if(checkerboard[i][j]==O && (i!=computer_Step.x || j!=computer_Step.y))
            {
                cout<<'O'<<"    ";
            }
            else if(checkerboard[i][j]==X)
            {
                SetColor(10);
                cout<<'X'<<"    ";
            }
            else
            {
                if(checkerboard[i][j]==O)
                {
                    SetColor(12);
                }
                else if(win_Chance_3[i][j]>=0)
                {
                    SetColor(14);
                }
                else
                {
                    SetColor(8);
                }

                if(win_Chance_3[i][j]<0)
                {
                    if(win_Chance_3[i][j]/10000!=0)
                    {
                        cout<<win_Chance_3[i][j]<<"";
                    }
                    else if(win_Chance_3[i][j]/1000!=0)
                    {
                        cout<<win_Chance_3[i][j]<<"";
                    }
                    else if(win_Chance_3[i][j]/100!=0)
                    {
                        cout<<win_Chance_3[i][j]<<" ";
                    }
                    else if(win_Chance_3[i][j]/10!=0)
                    {
                        cout<<win_Chance_3[i][j]<<"  ";
                    }
                    else
                    {
                        cout<<win_Chance_3[i][j]<<"   ";
                    }
                }
                else
                {
                    if(win_Chance_3[i][j]/10000!=0)
                    {
                        cout<<win_Chance_3[i][j]<<"";
                    }
                    else if(win_Chance_3[i][j]/1000!=0)
                    {
                        cout<<win_Chance_3[i][j]<<" ";
                    }
                    else if(win_Chance_3[i][j]/100!=0)
                    {
                        cout<<win_Chance_3[i][j]<<"  ";
                    }
                    else if(win_Chance_3[i][j]/10!=0)
                    {
                        cout<<win_Chance_3[i][j]<<"   ";
                    }
                    else
                    {
                        cout<<win_Chance_3[i][j]<<"    ";
                    }
                }

            }
            SetColor();
        }
        cout<<endl<<endl;
    }
    cout<<endl;
    gotoxy(0,34);
    system("pause");

    for(int i=0;i<35;i++)
    {
        gotoxy(76,i*2+1);
        cout<<"                                                                            "<<endl;
    }
}

void Layout_Mode(int st[2],Location computer_Step)
{
    cout<<"Enter The Layout Mode"<<endl;
    int leave_flag=0;
    int k;
    while(1)
    {
        k = getch();
        switch(k)
        {
            case 'w':
                if(st[0]==0)
                {
                    st[0]=chessboard_Width-1;
                }
                else
                {
                    st[0]--;
                }
                break;

            case 'a':
                if(st[1]==0)
                {
                    st[1]=chessboard_Width-1;
                }
                else
                {
                    st[1]--;
                }
                break;
            case 's':
                if(st[0]==chessboard_Width-1)
                {
                    st[0]=0;
                }
                else
                {
                    st[0]++;
                }
                break;
            case 'd':
                if(st[1]==chessboard_Width-1)
                {
                    st[1]=0;
                }
                else
                {
                    st[1]++;
                }
                break;
            case '1':
                if(checkerboard[st[0]][st[1]]==0)
                {
                    step_Timer++;
                }
                checkerboard[st[0]][st[1]] = O;
                break;
            case '2':
                if(checkerboard[st[0]][st[1]]==0)
                {
                    step_Timer++;
                }
                checkerboard[st[0]][st[1]] = X;
                break;
            case '3':
                if(checkerboard[st[0]][st[1]]==O || checkerboard[st[0]][st[1]]==X)
                {
                    step_Timer--;
                }
                checkerboard[st[0]][st[1]] = 0;
                break;
            case 'l':
                leave_flag=1;
                break;

        }

        Print_Map(computer_Step.x,computer_Step.y);

        if(leave_flag==1)
        {
            cout<<"Leaving The Layout Mode"<<endl;
            Sleep(1000);
            return;
        }
    }
}

int Check_OX_Amount()
{
    int O_counter=0;
    int X_counter=0;
    for(int i=0;i<chessboard_Width;i++)
    {
        for(int j=0;j<chessboard_Width;j++)
        {
            if(checkerboard[i][j]==O)
            {
                O_counter+=1;
            }
            if(checkerboard[i][j]==X)
            {
                X_counter+=1;
            }
        }
    }

    if(O_counter==X_counter)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Print_Start_Map()
{
    HideCursor();
    gotoxy(0,0);
    cout<<"                                 1                                                                                    1"<<endl;
    cout<<"                                 1                                                                                    1"<<endl;
    cout<<"                                 1                                                                                    1"<<endl;
    cout<<"                                 1                                                                                    1"<<endl;
    cout<<"                                 1                                                                                    1"<<endl;
    cout<<"                                 1        @ @                     @                                                   1"<<endl;
    cout<<"                                 1      @     @                   @              @ @                                  1"<<endl;
    cout<<"                                 1    @                           @                  @                                1"<<endl;
    cout<<"                                 1    @                @ @        @ @ @          @ @ @        @ @ @          @ @ @    1"<<endl;
    cout<<"                                 1    @              @     @      @     @      @     @        @     @      @     @    1"<<endl;
    cout<<"                                 1    @     @ @      @     @      @     @      @     @        @     @      @     @    1"<<endl;
    cout<<"                                 1      @     @      @     @      @     @      @     @        @     @      @     @    1"<<endl;
    cout<<"                                 1        @ @          @ @        @ @ @          @ @   @      @     @        @ @ @    1"<<endl;
    cout<<"                                 1                                                                               @    1"<<endl;
    cout<<"                                 1                                                                           @ @      1"<<endl;

}

void Print_Start_Map_Part_2(int position)
{
    gotoxy(0,15);
    for(int i=0;i<20;i++)
    {
        cout<<"                                                                                                                        "<<endl;
    }
    gotoxy(0,16);
    cout<<endl<<endl<<endl<<endl<<endl;
    if(position==0)
    {
        SetColor(6);
        cout<<"                                                               > >  ";
        SetColor();
        cout<<"s t a r t  g a m e";
        SetColor(6);
        cout<<"  < <"<<endl;
        SetColor();
    }
    else
    {
        cout<<"                                                                    s t a r t  g a m e"<<endl;
    }

    cout<<endl<<endl;
    if(position==1)
    {
        SetColor(6);
        cout<<"                                                                 > >  ";
        SetColor();
        cout<<"s e t t i n g";
        SetColor(6);
        cout<<"  < <"<<endl;
        SetColor();
    }
    else
    {
        cout<<"                                                                      s e t t i n g"<<endl;
    }

    cout<<endl<<endl;
    if(position==2)
    {
        SetColor(6);
        cout<<"                                                                    > >  ";
        SetColor();
        cout<<"h e l p";
        SetColor(6);
        cout<<"  < <"<<endl;
        SetColor();
    }
    else
    {
        cout<<"                                                                         h e l p"<<endl;
    }

    cout<<endl<<endl;
    if(position==3)
    {
        SetColor(6);
        cout<<"                                                                    > >  ";
        SetColor();
        cout<<"e x i t";
        SetColor(6);
        cout<<"  < <"<<endl;
        SetColor();
    }
    else
    {
        cout<<"                                                                         e x i t"<<endl;
    }
}

void Print_Setting_Map(int position)
{
    gotoxy(0,0);
    for(int i=0;i<30;i++)
    {
        cout<<"                                                                                                                        "<<endl;
    }
    gotoxy(0,0);
    cout<<endl<<endl<<endl<<endl<<endl;
    if(position==0)
    {
        SetColor(6);
        cout<<"                 > >  ";
        SetColor();
        cout<<"d i f f i c u l t";
    }
    else
    {
        cout<<"                      d i f f i c u l t";
    }
    cout<<"  < ";
    if(difficult==0)
    {
        SetColor(10);
        cout<<"Easy";
    }
    else if(difficult==1)
    {
        SetColor(11);
        cout<<"Medium";
    }
    else if(difficult==2)
    {
        SetColor(12);
        cout<<"Hard";
    }
    else if(difficult==3)
    {
        SetColor(13);
        cout<<"Master";
    }
    SetColor();
    cout<<" >"<<endl;

    cout<<endl<<endl;
    if(position==1)
    {
        SetColor(6);
        cout<<"                 > >  ";
        SetColor();
        cout<<"c l o s e _ f i l t e r _ 1";
    }
    else
    {
        cout<<"                      c l o s e _ f i l t e r _ 1";
    }
    cout<<"  < ";
    if(close_Filter_1==1)
    {
        SetColor(9);
        cout<<"True";
    }
    else if(close_Filter_1==0)
    {
        SetColor(12);
        cout<<"False";
    }
    SetColor();
    cout<<" >"<<endl;

    cout<<endl<<endl;
    if(position==2)
    {
        SetColor(6);
        cout<<"                 > >  ";
        SetColor();
        cout<<"f i l t e r _ 2 _ S t r i c t";
    }
    else
    {
        cout<<"                      f i l t e r _ 2 _ S t r i c t";
    }
    cout<<"  < ";
    SetColor(14);
    if(abs(filter_2_Strict)<0.000001)
    {
        cout<<0;
    }
    else
    {
        cout<<filter_2_Strict;
    }
    SetColor();
    cout<<" >"<<endl;

    cout<<endl<<endl;
    if(position==3)
    {
        SetColor(6);
        cout<<"                 > >  ";
        SetColor();
        cout<<"f i l t e r _ 2 _ S t r i c t _ F o r _ R e c u r s i v e";
    }
    else
    {
        cout<<"                      f i l t e r _ 2 _ S t r i c t _ F o r _ R e c u r s i v e";
    }
    cout<<"  < ";
    SetColor(14);
    if(abs(filter_2_Strict_For_Recursive)<0.000001)
    {
        cout<<0;
    }
    else
    {
        cout<<filter_2_Strict_For_Recursive;
    }
    SetColor();
    cout<<" >"<<endl;

    cout<<endl<<endl;
    if(position==4)
    {
        SetColor(6);
        cout<<"                 > >  ";
        SetColor();
        cout<<"o p e n _ f i l t e r _ 3 _ S t e p";
    }
    else
    {
        cout<<"                      o p e n _ f i l t e r _ 3 _ S t e p";
    }
    cout<<"  < ";
    SetColor(14);
    cout<<open_filter_3_Step;
    SetColor();
    cout<<" >"<<endl;

    cout<<endl<<endl;
    if(position==5)
    {
        SetColor(6);
        cout<<"                 > >  ";
        SetColor();
        cout<<"c l o s e _ f i l t e r _ 3 _ S t e p";
    }
    else
    {
        cout<<"                      c l o s e _ f i l t e r _ 3 _ S t e p";
    }
    cout<<"  < ";
    SetColor(14);
    cout<<close_filter_3_Step;
    SetColor();
    cout<<" >"<<endl;

    cout<<endl<<endl;
    if(position==6)
    {
        SetColor(6);
        cout<<"                 > >  ";
        SetColor();
        cout<<"r e c u r s i v e _ S t e p _ t i m e s";
    }
    else
    {
        cout<<"                      r e c u r s i v e _ S t e p _ t i m e s";
    }
    cout<<"  < ";
    SetColor(14);
    cout<<recursive_Step_times;
    SetColor();
    cout<<" >"<<endl;

    cout<<endl<<endl;
    if(position==7)
    {
        SetColor(6);
        cout<<"                 > >  ";
        SetColor();
        cout<<"b a c k"<<endl;
    }
    else
    {
        cout<<"                      b a c k"<<endl;
    }
}

void Set_Difficult()
{
    if(difficult==0)
    {
        close_Filter_1=0;
        filter_2_Strict=0.1;
        filter_2_Strict_For_Recursive=0;
        open_filter_3_Step=257;
        close_filter_3_Step=257;
        recursive_Step_times=0;
    }
    else if(difficult==1)
    {
        close_Filter_1=0;
        filter_2_Strict=1;
        filter_2_Strict_For_Recursive=0;
        open_filter_3_Step=257;
        close_filter_3_Step=257;
        recursive_Step_times=0;
    }
    else if(difficult==2)
    {
        close_Filter_1=1;
        filter_2_Strict=0.9;
        filter_2_Strict_For_Recursive=0.99;
        open_filter_3_Step=11;
        close_filter_3_Step=211;
        recursive_Step_times=10;
    }
    else if(difficult==3)
    {
        close_Filter_1=1;
        filter_2_Strict=0.6;
        filter_2_Strict_For_Recursive=0.99;
        open_filter_3_Step=7;
        close_filter_3_Step=205;
        recursive_Step_times=16;
    }
}

void change_Parameter(int position,int type)
{
    if(type==0)
    {
        if(position==0)
        {
            if(difficult==3)
            {
                difficult=0;
            }
            else
            {
                difficult++;
            }
            Set_Difficult();
        }
        else if(position==1)
        {
            if(close_Filter_1==1)
            {
                close_Filter_1=0;
            }
            else
            {
                close_Filter_1++;
            }
        }
        else if(position==2)
        {
            if(abs(filter_2_Strict-1)<=0.000001)
            {
                filter_2_Strict=0;
            }
            else
            {
                filter_2_Strict+=0.01;
            }
        }
        else if(position==3)
        {
            if(abs(filter_2_Strict_For_Recursive-1)<=0.000001)
            {
                filter_2_Strict_For_Recursive=0;
            }
            else
            {
                filter_2_Strict_For_Recursive+=0.01;
            }
        }
        else if(position==4)
        {
            if(open_filter_3_Step==225-recursive_Step_times)
            {
                open_filter_3_Step=0;
            }
            else
            {
                open_filter_3_Step++;
            }
        }
        else if(position==5)
        {
            if(close_filter_3_Step==225-recursive_Step_times)
            {
                close_filter_3_Step=0;
            }
            else
            {
                close_filter_3_Step++;
            }
        }
        else if(position==6)
        {
            recursive_Step_times++;
        }
    }
    else if(type==1)
    {
        if(position==0)
        {
            if(difficult==0)
            {
                difficult=3;
            }
            else
            {
                difficult--;
            }
            Set_Difficult();
        }
        else if(position==1)
        {
            if(close_Filter_1==0)
            {
                close_Filter_1=1;
            }
            else
            {
                close_Filter_1--;
            }
        }
        else if(position==2)
        {
            if(abs(filter_2_Strict)<=0.000001)
            {
                filter_2_Strict=1;
            }
            else
            {
                filter_2_Strict-=0.01;
            }
        }
        else if(position==3)
        {
            if(abs(filter_2_Strict_For_Recursive)<=0.000001)
            {
                filter_2_Strict_For_Recursive=1;
            }
            else
            {
                filter_2_Strict_For_Recursive-=0.01;
            }
        }
        else if(position==4)
        {
            if(open_filter_3_Step==0)
            {
                open_filter_3_Step=225-recursive_Step_times;
            }
            else
            {
                open_filter_3_Step--;
            }
        }
        else if(position==5)
        {
            if(close_filter_3_Step==0)
            {
                close_filter_3_Step=225-recursive_Step_times;
            }
            else
            {
                close_filter_3_Step--;
            }
        }
        else if(position==6)
        {
            if(recursive_Step_times>0)
            {
                recursive_Step_times--;
            }
        }
    }
}

void Setting()
{
    char operation;
    int position=0;
    while(1)
    {
        if(_kbhit())
        {
            operation=getch();
            if(operation=='w')
            {
                if(position==0)
                {
                    position=7;
                }
                else
                {
                    position--;
                }
            }
            else if(operation=='s')
            {
                if(position==7)
                {
                    position=0;
                }
                else
                {
                    position++;
                }
            }
            else if(operation=='+')
            {
                change_Parameter(position,0);
            }
            else if(operation=='-')
            {
                change_Parameter(position,1);
            }
            else if(operation==32)
            {
                gotoxy(0,0);
                for(int i=0;i<31;i++)
                {
                    cout<<"                                                                                                                        "<<endl;
                }

                if(position==7)
                {
                    break;
                }
            }
        }

        Print_Setting_Map(position);

        Sleep(10);
    }
}

void Help()
{
    gotoxy(0,0);
    for(int i=0;i<30;i++)
    {
        cout<<"                                                                                                                        "<<endl;
    }
    gotoxy(0,0);

    cout<<endl<<endl<<endl<<endl<<endl;
    cout<<"       /// in menu ///"<<endl<<endl;
    cout<<"       press ";
    SetColor(12);
    cout<<"w";
    SetColor();
    cout<<",";
    SetColor(12);
    cout<<"s";
    SetColor();
    cout<<" to up and down"<<endl;

    cout<<"       press ";
    SetColor(12);
    cout<<"space";
    SetColor();
    cout<<" to enter"<<endl;
    cout<<endl;


    cout<<"       /// in setting ///"<<endl<<endl;
    cout<<"       press ";
    SetColor(12);
    cout<<"w";
    SetColor();
    cout<<",";
    SetColor(12);
    cout<<"s";
    SetColor();
    cout<<" to up and down"<<endl;

    cout<<"       press ";
    SetColor(12);
    cout<<"space";
    SetColor();
    cout<<" to enter"<<endl;

    cout<<"       press ";
    SetColor(12);
    cout<<"+";
    SetColor();
    cout<<",";
    SetColor(12);
    cout<<"-";
    SetColor();
    cout<<" to edit parameter"<<endl<<endl;


    cout<<"       /// in game ///"<<endl<<endl;
    cout<<"       press ";
    SetColor(12);
    cout<<"w";
    SetColor();
    cout<<",";
    SetColor(12);
    cout<<"a";
    SetColor();
    cout<<",";
    SetColor(12);
    cout<<"s";
    SetColor();
    cout<<",";
    SetColor(12);
    cout<<"d";
    SetColor();
    cout<<" to move"<<endl;

    cout<<"       press ";
    SetColor(12);
    cout<<"space";
    SetColor();
    cout<<" to drop chess"<<endl;

    cout<<"       press ";
    SetColor(12);
    cout<<"q";
    SetColor();
    cout<<" to regret"<<endl;

    cout<<"       press ";
    SetColor(12);
    cout<<"b";
    SetColor();
    cout<<" to enter Developer Mode"<<endl;

    cout<<"       press ";
    SetColor(12);
    cout<<"l(lower L)";
    SetColor();
    cout<<" to enter Layout Mode"<<endl;

    cout<<"       press ";
    SetColor(12);
    cout<<"Esc";
    SetColor();
    cout<<" to leave"<<endl<<endl;


    cout<<"       /// in Layout Mode ///"<<endl<<endl;
    cout<<"       press ";
    SetColor(12);
    cout<<"w";
    SetColor();
    cout<<",";
    SetColor(12);
    cout<<"a";
    SetColor();
    cout<<",";
    SetColor(12);
    cout<<"s";
    SetColor();
    cout<<",";
    SetColor(12);
    cout<<"d";
    SetColor();
    cout<<" to move"<<endl;

    cout<<"       press ";
    SetColor(12);
    cout<<"1";
    SetColor();
    cout<<" to drop O"<<endl;

    cout<<"       press ";
    SetColor(12);
    cout<<"2";
    SetColor();
    cout<<" to drop X"<<endl;

    cout<<"       press ";
    SetColor(12);
    cout<<"3";
    SetColor();
    cout<<" to remove this chess"<<endl;

    cout<<"       press ";
    SetColor(12);
    cout<<"l(lower L)";
    SetColor();
    cout<<" to leave Layout Mode"<<endl<<endl<<endl;


    cout<<"       ";
    system("pause");
}

void Start()
{
    char operation;
    int position=0;
    while(1)
    {
        Print_Start_Map();
        if(_kbhit())
        {
            operation=getch();
            if(operation=='w')
            {
                if(position==0)
                {
                    position=3;
                }
                else
                {
                    position--;
                }
            }
            else if(operation=='s')
            {
                if(position==3)
                {
                    position=0;
                }
                else
                {
                    position++;
                }
            }
            else if(operation==32)
            {
                gotoxy(0,0);
                for(int i=0;i<31;i++)
                {
                    cout<<"                                                                                                                        "<<endl;
                }

                if(position==0)
                {
                    break;
                }
                else if(position==1)
                {
                    Setting();
                }
                else if(position==2)
                {
                    Help();
                }
                else if(position==3)
                {
                    exit(1);
                }
            }
        }

        Print_Start_Map_Part_2(position);

        Sleep(10);
    }
}

int main()
{
    int k;
    int a;
    int b;
    st[0]=0;//x
    st[1]=0;//y
    int turn;
    int winner;
    int regret_flag;
    int leave_flag;
    int leave_Game_flag;
    Location computer_Step;
    Location my_Step;
    stack<Location> regret_Stack;

    while(1)
    {
        Start();
        for(int i=0;i<chessboard_Width;i++)
        {
            for(int j=0;j<chessboard_Width;j++)
            {
                checkerboard[i][j]=0;
            }
        }

        k = 0;
        time_t start,finish;
        a=0;
        b=0;
        st[0]=7;//x
        st[1]=7;//y
        turn = X;
        winner=0;
        step_Timer=1;
        regret_flag=0;
        srand(time(0));
        rand_x=7;
        rand_y=7;
        time(&start);
        while(!regret_Stack.empty())
        {
            regret_Stack.pop();
        }
        while(1)
        {
            if(regret_flag==0)
            {
                total_Future_Total_Step=0;
                computer_Step=Computer_Step(1);//換電腦下

                checkerboard[computer_Step.x][computer_Step.y]=O;
                regret_Stack.push(computer_Step);

                Print_Map(computer_Step.x,computer_Step.y);

                //判斷電腦是否贏了
                if(Judge_Who_Win(computer_Step.x,computer_Step.y)==computer_Take)
                {
                    time(&finish);
                    printf("You lose!\n");
                    printf("\nThis game spent %.2f seconds and take %d steps\n",difftime(finish,start),step_Timer);
                    k = getch();
                    if(k=='q')
                    {
                        checkerboard[regret_Stack.top().x][regret_Stack.top().y]=0;
                        regret_Stack.pop();
                        checkerboard[regret_Stack.top().x][regret_Stack.top().y]=0;
                        regret_Stack.pop();
                        computer_Step.x= regret_Stack.top().x;
                        computer_Step.y= regret_Stack.top().y;
                        step_Timer--;
                        step_Timer--;
                    }
                    else
                    {
                        break;
                    }

                }
                step_Timer+=1;
            }

            Print_Map(computer_Step.x,computer_Step.y);

            leave_flag=0;
            leave_Game_flag=0;
            regret_flag=0;
            while(1)
            {
                k = getch();
                switch(k)
                {
                    case 'w':
                        if(st[0]==0)
                        {
                            st[0]=chessboard_Width-1;
                        }
                        else
                        {
                            st[0]--;
                        }
                        break;

                    case 'a':
                        if(st[1]==0)
                        {
                            st[1]=chessboard_Width-1;
                        }
                        else
                        {
                            st[1]--;
                        }
                        break;
                    case 's':
                        if(st[0]==chessboard_Width-1)
                        {
                            st[0]=0;
                        }
                        else
                        {
                            st[0]++;
                        }
                        break;
                    case 'd':
                        if(st[1]==chessboard_Width-1)
                        {
                            st[1]=0;
                        }
                        else
                        {
                            st[1]++;
                        }
                        break;
                    case 'q':
                        if(regret_Stack.size()>1)
                        {
                            checkerboard[regret_Stack.top().x][regret_Stack.top().y]=0;
                            regret_Stack.pop();
                            checkerboard[regret_Stack.top().x][regret_Stack.top().y]=0;
                            regret_Stack.pop();
                            computer_Step.x= regret_Stack.top().x;
                            computer_Step.y= regret_Stack.top().y;
                            step_Timer--;
                            step_Timer--;
                        }
                        break;
                    case 'b':
                        Debug_Mode(computer_Step);
                        break;
                    case 'l':
                        Layout_Mode(st,computer_Step);
                        if(Check_OX_Amount()==1)//O amount = X amount
                        {
                            leave_flag=1;
                        }
                        break;
                    case 32:
                        if(checkerboard[st[0]][st[1]] != X && checkerboard[st[0]][st[1]] != O)//落子
                        {
                            checkerboard[st[0]][st[1]] = turn;
                            my_Step.x=st[0];
                            my_Step.y=st[1];
                            regret_Stack.push(my_Step);
                            leave_flag=1;
                        }
                        else
                        {
                            printf("\nTry again!\n");
                            system("pause");
                        }
                        break;
                    case 27:
                        leave_Game_flag=1;
                        break;
                }

                Print_Map(computer_Step.x,computer_Step.y);

                if(leave_flag==1 || leave_Game_flag==1)
                {
                    break;
                }
            }
            if(leave_Game_flag==1)
            {
                break;
            }
            //判斷玩家是否贏了
            if(Judge_Who_Win(st[0],st[1])==gamer_Take)
            {
                time(&finish);
                printf("You win!\n");
                printf("\nThis game spent %.2f seconds and take %d steps\n",difftime(finish,start),step_Timer);
                k = getch();
                if(k=='q')
                {
                    checkerboard[regret_Stack.top().x][regret_Stack.top().y]=0;
                    regret_Stack.pop();
                    regret_flag=1;
                    step_Timer--;
                }
                else
                {
                    break;
                }
            }
            step_Timer+=1;
        }
    }
    return 0;
 }


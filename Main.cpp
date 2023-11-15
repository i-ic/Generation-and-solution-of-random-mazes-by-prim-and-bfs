/* 主程序 */

#include <graphics.h>
#include <Windows.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <queue>
#include <conio.h>
#pragma comment(lib,"Winmm.lib")//导入音效播放api

using namespace std;

#define WIDTH 1080//分辨率
#define HEIGHT 720
#define BLOCK_XY 24//小方格边长
const int BLOCK_WIDTH = WIDTH / (BLOCK_XY);//小方格x轴数量 
const int BLOCK_HEIGHT = HEIGHT / (BLOCK_XY);//小方格y轴数量

#define lu COLORREF RGB(220,220,220)//路面颜色
#define qiangbi COLORREF RGB(30,30,30)//墙壁颜色
#define qidian COLORREF RGB(0,255,0)//起点颜色
#define zhongdian COLORREF RGB(255,0,0)//终点颜色
#define xuanzhong COLORREF RGB(190,50,50)//选中颜色
#define bfsPath COLORREF RGB(200,200,255)//BFS路径颜色
#define shortestPath COLORREF RGB(255,255,0)//最短路径颜色

struct block 
{
public:
    int x;
    int y;
    block(int x, int y) 
    {
        this->x = x;
        this->y = y;
    }
};
//表示地图中的方块以及方块的位置

int map[BLOCK_WIDTH][BLOCK_HEIGHT];
vector<block*> openlist;//待选列表
vector<block*> list;// 创建一个用于存储周围路节点的列表
block* myprev[BLOCK_WIDTH][BLOCK_HEIGHT]; 
/* 用于记录父节点，同时可以用于bfs寻找最短路径 */

// 函数声明
void clearscreen(COLORREF RGB);//遍历地图并且初始化地图颜色
void BLOCK_color(int x, int y, COLORREF RGB);//在x，y位置画颜色为RGB的方格
void init(void);//初始化
void add_list(int x, int y);//将x,y四周不是路的节点加入待选列表
void redarw(void);//刷新屏幕
void prim(void);//prim主循环
void through(int x, int y);//将x,y与它四周一个随机已经变成路的节点打通
void bfs(void); //BFS寻找路径
void drawShortestPath(block* start, block* end); //画出路径

void clearscreen(COLORREF RGB) 
{
    for (int y = 0; y < BLOCK_HEIGHT; y++) 
    {
        for (int x = 0; x < BLOCK_WIDTH; x++) 
        {
            BLOCK_color(x, y, RGB);
        }
    }
}

void BLOCK_color(int x, int y, COLORREF RGB) 
{
    setfillcolor(RGB); setlinecolor(COLORREF RGB(0, 0, 0));
    fillrectangle(x * BLOCK_XY, y * BLOCK_XY, BLOCK_XY + x * BLOCK_XY, BLOCK_XY + y * BLOCK_XY);
}

int main(void) 
{
    srand((unsigned)time(NULL)); //随机数种子
    initgraph(WIDTH, HEIGHT);//创建绘图窗口
    BeginBatchDraw();
    cleardevice();//清空显示
    init();//主程序，初始化迷宫并自动开始查找
    mciSendString(_T("open levelup.mp3 alias bkmusic"), NULL, 0, NULL);//播放音效
    mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);//循环播放
    Sleep(1000);
    mciSendString(_T("close bkmusic"), NULL, 0, NULL);//停止播放
    BLOCK_color(1, 1, qidian);
    BLOCK_color(BLOCK_WIDTH - 2, BLOCK_HEIGHT - 2, zhongdian);
    FlushBatchDraw();//批量绘制
    while (1);//保持窗口不被关闭
}

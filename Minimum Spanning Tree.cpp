/* 最小生成树初始化并生成迷宫 */

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
static const int BLOCK_WIDTH = WIDTH / (BLOCK_XY);//小方格x轴数量 
static const int BLOCK_HEIGHT = HEIGHT / (BLOCK_XY);//小方格y轴数量 

#define lu COLORREF RGB(220,220,220)//路面颜色
#define qiangbi COLORREF RGB(30,30,30)//墙壁颜色
#define qidian COLORREF RGB(0,255,0)//起点颜色
#define zhongdian COLORREF RGB(255,0,0)//终点颜色
#define xuanzhong COLORREF RGB(190,50,50)//选中颜色

extern struct block {
public:
    int x;
    int y;
    block(int x, int y) {
        this->x = x;
        this->y = y;
    }
};
//表示地图中的方块以及方块的位置

extern int map[BLOCK_WIDTH][BLOCK_HEIGHT];
extern vector<block*> openlist;//待选列表
extern vector<block*> list;
extern block* myprev[BLOCK_WIDTH][BLOCK_HEIGHT]; 
/* 用于记录父节点，同时可以用于bfs寻找最短路径 */

// 函数声明
void clearscreen(COLORREF RGB);//遍历地图并且初始化地图颜色
void BLOCK_color(int x, int y, COLORREF RGB);//在x，y位置画颜色为RGB的方格
void init(void);//初始化
void add_list(int x, int y);//将x,y四周不是路的节点加入待选列表
void redarw(void);//刷新屏幕
void prim(void);//prim主循环
void through(int x, int y);//将x,y与它四周一个随机已经变成路的路点打通
void bfs(void); //BFS寻找路径
void drawShortestPath(block* start, block* end); //画出路径

void init(void) {
    openlist.clear();
        for (int y = 0; y < BLOCK_HEIGHT; y++)     // 地图初始化为墙
        {
            for (int x = 0; x < BLOCK_WIDTH; x++) 
            {
                map[x][y] = 1;
                myprev[x][y] = nullptr;
            }
        }
        map[1][1] = 0;    // 起始化起点、终点
        map[BLOCK_WIDTH - 2][BLOCK_HEIGHT - 2] = 0;
        add_list(1, 1);        // 将节点的子节点中墙壁加入待选列表
    prim();
    bfs();
}

void prim(void) {
    mciSendString(_T("open orb.mp3 alias bkmusic1"), NULL, 0, NULL);//播放音效
    mciSendString(_T("play bkmusic1 repeat"), NULL, 0, NULL);//循环播放
    while (openlist.size() > 0) //直到所有的节点都已经被访问过
    {

        int Index = rand() % openlist.size();        // 从待选中随机选一个开始节点
        block* kaishi = openlist[Index];        // 将开始节点与它四周一个随机已经变成路的节点打通
        through(kaishi->x, kaishi->y);
        map[kaishi->x][kaishi->y] = 0;        // 将开始节点变成路
        add_list(kaishi->x, kaishi->y);        // 将开始节点的子节点中的所有墙壁加入待选列表
        openlist.erase(openlist.begin() + Index);        // 从待选路点中移除开始节点
        BLOCK_color(kaishi->x, kaishi->y, xuanzhong);
        FlushBatchDraw();
        redarw();
    }
    mciSendString(_T("close bkmusic1"), NULL, 0, NULL);//停止播放
    openlist.clear();
    if (map[BLOCK_WIDTH - 1][BLOCK_HEIGHT - 2] == 1 || map[BLOCK_WIDTH - 2][BLOCK_HEIGHT - 1] == 1 || map[BLOCK_WIDTH - 3][BLOCK_HEIGHT - 2] == 1 || map[BLOCK_WIDTH - 2][BLOCK_HEIGHT - 3] || 1)
    {
        map[BLOCK_WIDTH - 3][BLOCK_HEIGHT - 3] = 0;map[BLOCK_WIDTH - 4][BLOCK_HEIGHT - 3] = 0;
        map[BLOCK_WIDTH - 5][BLOCK_HEIGHT - 3] = 0;map[BLOCK_WIDTH - 6][BLOCK_HEIGHT - 3] = 0;
        map[BLOCK_WIDTH - 6][BLOCK_HEIGHT - 2] = 0;map[BLOCK_WIDTH - 2][BLOCK_HEIGHT - 3] = 0;
        FlushBatchDraw();
        redarw();
    }
}

void through(int x, int y) 
{
    list.clear();
    // 检查上方的节点，如果是路，则加入列表
    if (y - 2 >= 0 && map[x][y - 2] == 0) 
    {
        block* a = new block(x, y - 1);
        list.push_back(a);
    }
    //下
    if (y + 2 <= BLOCK_HEIGHT && map[x][y + 2] == 0) 
    {
        block* b = new block(x, y + 1);
        list.push_back(b);
    }
    //左
    if (x - 2 >= 0 && map[x - 2][y] == 0) 
    {
        block* c = new block(x - 1, y);
        list.push_back(c);
    }
    //右
    if (x + 2 <= BLOCK_WIDTH && map[x + 2][y] == 0) 
    {
        block* d = new block(x + 1, y);
        list.push_back(d);
    }
    int AIndexea = rand() % list.size();// 从列表中随机选择一个路节点
    block* A = list[AIndexea];
    // 将x，y与A打通
        map[A->x][A->y] = 0;
}

void add_list(int x, int y) 
{
    // 检查上方的节点，如果是墙壁，并且还没有被加入到待选列表中，那么就将它加入到待选列表中
    if (y - 2 >= 0 && map[x][y - 2] == 1) 
    {
        block* a = new block(x, y - 2);
        openlist.push_back(a);
        map[x][y - 2] = 2;
    }
    //下
    if (y + 2 <= BLOCK_HEIGHT && map[x][y + 2] == 1) 
    {
        block* b = new block(x, y + 2);
        openlist.push_back(b);
        map[x][y + 2] = 2;
    }
    //左
    if (x - 2 >= 0 && map[x - 2][y] == 1) 
    {
        block* c = new block(x - 2, y);
        openlist.push_back(c);
        map[x - 2][y] = 2;
    }
    //右
    if (x + 2 <= BLOCK_WIDTH && map[x + 2][y] == 1) 
    {
        block* d = new block(x + 2, y);
        openlist.push_back(d);
        map[x + 2][y] = 2;
    }
}

void redarw(void) 
{
    for (int y = 0; y < BLOCK_HEIGHT; y++) 
    {
        for (int x = 0; x < BLOCK_WIDTH; x++) 
        {
            if (map[x][y] == 1) 
            {
                BLOCK_color(x, y, qiangbi);
            }
            else if (map[x][y] == 2) 
            {
                BLOCK_color(x, y, qiangbi);
            }
            else if (map[x][y] == 0) 
            {
                BLOCK_color(x, y, lu);
            }
        }
    }
    FlushBatchDraw();
}

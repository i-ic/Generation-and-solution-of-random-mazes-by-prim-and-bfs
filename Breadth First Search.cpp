/* 用于搜索迷宫路径并绘制最短路径 */

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

#define bfsPath COLORREF RGB(200,200,255)//BFS路径颜色
#define shortestPath COLORREF RGB(255,255,0)//最短路径颜色
#define qidian COLORREF RGB(0,255,0)//起点颜色
#define zhongdian COLORREF RGB(255,0,0)//终点颜色

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
void through(int x, int y);//将x,y与它四周一个随机已经变成路的节点打通
void bfs(void); //BFS寻找路径
void drawShortestPath(block* start, block* end); //画出路径

void drawShortestPath(block* start, block* end) {
    block* current = end;
    while (current != start) {
        BLOCK_color(current->x, current->y, shortestPath);
        current = myprev[current->x][current->y];
    }
}

void bfs(void) 
{
    queue<block*> bfsQueue;//队列遍历节点、先进先出
    block* start = new block(1, 1);
    block* end = new block(BLOCK_WIDTH - 2, BLOCK_HEIGHT - 2);
    bfsQueue.push(start);
    mciSendString(_T("open portal.mp3 alias bkmusic2"), NULL, 0, NULL);//播放音效
    mciSendString(_T("play bkmusic2 repeat"), NULL, 0, NULL);//循环播放
    while (!bfsQueue.empty())     //判断队列是否为空，队列为空表示无路径(生成了无解迷宫)
    {
        block* current = bfsQueue.front();
        bfsQueue.pop();//删掉队列最下层的元素
        if (current->x == end->x && current->y == end->y) //判断是否到达目标节点
        {
            drawShortestPath(start, end);//画出最短路径
            mciSendString(_T("close bkmusic2"), NULL, 0, NULL);//停止播放
            BLOCK_color(1, 1, qidian);
            BLOCK_color(BLOCK_WIDTH - 2, BLOCK_HEIGHT - 2, zhongdian);
            FlushBatchDraw();//批量绘制
            return;//结束bfs
        }
        if (current->y - 1 >= 0 && map[current->x][current->y - 1] == 0 && myprev[current->x][current->y - 1] == nullptr)         // 如果当前节点的上节点是未访问过的，那么就将它们加入到队列中
        {
            block* up = new block(current->x, current->y - 1);
            myprev[up->x][up->y] = current;
            bfsQueue.push(up);
            BLOCK_color(up->x, up->y, bfsPath);
            FlushBatchDraw();//批量绘制
        }
        if (current->y + 1 < BLOCK_HEIGHT && map[current->x][current->y + 1] == 0 && myprev[current->x][current->y + 1] == nullptr)         //下
        {
            block* down = new block(current->x, current->y + 1);
            myprev[down->x][down->y] = current;
            bfsQueue.push(down);
            BLOCK_color(down->x, down->y, bfsPath);
            FlushBatchDraw();//批量绘制
        }
        if (current->x - 1 >= 0 && map[current->x - 1][current->y] == 0 && myprev[current->x - 1][current->y] == nullptr)         //左
        {
            block* left = new block(current->x - 1, current->y);
            myprev[left->x][left->y] = current;
            bfsQueue.push(left);
            BLOCK_color(left->x, left->y, bfsPath);
            FlushBatchDraw();//批量绘制
        }
        if (current->x + 1 < BLOCK_WIDTH && map[current->x + 1][current->y] == 0 && myprev[current->x + 1][current->y] == nullptr)         //右
        {
            block* right = new block(current->x + 1, current->y);
            myprev[right->x][right->y] = current;
            bfsQueue.push(right);
            BLOCK_color(right->x, right->y, bfsPath);
            FlushBatchDraw();//批量绘制
        }
    }
    exit(1);
}
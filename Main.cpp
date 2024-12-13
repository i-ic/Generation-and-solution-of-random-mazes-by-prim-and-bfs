/* 主程序 */

#include <graphics.h>
#include "initset.h"
#include "bits/stdc++.h"
#pragma comment(lib,"Winmm.lib")//导入音效播放api

struct block {
public:
    int x;
    int y;
    block(int x, int y) {
        this->x = x;
        this->y = y;
    }
};
//表示地图中的方块以及方块的位置

int map[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10];
std::vector<block*> openlist;//待选列表
std::vector<block*> list;// 创建一个用于存储周围路节点的列表
block* myprev[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10]; 
/* 用于记录父节点，同时可以用于bfs寻找最短路径 */

// 函数声明
void clearScreen(COLORREF RGB);//遍历地图并且初始化地图颜色
void blockColor(int x, int y, COLORREF RGB);//在x，y位置画颜色为RGB的方格
void init(void);//初始化

void clearScreen(COLORREF RGB) { //建图,复杂度n*m
    for (int y = 0; y <= BLOCK_HEIGHT; y++) {
        for (int x = 0; x <= BLOCK_WIDTH; x++) {
            blockColor(x, y, RGB);
        }
    }
}

void blockColor(int x, int y, COLORREF RGB) {
    setfillcolor(RGB); setlinecolor(COLORREF RGB(0, 0, 0));
    fillrectangle(x * BLOCK_XY, y * BLOCK_XY, BLOCK_XY + x * BLOCK_XY, BLOCK_XY + y * BLOCK_XY);
}

int main(void) {
    srand((unsigned)time(NULL)); //随机数种子
    initgraph(WIDTH + BLOCK_XY, HEIGHT + BLOCK_XY);//创建绘图窗口
    BeginBatchDraw();
    cleardevice();//清空显示
    init();//主程序，初始化迷宫并自动开始查找
    mciSendString(_T("open levelup.mp3 alias bkmusic"), NULL, 0, NULL);//播放音效
    mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);//循环播放
    Sleep(1000);
    mciSendString(_T("close bkmusic"), NULL, 0, NULL);//停止播放
    blockColor(1, 1, beginPoint);
    blockColor(BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, endPoint);
    FlushBatchDraw();//批量绘制
    while (1);//保持窗口不被关闭
}

/* 用于搜索迷宫路径并绘制最短路径 */

#include <graphics.h>
#include "initset.h"
#include "bits/stdc++.h"
#pragma comment(lib,"Winmm.lib")//导入音效播放api

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

extern int map[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10];
extern std::vector<block*> openlist;//待选列表
extern std::vector<block*> list;
extern block* myprev[BLOCK_WIDTH + 10][BLOCK_HEIGHT + 10]; 
/* 用于记录父节点，同时可以用于bfs寻找最短路径 */

// 函数声明
void blockColor(int x, int y, COLORREF RGB);//在x，y位置画颜色为RGB的方格
void bfs(void); //BFS寻找路径
void drawShortestPath(block* start, block* end); //画出路径

void drawShortestPath(block* start, block* end) {
    block* current = end;
    while (current != start) {
        blockColor(current->x, current->y, shortestPath);
        current = myprev[current->x][current->y];
    }
}
void bfs(void) //每个点最多被访问一次，复杂度O(n*m)
{
    std::queue<block*> bfsQueue;//队列遍历节点、先进先出
    int cnt = 1920;
    COLORREF bfsPath = RGB(190,190,cnt / 10);//BFS路径颜色
    block* start = new block(1, 1);
    block* end = new block(BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1);
    bfsQueue.push(start);
    mciSendString(_T("open portal.mp3 alias bkmusic2"), NULL, 0, NULL);//播放音效
    mciSendString(_T("play bkmusic2 repeat"), NULL, 0, NULL);//循环播放
    while (!bfsQueue.empty()) {
        block* current = bfsQueue.front();
        bfsQueue.pop();
        if (current->x == end->x && current->y == end->y) { //判断是否到达目标节点
            drawShortestPath(start, end);//画出最短路径
            mciSendString(_T("close bkmusic2"), NULL, 0, NULL);//停止播放
            blockColor(1, 1, beginPoint);
            blockColor(BLOCK_WIDTH - 1, BLOCK_HEIGHT - 1, endPoint);
            FlushBatchDraw();//批量绘制
            return;//结束bfs
        }
        for (int i = 0; i < 4; ++i) { //方向数组check四个方向合法性
            if (current->x + dir1[i][0] > 0 && current->y + dir1[i][1] > 0 && 
                current->x + dir1[i][0] < BLOCK_WIDTH && current->y + dir1[i][1] < BLOCK_HEIGHT &&
                map[current->x + dir1[i][0]][current->y + dir1[i][1]] == 0 && 
                myprev[current->x + dir1[i][0]][current->y + dir1[i][1]] == nullptr){         // 如果当前节点的子节点是未访问过的，那么就将它们加入到队列中
                    block* temp = new block(current->x + dir1[i][0], current->y + dir1[i][1]);
                    myprev[temp->x][temp->y] = current;
                    bfsQueue.push(temp);
                    cnt++;
                    bfsPath = RGB(max(50,190 - (cnt / 50)),max(50, 190 - (cnt / 50)), min(255, cnt / 10));
                    blockColor(temp->x, temp->y, bfsPath);
                    FlushBatchDraw();//批量绘制
            }
        }
    }
    exit(1);
}
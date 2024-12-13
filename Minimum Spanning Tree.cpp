/* 最小生成树初始化并生成迷宫 */

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
void init(void);//初始化
void addList(int x, int y);//将x,y四周不是路的节点加入待选列表
void reDarw(void);//刷新屏幕
void prim(void);//prim主循环
bool through(int x, int y);//将x,y与它四周一个随机已经变成路的路点打通
void bfs(void); //BFS寻找路径

void init(void) {
    openlist.clear();
        for (int y = 0; y <= BLOCK_HEIGHT; y++) {     // 地图初始化为墙,复杂度n*m
            for (int x = 0; x <= BLOCK_WIDTH; x++) {
                map[x][y] = 1;
                myprev[x][y] = nullptr;
            }
        }
        map[1][1] = 0;    // 起始化起点、终点
        map[BLOCK_WIDTH - 1][BLOCK_HEIGHT - 1] = 0;
        addList(1, 1);        // 将节点的子节点中墙壁加入待选列表
    prim();//
    bfs();
}

void prim(void) { //每个点至多被访问一次 外层循环n*m,内层重绘制函数n*m,复杂度为n2*m2/REF，为本程序的复杂度瓶颈
    mciSendString(_T("open orb.mp3 alias bkmusic1"), NULL, 0, NULL);//播放音效
    mciSendString(_T("play bkmusic1 repeat"), NULL, 0, NULL);//循环播放
    while (openlist.size() > 0) { //直到所有的节点都已经被访问过
        int Index = rand() % openlist.size();        // 从待选中随机选一个开始节点
        block* kaishi = openlist[Index];        // 将开始节点与它四周一个随机已经变成路的节点打通
        if (through(kaishi->x, kaishi->y)) {
            map[kaishi->x][kaishi->y] = 0;        // 将开始节点变成路
            addList(kaishi->x, kaishi->y);     // 将开始节点的子节点中的所有墙壁加入待选列表
        }
        openlist.erase(openlist.begin() + Index);        // 从待选路点中移除开始节点
        blockColor(kaishi->x, kaishi->y, selected);
        FlushBatchDraw();
        if (openlist.size() % REF == 0) {
            reDarw();
        }
    }
    for (int i = 0; i < 4; ++i) { //保证迷宫有解
        through(BLOCK_WIDTH - 1,BLOCK_HEIGHT - 1);
        through(BLOCK_WIDTH - 3, BLOCK_HEIGHT - 3);
    }
    reDarw();
    mciSendString(_T("close bkmusic1"), NULL, 0, NULL);//停止播放
}

bool through(int x, int y) { //复杂度o1
    list.clear();
    // 检查子节点，如果是路，则加入列表
    for (int i = 0; i < 4; ++i) {
        if (y + dir2[i][1] > 0 && x + dir2[i][0] > 0 && 
            y + dir2[i][1] < BLOCK_HEIGHT && x + dir2[i][0] < BLOCK_WIDTH &&
            map[x + dir2[i][0]][y + dir2[i][1]] == 0) {
                block* temp = new block(x + dir1[i][0], y + dir1[i][1]);
                list.push_back(temp);
        }
    }
    if (list.size() != 0) {
        int AIndexea = rand() % list.size();// 从列表中随机选择一个路节点
        block* A = list[AIndexea];
        map[A->x][A->y] = 0;// 将x，y与A打通
        return true;
    } else {
        return false;
    }
}

void addList(int x, int y) { //复杂度o1
    // 检查子节点，如果是墙壁，并且还没有被加入到待选列表中，那么就将它加入到待选列表中
    for (int i = 0; i < 4; ++i) {
        if (x + dir2[i][0] > 0 && y + dir2[i][1] > 0 &&
            x + dir2[i][0] < BLOCK_WIDTH && y + dir2[i][1] < BLOCK_HEIGHT &&
            map[x + dir2[i][0]][y + dir2[i][1]] == 1) {
                block* temp = new block(x + dir2[i][0], y + dir2[i][1]);
                openlist.push_back(temp);
                map[x + dir2[i][0]][y + dir2[i][1]] = 2;
        }
    }
}

void reDarw(void) { //复杂度n*m
    for (int y = 0; y <= BLOCK_HEIGHT; y++) {
        for (int x = 0; x <= BLOCK_WIDTH; x++) {
            if (map[x][y] == 1) {
                blockColor(x, y, wal);
            } else if (map[x][y] == 2) {
                blockColor(x, y, wal);
            } else {
                blockColor(x, y, road);
            }
        }
    }
    FlushBatchDraw();
}

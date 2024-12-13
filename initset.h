#pragma once

enum Info {
	WIDTH = 1920,//分辨率
	HEIGHT = 1080,
	BLOCK_XY = 12,//小方格边长
	REF = 35//刷新速度
};

const int BLOCK_WIDTH = WIDTH / (BLOCK_XY);//小方格x轴数量
const int BLOCK_HEIGHT = HEIGHT / (BLOCK_XY);//小方格y轴数量 
const int dir1[][2] = { {0,1},{0,-1},{1,0},{-1,0} };
const int dir2[][2] = { {0,2},{0,-2},{2,0},{-2,0} };

#define road COLORREF RGB(220,220,220)//路面颜色
#define wal COLORREF RGB(30,30,30)//墙壁颜色
#define beginPoint COLORREF RGB(0,255,0)//起点颜色
#define endPoint COLORREF RGB(255,0,0)//终点颜色
#define selected COLORREF RGB(190,50,50)//选中颜色
#define shortestPath COLORREF RGB(255,255,0)//最短路径颜色
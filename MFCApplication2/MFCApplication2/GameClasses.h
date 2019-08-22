//
//存放所有的自定义游戏类

#pragma once

using namespace std;
#include <vector>
//#include <cassert>
#include "GameClasses.h"
#include "stdafx.h"
#include "afxdialogex.h"

#define BORDER_WIDTH 3
#define BIRD_INIT_CENTERX 0
#define BIRD_INIT_CENTERY 100
#define BIRD_INIT_WIDTH 100
#define BIRD_INIT_HEIGHT 80

#define PIPEUP_INIT_CENTERX 350
#define PIPEUP_INIT_CENTERY 300
#define PIPEUP_INIT_WIDTH 100
#define PIPE_MIN_HEIGHT 100
#define PIPEDOWN_INIT_CENTERX 350
#define PIPEDOWN_INIT_CENTERY 50
#define PIPEDOWN_INIT_WIDTH 100

#define PIPE_EXPECT_BIRD_EXTRA_INTERVALY 50 
#define PIPE_SPEED 10
#define UPDATE_INTERVAL 40 //ms
#define BIRD_MOVEY_SPEED 20

//点, 包含x和y坐标
struct Point
{
	int x;
	int y;
	Point()
	{
		;
	}
	Point(int inputX, int inputY)
	{
		x = inputX;
		y = inputY;
	}
};

//尺寸, 包含高度和宽度
struct Size
{
	int width;
	int height;
	Size()
	{
		;
	}
	Size(int inputWidth, int inputHeight)
	{
		width = inputWidth;
		height = inputHeight;
	}
};

//基类, 矩形对象
class RectangleObject
{
protected:
	Point m_point;
	Size m_size;
public:
	RectangleObject();
	RectangleObject(Point inputPoint, Size inputSize);
	~RectangleObject();
	void SetPoint(Point input);
	Point GetPoint();
	void SetSize(Size input);
	Size GetSize();
	bool IsOverlapped(RectangleObject* anotherRec);
};

//鸟
//单例模式, 首次用到才加载
class Bird : public RectangleObject
{
private:
	static Bird * m_Bird_pInstance;
	Bird();
	~Bird();
public:
	static Bird * GetInstance();
	void Destroy();
};

//管道
class Pipe : public RectangleObject
{
private:
	bool IsLocationUp; //上方的
public:
	Pipe(Point inputPoint, Size inputSize, bool isUp, int maxY);
	~Pipe();
	void autoGenerateY(int maxY);
	void setIsLocationUp(bool isUp);
};

//地图
class Map
{
private:
	vector<Pipe*> m_pipes;
	Bird* m_bird;
	int m_minX;
	int m_minY;
	int m_maxX;
	int m_maxY;
	void fillBackground(CDC* pDC, CRect &rectPicture);
	void drawBird(CDC *pDC, CRect &rectPicture);
	void Map::drawPipe(CDC *pDC, CRect &rectPicture);
public:
	Map();
	~Map();
	void init();
	void update();
	void draw(CDC *pDC, CRect &rectPicture);
	void remove();
	bool isGameFinished();
	void addBirdY();
	void minusBirdY();
	void setMaxMinXY(int minX, int minY, int maxX, int maxY);
};

//画图, 全部都是静态方法, 不保存任何变量
class SimpleDraw
{
public:
	static vector<Point> SimpleDraw::FitBorderRectangle(Point inputPoint, 
		Size inputSize, int maxx, int minx, int maxy, int miny);
};
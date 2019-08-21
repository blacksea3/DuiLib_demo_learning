//
//������е��Զ�����Ϸ��

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
#define PIPEUP_INIT_HEIGHT 20
#define PIPEDOWN_INIT_CENTERX 350
#define PIPEDOWN_INIT_CENTERY 50
#define PIPEDOWN_INIT_WIDTH 100
#define PIPEDOWN_INIT_HEIGHT 20

#define PIPE_SPEED 10

//��, ����x��y����
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

//�ߴ�, �����߶ȺͿ��
struct Size
{
	int height;
	int width;
	Size()
	{
		;
	}
	Size(int inputHeight, int inputWidth)
	{
		height = inputHeight;
		width = inputWidth;
	}
};

//����, ���ζ���
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

//��
//����ģʽ, �״��õ��ż���
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

//�ܵ�
class Pipe : public RectangleObject
{
public:
	Pipe(Point inputPoint, Size inputSize);
	~Pipe();
};

//��ͼ
class Map
{
private:
	vector<Pipe*> m_pipes;
	Bird* m_bird;
public:
	Map();
	~Map();
	void init();
	void update();
	void draw(CDC *pDC, CRect &rectPicture);
	void remove();
	bool isGameFinished();
};

//��ͼ, ȫ�����Ǿ�̬����, �������κα���
class SimpleDraw
{
public:
	static vector<Point> SimpleDraw::FitBorderRectangle(Point inputPoint, 
		Size inputSize, int maxx, int minx, int maxy, int miny);
};
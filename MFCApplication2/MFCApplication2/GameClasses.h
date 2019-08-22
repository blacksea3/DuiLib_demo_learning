//
//������е��Զ�����Ϸ��

#pragma once

using namespace std;
#include <vector>
//#include <cassert>
#include "GameClasses.h"
#include "stdafx.h"
#include "afxdialogex.h"

#define BIRD_BMP_FILE "../Resources/bmp/bird.bmp"
#define BIRD_INIT_CENTERX 0
#define BIRD_INIT_CENTERY 100
#define BIRD_INIT_WIDTH 50
#define BIRD_INIT_HEIGHT 50

#define PIPE_BMP_FILE "../Resources/bmp/pipe.bmp"
#define PIPE_INIT_CENTERX 350
#define PIPE_INIT_WIDTH 20
#define PIPE_MIN_HEIGHT 100

#define PIPE_EXPECT_BIRD_EXTRA_INTERVALY 50 
#define PIPE_SPEED 10
#define UPDATE_INTERVAL 40 //ms
#define BIRD_MOVEY_SPEED 20
#define PIPE_ADD_INTERVAL 25 //��ʾ���ٸ�UPDATE_INTERVAL����ֹ���


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
private:
	bool IsLocationUp; //�Ϸ���
public:
	Pipe(Point inputPoint, Size inputSize, bool isUp, int maxY);
	~Pipe();
	void autoGenerateY(int maxY);
	void setIsLocationUp(bool isUp);
};

//��ͼ
class Map
{
private:
	vector<Pipe*> m_pipes;
	Bird* m_bird;
	int m_minX;
	int m_minY;
	int m_maxX;
	int m_maxY;
	int m_timeCount;

	void fillBackground(CDC* pDC, CRect &rectPicture);
	void drawBird(CDC *pDC, CRect &rectPicture);
	void drawPipe(CDC *pDC, CRect &rectPicture);
	void setClipRegion(CDC *pDC);

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
	void drawTest(CDC *pDC, CRect &rectPicture, UINT pictureResource);
};

//��ͼ, ȫ�����Ǿ�̬����, �������κα���
class SimpleDraw
{
public:
	static vector<Point> SimpleDraw::FitBorderRectangle(Point inputPoint, 
		Size inputSize, int maxx, int minx, int maxy, int miny);
	static vector<int> SimpleDraw::GenerateFourPoints(Point inputPoint,
		Size inputSize, int maxy);
};
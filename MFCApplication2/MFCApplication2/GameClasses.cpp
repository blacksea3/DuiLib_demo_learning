#include "stdafx.h"
#include "afxdialogex.h"
#include "GameClasses.h"
#include <random>

//基类, 矩形对象
RectangleObject::RectangleObject()
{
	;
}

RectangleObject::RectangleObject(Point inputPoint, Size inputSize)
{
	m_point = inputPoint;
	m_size = inputSize;
}

RectangleObject::~RectangleObject()
{
	;
}

void RectangleObject::SetPoint(Point input)
{
	m_point = input;
}

Point RectangleObject::GetPoint()
{
	return m_point;
}

void RectangleObject::SetSize(Size input)
{
	m_size = input;
}

Size RectangleObject::GetSize()
{
	return m_size;
}

bool RectangleObject::IsOverlapped(RectangleObject* anotherRec)
{
	Point anotherPoint = anotherRec->GetPoint();
	Size anotherSize = anotherRec->GetSize();

	int deltaX = abs(m_point.x - anotherPoint.x);
	int deltaY = abs(m_point.y - anotherPoint.y);
	int halfSumHeight = (m_size.height + anotherSize.height) / 2;
	int halfSumWidth = (m_size.width + anotherSize.width) / 2;

	return ((deltaX <= halfSumWidth) && (deltaY <= halfSumHeight));
}

//鸟
//单例模式, 首次用到才加载
Bird::Bird()
{
	;
}

Bird::~Bird()
{

}

Bird* Bird::GetInstance()
{
	if (m_Bird_pInstance == NULL)
		m_Bird_pInstance = new Bird();
	return m_Bird_pInstance;
}

void Bird::Destroy()
{
	if (m_Bird_pInstance != NULL)
	{
		delete m_Bird_pInstance;
		m_Bird_pInstance = NULL;
	}
}

Bird *Bird::m_Bird_pInstance = NULL;  //静态类变量类外定义/初始化

//管道
Pipe::Pipe(Point inputPoint, Size inputSize, bool isUp, int maxY) :RectangleObject(inputPoint, inputSize)
{
	IsLocationUp = isUp;
	autoGenerateY(maxY);
}
Pipe::~Pipe()
{

}

void Pipe::autoGenerateY(int maxY)
{
	Point pipePoint = GetPoint();
	Size pipeSize = GetSize();
	if (IsLocationUp)
	{
		pipePoint.y = maxY - pipeSize.height / 2;
		SetPoint(pipePoint);
	}
	else
	{
		pipePoint.y = pipeSize.height / 2;
		SetPoint(pipePoint);
	}
}

void Pipe::setIsLocationUp(bool isUp)
{
	IsLocationUp = isUp;
}

//地图
Map::Map()
{
	;
}

Map::~Map()
{
	;
}

void Map::init()
{
	m_bird = Bird::GetInstance();
	Point newPoint(BIRD_INIT_CENTERX, BIRD_INIT_CENTERY);
	m_bird->SetPoint(newPoint);

	Size newSize(BIRD_INIT_WIDTH, BIRD_INIT_HEIGHT);
	m_bird->SetSize(newSize);
}

void Map::update()
{
	if (m_pipes.empty())
	{
		Size birdSize = m_bird->GetSize();

		std::default_random_engine e;
		e.seed(unsigned(time(0)));
		uniform_int_distribution<unsigned> u(PIPE_MIN_HEIGHT, m_maxY - PIPE_MIN_HEIGHT);
		int randomPipeHeight = u(e);

		Point newPoint(PIPEUP_INIT_CENTERX, 0);
		Size newSize(PIPEUP_INIT_WIDTH, randomPipeHeight);
		Pipe* upPipe = new Pipe(newPoint, newSize, true, m_maxY);
		m_pipes.push_back(upPipe);

		Point newPoint_2(PIPEDOWN_INIT_CENTERX, 0);
		Size newSize_2(PIPEDOWN_INIT_WIDTH, max(0, m_maxY - randomPipeHeight - PIPE_EXPECT_BIRD_EXTRA_INTERVALY - birdSize.height));
		Pipe* downPipe = new Pipe(newPoint_2, newSize_2, false, m_maxY);
		m_pipes.push_back(downPipe);
	}
	else
	{
		//理论上两个Pipe的x将同时为0
		//故将会一起删除
		Pipe* upPipe = m_pipes[0];
		Pipe* downPipe = m_pipes[1];

		Point upPoint = upPipe->GetPoint();
		Size upSize = upPipe->GetSize();

		Point downPoint = downPipe->GetPoint();
		Size downSize = downPipe->GetSize();
		if (upPoint.x == 0)
		{
			delete upPipe;
			delete downPipe;
			m_pipes.clear();
		}
		else
		{
			upPoint.x -= PIPE_SPEED;
			downPoint.x -= PIPE_SPEED;
			upPipe->SetPoint(upPoint);
			downPipe->SetPoint(downPoint);
		}
	}
}

bool Map::isGameFinished()
{
	for (auto& pipe : m_pipes)
	{
		if (m_bird->IsOverlapped(pipe)) return true;
	}
	return false;
}

void Map::addBirdY()
{
	Point currentBirdPoint = m_bird->GetPoint();
	Size currentBirdSize = m_bird->GetSize();
	if (currentBirdPoint.y + currentBirdSize.height / 2 + BIRD_MOVEY_SPEED <= m_maxY)
	{
		currentBirdPoint.y += BIRD_MOVEY_SPEED;
		m_bird->SetPoint(currentBirdPoint);
	}
}

void Map::minusBirdY()
{
	Point currentBirdPoint = m_bird->GetPoint();
	Size currentBirdSize = m_bird->GetSize();
	if (currentBirdPoint.y - currentBirdSize.height / 2 - BIRD_MOVEY_SPEED >= m_minY)
	{
		currentBirdPoint.y -= BIRD_MOVEY_SPEED;
		m_bird->SetPoint(currentBirdPoint);
	}
}

void Map::setMaxMinXY(int minX, int minY, int maxX, int maxY)
{
	m_minX = minX;
	m_minY = minY;
	m_maxX = maxX;
	m_maxY = maxY;
}

void Map::fillBackground(CDC* pDC, CRect &rectPicture)
{
	CBrush newBrush;   // 用于创建新画刷
	CBrush *pOldBrush; // 用于存放旧画刷
	//清空背景(即背景涂白色)
	// 创建黑色新画刷   
	newBrush.CreateSolidBrush(RGB(255, 255, 255));
	// 选择新画刷，并将旧画刷的指针保存到pOldBrush   
	pOldBrush = pDC->SelectObject(&newBrush);
	// 以黑色画刷为绘图控件填充黑色，形成黑色背景   
	pDC->Rectangle(rectPicture);
	// 恢复旧画刷   
	pDC->SelectObject(pOldBrush);
	// 删除新画刷   
	newBrush.DeleteObject();
}

void Map::drawBird(CDC *pDC, CRect &rectPicture)
{
	//画鸟
	CPen newBirdPen;           // 用于创建新画笔
	CPen *pOldBirdPen;         // 用于存放旧画笔
	newBirdPen.CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
	// 选择新画笔，并将旧画笔的指针保存到pOldPen   
	pOldBirdPen = pDC->SelectObject(&newBirdPen);
	// 用四个点连线
	Point birdPoint = m_bird->GetPoint();
	Size birdSize = m_bird->GetSize();
	vector<Point> birdLocations = SimpleDraw::FitBorderRectangle(birdPoint, birdSize,
		rectPicture.Width(), 0, rectPicture.Height(), 0);
	pDC->MoveTo(birdLocations[0].x, birdLocations[0].y);
	for (int index = 1; index < 4; ++index)
	{
		pDC->LineTo(birdLocations[index].x, birdLocations[index].y);
	}
	pDC->LineTo(birdLocations[0].x, birdLocations[0].y);
	// 恢复旧画笔   
	pDC->SelectObject(pOldBirdPen);
	// 删除新画笔   
	newBirdPen.DeleteObject();
}

void Map::drawPipe(CDC *pDC, CRect &rectPicture)
{
	//画管道
	CPen newPipePen;           // 用于创建新画笔
	CPen *pOldPipePen;         // 用于存放旧画笔
	newPipePen.CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	// 选择新画笔，并将旧画笔的指针保存到pOldPen   
	pOldPipePen = pDC->SelectObject(&newPipePen);
	for (auto& pipe : m_pipes)
	{
		// 用四个点连线
		Point PipePoint = pipe->GetPoint();
		Size PipeSize = pipe->GetSize();
		vector<Point> pipeLocations = SimpleDraw::FitBorderRectangle(PipePoint, PipeSize,
			rectPicture.Width(), 0, rectPicture.Height(), 0);
		pDC->MoveTo(pipeLocations[0].x, pipeLocations[0].y);
		for (int index = 1; index < 4; ++index)
		{
			pDC->LineTo(pipeLocations[index].x, pipeLocations[index].y);
		}
		pDC->LineTo(pipeLocations[0].x, pipeLocations[0].y);
	}
	// 恢复旧画笔   
	pDC->SelectObject(pOldPipePen);
	// 删除新画笔   
	newPipePen.DeleteObject();
}

void Map::draw(CDC *pDC, CRect &rectPicture)
{
	fillBackground(pDC, rectPicture);
	drawBird(pDC, rectPicture);
	drawPipe(pDC, rectPicture);
}

void Map::remove()
{
	if (!m_pipes.empty())
	{
		delete m_pipes[1];
		delete m_pipes[0];
		m_pipes.clear();
		m_bird->Destroy();
		m_bird = nullptr;
	}
}

//限制矩形坐标
//输入input:中心点, 长宽, 最大/最小x值, 最大/最小y值
//将自动边缘裁剪BORDER_WIDTH, 即:实际限制范围比设定更小
//输出四个点(已经上下翻转)
//假定输入的矩形和给定限制范围有交集, 若不是, 则可能出现意外的情况
vector<Point> SimpleDraw::FitBorderRectangle(Point inputPoint, 
	Size inputSize, int maxx, int minx, int maxy, int miny)
{
	int leftX = max(minx + BORDER_WIDTH, inputPoint.x - inputSize.width / 2);
	int rightX = min(maxx - BORDER_WIDTH, inputPoint.x + inputSize.width / 2);
	int downY = maxy - max(miny + BORDER_WIDTH, inputPoint.y - inputSize.height / 2);
	int upY = maxy - min(maxy - BORDER_WIDTH, inputPoint.y + inputSize.height / 2);
	Point downLeft(leftX, downY);
	Point downRight(rightX, downY);
	Point upLeft(leftX, upY);
	Point upRight(rightX, upY);
	return { downLeft, downRight, upRight, upLeft };
}



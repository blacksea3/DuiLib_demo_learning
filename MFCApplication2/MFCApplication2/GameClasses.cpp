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
	m_timeCount = 0;

	m_bird = Bird::GetInstance();
	Point newPoint(BIRD_INIT_CENTERX, BIRD_INIT_CENTERY);
	m_bird->SetPoint(newPoint);

	Size newSize(BIRD_INIT_WIDTH, BIRD_INIT_HEIGHT);
	m_bird->SetSize(newSize);
}

void Map::update()
{
	if (m_timeCount%PIPE_ADD_INTERVAL == 0) //加一个Up管道, 加一个Down管道
	{
		Size birdSize = m_bird->GetSize();

		std::default_random_engine e;
		e.seed(unsigned(time(0)));
		uniform_int_distribution<unsigned> u(PIPE_MIN_HEIGHT, m_maxY - PIPE_MIN_HEIGHT);
		int randomPipeHeight = u(e);

		Point newPoint(PIPE_INIT_CENTERX, 0);
		Size newSize(PIPE_INIT_WIDTH, randomPipeHeight);
		Pipe* upPipe = new Pipe(newPoint, newSize, true, m_maxY);
		m_pipes.push_back(upPipe);

		Point newPoint_2(PIPE_INIT_CENTERX, 0);
		Size newSize_2(PIPE_INIT_WIDTH, max(0, m_maxY - randomPipeHeight - PIPE_EXPECT_BIRD_EXTRA_INTERVALY - birdSize.height));
		Pipe* downPipe = new Pipe(newPoint_2, newSize_2, false, m_maxY);
		m_pipes.push_back(downPipe);
	}

	//更新管道们的位置
	for (vector<Pipe*>::iterator pipeIter = m_pipes.begin(); pipeIter != m_pipes.end();)
	{
		Point pipePoint = (*pipeIter)->GetPoint();
		Size pipeSize = (*pipeIter)->GetSize();
		if (pipePoint.x == 0)
		{
			pipeIter = m_pipes.erase(pipeIter);
		}
		else
		{
			pipePoint.x -= PIPE_SPEED;
			(*pipeIter)->SetPoint(pipePoint);
			pipeIter++;
		}
	}
	m_timeCount++;
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
	/* 用长方形代替, 已废除
	//画鸟
	CPen newBirdPen;           // 用于创建新画笔
	CPen *pOldBirdPen;         // 用于存放旧画笔
	newBirdPen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
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
	*/

	//获取图片
	char *path = BIRD_BMP_FILE;
	CString strpath = path;
	CImage image;
	image.Load(LPCTSTR(strpath));

	//计算鸟画图位置
	SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);

	Point birdPoint = m_bird->GetPoint();
	Size birdSize = m_bird->GetSize();
	vector<int> birdLocations = SimpleDraw::GenerateFourPoints(birdPoint, birdSize, m_maxY);
	CRect drawPic;
	drawPic = CRect(birdLocations[0], birdLocations[1], birdLocations[2], birdLocations[3]);

	//画图
	image.Draw(pDC->m_hDC, drawPic);
	image.Destroy();
}

void Map::drawTest(CDC *pDC, CRect &rectPicture, UINT pictureResource)
{
	char *path = "../Resources/bmp/test.bmp";
	CString strpath = path;

	//你可以通过下列两行弄清楚当前文件绝对路径
	char pFileName[MAX_PATH];
	GetModuleFileName(NULL, pFileName, MAX_PATH);

	CImage image;
	image.Load(LPCTSTR(strpath));

	//以下两个矩形主要作用是，获取对话框上面的Picture Control的width和height，
	//并设置到图片矩形rectPicture，根据图片矩形rectPicture对图片进行处理，
	//最后绘制图片到对话框上Picture Control上面
	CRect rectPicture2, rectPicture3;                        //图片矩形对象

	int x = image.GetWidth();
	int y = image.GetHeight();
	SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);

	rectPicture2 = CRect(0, 0, 100, 100);
	rectPicture3 = CRect(300, 300, 500, 500);

	//((CStatic*)GetDlgItem(IDC_IMAGE))->SetBitmap(NULL);

	//以下两种方法都可绘制图片
	image.StretchBlt(pDC->m_hDC, 0, 0, 100, 100, 0, 0, 100, 100, SRCCOPY);

	//image.StretchBlt(pDC->m_hDC, rectPicture2, SRCCOPY); //将图片绘制到Picture控件表示的矩形区域
	//image.Draw(pDC->m_hDC, rectPicture2);                //将图片绘制到Picture控件表示的矩形区域
	
	image.Draw(pDC->m_hDC, rectPicture3);                //将图片绘制到Picture控件表示的矩形区域
	image.Destroy();
	//pWnd->ReleaseDC(pDc);
}

void Map::setClipRegion(CDC *pDC)
{
	CRgn rgn;
	rgn.CreateRectRgn(m_minX, m_minY, m_maxX, m_maxY);
	pDC->SelectClipRgn(&rgn);
}

void Map::drawPipe(CDC *pDC, CRect &rectPicture)
{
	/* 用长方形代替, 已废除
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
	*/

	//获取图片
	char *path = PIPE_BMP_FILE;
	CString strpath = path;
	CImage image;
	image.Load(LPCTSTR(strpath));

	//计算管道画图位置
	SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);

	for (auto& pipe : m_pipes)
	{
		// 用四个点连线
		Point PipePoint = pipe->GetPoint();
		Size PipeSize = pipe->GetSize();
		vector<int> pipeLocations = SimpleDraw::GenerateFourPoints(PipePoint, PipeSize, m_maxY);
		CRect drawPic;
		drawPic = CRect(pipeLocations[0], pipeLocations[1], pipeLocations[2], pipeLocations[3]);
		image.Draw(pDC->m_hDC, drawPic);
	}
	image.Destroy();
}

void Map::draw(CDC *pDC, CRect &rectPicture)
{
	setClipRegion(pDC);
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
//输出四个点(已经上下翻转)
//假定输入的矩形和给定限制范围有交集, 若不是, 则可能出现意外的情况
vector<Point> SimpleDraw::FitBorderRectangle(Point inputPoint, 
	Size inputSize, int maxx, int minx, int maxy, int miny)
{
	int leftX = max(minx, inputPoint.x - inputSize.width / 2);
	int rightX = min(maxx, inputPoint.x + inputSize.width / 2);
	int downY = maxy - max(miny, inputPoint.y - inputSize.height / 2);
	int upY = maxy - min(maxy, inputPoint.y + inputSize.height / 2);
	Point downLeft(leftX, downY);
	Point downRight(rightX, downY);
	Point upLeft(leftX, upY);
	Point upRight(rightX, upY);
	return { downLeft, downRight, upRight, upLeft };
}

vector<int> SimpleDraw::GenerateFourPoints(Point inputPoint, Size inputSize, int maxy)
{
	int leftX = inputPoint.x - inputSize.width / 2;
	int rightX = inputPoint.x + inputSize.width / 2;
	int downY = maxy - inputPoint.y - inputSize.height / 2;
	int upY = maxy - inputPoint.y + inputSize.height / 2;

	return vector<int>{leftX, downY, rightX, upY};
}



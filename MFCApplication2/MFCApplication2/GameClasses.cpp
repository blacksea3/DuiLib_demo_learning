#include "stdafx.h"
#include "afxdialogex.h"
#include "GameClasses.h"

//����, ���ζ���
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

//��
//����ģʽ, �״��õ��ż���
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

Bird *Bird::m_Bird_pInstance = NULL;  //��̬��������ⶨ��/��ʼ��

//�ܵ�
Pipe::Pipe(Point inputPoint, Size inputSize) :RectangleObject(inputPoint, inputSize)
{
	;
}
Pipe::~Pipe()
{

}

//��ͼ
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
		Point newPoint(PIPEUP_INIT_CENTERX, PIPEUP_INIT_CENTERY);
		Size newSize(PIPEUP_INIT_WIDTH, PIPEUP_INIT_HEIGHT);
		Pipe* upPipe = new Pipe(newPoint, newSize);

		Point newPoint_2(PIPEDOWN_INIT_CENTERX, PIPEDOWN_INIT_CENTERY);
		Size newSize_2(PIPEDOWN_INIT_WIDTH, PIPEDOWN_INIT_HEIGHT);
		Pipe* downPipe = new Pipe(newPoint_2, newSize_2);
		m_pipes.push_back(upPipe);
		m_pipes.push_back(downPipe);
	}
	else
	{
		//����������Pipe��x��ͬʱΪ0
		//�ʽ���һ��ɾ��
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

void Map::draw(CDC *pDC, CRect &rectPicture)
{
	CBrush newBrush;   // ���ڴ����»�ˢ
	CBrush *pOldBrush; // ���ڴ�žɻ�ˢ
	//��ձ���(������Ϳ��ɫ)
	// ������ɫ�»�ˢ   
	newBrush.CreateSolidBrush(RGB(255, 255, 255));
	// ѡ���»�ˢ�������ɻ�ˢ��ָ�뱣�浽pOldBrush   
	pOldBrush = pDC->SelectObject(&newBrush);
	// �Ժ�ɫ��ˢΪ��ͼ�ؼ�����ɫ���γɺ�ɫ����   
	pDC->Rectangle(rectPicture);
	// �ָ��ɻ�ˢ   
	pDC->SelectObject(pOldBrush);
	// ɾ���»�ˢ   
	newBrush.DeleteObject();

	//����
	CPen newBirdPen;           // ���ڴ����»���
	CPen *pOldBirdPen;         // ���ڴ�žɻ���
	newBirdPen.CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
	// ѡ���»��ʣ������ɻ��ʵ�ָ�뱣�浽pOldPen   
	pOldBirdPen = pDC->SelectObject(&newBirdPen);
	// ���ĸ�������
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
	// �ָ��ɻ���   
	pDC->SelectObject(pOldBirdPen);
	// ɾ���»���   
	newBirdPen.DeleteObject();

	//���ܵ�
	CPen newPipePen;           // ���ڴ����»���
	CPen *pOldPipePen;         // ���ڴ�žɻ���
	newPipePen.CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	// ѡ���»��ʣ������ɻ��ʵ�ָ�뱣�浽pOldPen   
	pOldPipePen = pDC->SelectObject(&newPipePen);
	for (auto& pipe : m_pipes)
	{
		// ���ĸ�������
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
	// �ָ��ɻ���   
	pDC->SelectObject(pOldPipePen);
	// ɾ���»���   
	newPipePen.DeleteObject();
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

//���ƾ�������
//����input:���ĵ�, ����, ���/��Сxֵ, ���/��Сyֵ
//���Զ���Ե�ü�BORDER_WIDTH, ��:ʵ�����Ʒ�Χ���趨��С
//����ĸ���(�Ѿ����·�ת)
//�ٶ�����ľ��κ͸������Ʒ�Χ�н���, ������, ����ܳ�����������
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



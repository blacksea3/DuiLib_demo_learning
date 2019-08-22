#include "stdafx.h"
#include "afxdialogex.h"
#include "GameClasses.h"
#include <random>

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
	m_timeCount = 0;

	m_bird = Bird::GetInstance();
	Point newPoint(BIRD_INIT_CENTERX, BIRD_INIT_CENTERY);
	m_bird->SetPoint(newPoint);

	Size newSize(BIRD_INIT_WIDTH, BIRD_INIT_HEIGHT);
	m_bird->SetSize(newSize);
}

void Map::update()
{
	if (m_timeCount%PIPE_ADD_INTERVAL == 0) //��һ��Up�ܵ�, ��һ��Down�ܵ�
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

	//���¹ܵ��ǵ�λ��
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
}

void Map::drawBird(CDC *pDC, CRect &rectPicture)
{
	/* �ó����δ���, �ѷϳ�
	//����
	CPen newBirdPen;           // ���ڴ����»���
	CPen *pOldBirdPen;         // ���ڴ�žɻ���
	newBirdPen.CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
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
	*/

	//��ȡͼƬ
	char *path = BIRD_BMP_FILE;
	CString strpath = path;
	CImage image;
	image.Load(LPCTSTR(strpath));

	//������ͼλ��
	SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);

	Point birdPoint = m_bird->GetPoint();
	Size birdSize = m_bird->GetSize();
	vector<int> birdLocations = SimpleDraw::GenerateFourPoints(birdPoint, birdSize, m_maxY);
	CRect drawPic;
	drawPic = CRect(birdLocations[0], birdLocations[1], birdLocations[2], birdLocations[3]);

	//��ͼ
	image.Draw(pDC->m_hDC, drawPic);
	image.Destroy();
}

void Map::drawTest(CDC *pDC, CRect &rectPicture, UINT pictureResource)
{
	char *path = "../Resources/bmp/test.bmp";
	CString strpath = path;

	//�����ͨ����������Ū�����ǰ�ļ�����·��
	char pFileName[MAX_PATH];
	GetModuleFileName(NULL, pFileName, MAX_PATH);

	CImage image;
	image.Load(LPCTSTR(strpath));

	//��������������Ҫ�����ǣ���ȡ�Ի��������Picture Control��width��height��
	//�����õ�ͼƬ����rectPicture������ͼƬ����rectPicture��ͼƬ���д���
	//������ͼƬ���Ի�����Picture Control����
	CRect rectPicture2, rectPicture3;                        //ͼƬ���ζ���

	int x = image.GetWidth();
	int y = image.GetHeight();
	SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);

	rectPicture2 = CRect(0, 0, 100, 100);
	rectPicture3 = CRect(300, 300, 500, 500);

	//((CStatic*)GetDlgItem(IDC_IMAGE))->SetBitmap(NULL);

	//�������ַ������ɻ���ͼƬ
	image.StretchBlt(pDC->m_hDC, 0, 0, 100, 100, 0, 0, 100, 100, SRCCOPY);

	//image.StretchBlt(pDC->m_hDC, rectPicture2, SRCCOPY); //��ͼƬ���Ƶ�Picture�ؼ���ʾ�ľ�������
	//image.Draw(pDC->m_hDC, rectPicture2);                //��ͼƬ���Ƶ�Picture�ؼ���ʾ�ľ�������
	
	image.Draw(pDC->m_hDC, rectPicture3);                //��ͼƬ���Ƶ�Picture�ؼ���ʾ�ľ�������
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
	/* �ó����δ���, �ѷϳ�
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
	*/

	//��ȡͼƬ
	char *path = PIPE_BMP_FILE;
	CString strpath = path;
	CImage image;
	image.Load(LPCTSTR(strpath));

	//����ܵ���ͼλ��
	SetStretchBltMode(pDC->m_hDC, STRETCH_HALFTONE);

	for (auto& pipe : m_pipes)
	{
		// ���ĸ�������
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

//���ƾ�������
//����input:���ĵ�, ����, ���/��Сxֵ, ���/��Сyֵ
//����ĸ���(�Ѿ����·�ת)
//�ٶ�����ľ��κ͸������Ʒ�Χ�н���, ������, ����ܳ�����������
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



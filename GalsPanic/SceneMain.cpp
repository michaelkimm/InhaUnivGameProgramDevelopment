#include <Windows.h>
#include <tchar.h>
#include <iostream>

#include "SceneMain.h"
#include "figureDraw.h"
#include "MyGdiplus.h"
#include "User.h"
#include "Hole.h"

#pragma comment(lib, "msimg32.lib")


SceneMain::SceneMain() : SceneState() {}
SceneMain::SceneMain(HWND _hWnd) : SceneState(_hWnd)
{
	// ��� �̹��� ���
	hBackImage_ = (HBITMAP)LoadImage(NULL, TEXT("images/maple_background.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hBackImage_, sizeof(BITMAP), &bitBack_);

	hBackImage2_ = (HBITMAP)LoadImage(NULL, TEXT("images/purpleMap.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	GetObject(hBackImage2_, sizeof(BITMAP), &bitBack2_);

	// �߰� �ո� �� ���
	int cx = client_.right / 2;
	int cy = client_.bottom / 2;
	int rect_height = 50;
	int rect_width = 200;
	POINT left_up = POINT{ cx - rect_width / 2, cy - rect_height / 2 };
	
	hole_.emplace_back(left_up);
	hole_.emplace_back(POINT{ cx + rect_width / 2, cy - rect_height / 2 });
	hole_.emplace_back(POINT{ cx + rect_width / 2, cy + rect_height / 2 });
	hole_.emplace_back(POINT{ cx - rect_width / 2, cy + rect_height / 2 });

	// �÷��̾� ���
	user_ = new User(hWnd_, left_up.x, left_up.y, 3, SIZE{ 20, 20 }, SIZE{ 20, 20 });
	user_->SetData(hWnd_, left_up.x, left_up.y, 3, SIZE{ 20, 20 }, SIZE{ 20, 20 });
}

SceneMain::~SceneMain()
{
	DeleteObject(hDoubleBufferImage_);
	DeleteObject(hBackImage_);
}

int SceneMain::Update(SceneManager * _s)
{
	/*if (User::Instance()->CollisionHole(Hole::Instance()))
	{

	}*/
		//std::cout << "���� ����!\n";
	user_->Update();
	return 0;
}

int SceneMain::Input()
{
	// : >> ������Ʈ �ð�
	DWORD cur_time = GetTickCount();
	static DWORD prev_time = cur_time;
	if (prev_time - cur_time < 100)
		return -1;
	prev_time = cur_time;
	// <<

	// : >> Ű�� ���� �����̱�
	user_->UserMove();

	// : >> ������ ���� ��ġ ����
	user_->UserMovefix(hole_);

	// : >> ������ hole ��ȣ�ۿ�
	user_->UserMeetHole(hole_);

	// user_->Input();
	return 0;
}

int SceneMain::Input(SceneManager * _s, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	user_->Input(hWnd, message, wParam, lParam);
	return 0;
}

int SceneMain::Draw(HDC hdc)
{
	HDC hMemDC;
	HBITMAP hOldBitmap;

	HDC hMemDC2;
	HBITMAP hOldBitmap2;

	int w = client_.right;
	int h = client_.bottom;
	
	hMemDC = CreateCompatibleDC(hdc);		// ���� ���۸��� DC ����
	if (!hMemDC)
	{
		MessageBox(hWnd_, _T("Create CompatibleDC failed"), _T("Error"), MB_OK);
		return -1;
	}
	if (hDoubleBufferImage_ == NULL)
		hDoubleBufferImage_ = CreateCompatibleBitmap(hdc, w, h);			// DC�� ���� ȭ�� ���� ��Ʈ�� �ڵ� ����
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hDoubleBufferImage_);		// ���� ���۸��� DC�� ���� ��Ʈ�� ���� ����

	// Gdi_Draw(hMemDC);
	// : >> ��� �׸���
	{
		// �̹��� �ø���
		hMemDC2 = CreateCompatibleDC(hMemDC);
		hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hBackImage_);
		int width = bitBack_.bmWidth;
		int height = bitBack_.bmHeight;

		// �̹��� �׸���
		BitBlt(hMemDC, 0, 0, width, height, hMemDC2, 0, 0, SRCCOPY);
		
		SelectObject(hMemDC2, hOldBitmap2);
		DeleteDC(hMemDC2);
	}
	// <<
	// : >> ���� �� �׸���
	{
		// �ۺ� �а� �׸���
		hMemDC2 = CreateCompatibleDC(hMemDC);
		hOldBitmap2 = (HBITMAP)SelectObject(hMemDC2, hBackImage2_);

		// ���� ���� ����
		hBrush_ = CreateSolidBrush(RGB(255, 0, 255));
		hOldBrush_ = (HBRUSH)SelectObject(hMemDC2, hBrush_);

		// ���� �׸���
		Polygon(hMemDC2, &hole_[0], hole_.size());

		int width = bitBack2_.bmWidth;
		int height = bitBack2_.bmHeight;

		// �̹��� �׸���
		//BitBlt(hMemDC, 0, 0, width, height, hMemDC2, 0, 0, SRCCOPY);
		TransparentBlt(hMemDC, 0, 0, width, height, hMemDC2, 0, 0, width, height, RGB(255, 0, 255));

		SelectObject(hMemDC2, hOldBitmap2);
		DeleteDC(hMemDC2);
	}
	// <<
	 
	// : >> �÷��̾� �׸���
	{
		user_->Draw(hMemDC);
	}
	// <<

	BitBlt(hdc, 0, 0, w, h, hMemDC, 0, 0, SRCCOPY);
	
	SelectObject(hMemDC, hOldBitmap);
	DeleteObject(hMemDC);
	return 0;
}

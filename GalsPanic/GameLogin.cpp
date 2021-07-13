#include "GameLogin.h"
#include "GamePlay.h"

#include "figureDraw.h"


GameLogin::GameLogin(HWND _hWnd) : GameState(_hWnd)
{
	CreateCaret(_hWnd, NULL, 5, 15);
}

GameLogin::~GameLogin()
{
	DestroyCaret();
	if ()
}

int GameLogin::Draw(HDC hdc)
{
	int w = client_.right;
	int h = client_.bottom;
	POINT c{ w / 2, h / 2 };
	DrawInputText(hdc, c, w / 3, h / 6, str_, DT_TOP | DT_LEFT | DT_SINGLELINE, true);
	return 0;
}

int GameLogin::Update()
{
	return 0;
}

int GameLogin::Input(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		break;
	case WM_CHAR:
		switch (wParam)
		{
		case VK_BACK:
			if (strlen_ > 0)	// 
				strlen_ -= 1;
			str_[strlen_] = '\0';
			break;
		case VK_RETURN:
			// ¥Ÿ¿Ω æ¿ »£√‚
			Next();
			break;
		default:
			if (strlen_ <= STRLENMAX - 2)
			{
				str_[strlen_++] = wParam;
				str_[strlen_] = '\0';
			}
			break;
		}
		break;
	default:
		return 0;
	}
	return 1;
}

int GameLogin::Next()
{
	GameState* tmp = new GamePlay(hWnd_);
	delete next_;
	next_ = tmp;

	return 0;
}

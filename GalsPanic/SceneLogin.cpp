#include "SceneLogin.h"
#include "SceneManager.h"
#include "figureDraw.h"

SceneLogin::SceneLogin() : SceneState()
{
	CreateCaret(hWnd_, NULL, 5, 15);
}
SceneLogin::SceneLogin(HWND _hWnd) : SceneState(_hWnd)
{
	CreateCaret(hWnd_, NULL, 5, 15);
}

SceneLogin::~SceneLogin()
{
	DestroyCaret();
}

int SceneLogin::Update(SceneManager* _s)
{
	// Login에는 업데이트가 없음.
	return 0;
}

int SceneLogin::Input()
{
	return 0;
}

int SceneLogin::Input(SceneManager* s, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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
			// 다음 씬 호출
			ChangeState(s, SceneManager::MAIN);
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

int SceneLogin::Draw(HDC hdc)
{
	// 중간에 아이디 입력 창 그리기
	int typing_width = client_.right / 4;
	int typing_height = client_.bottom / 30;

	DrawInputText(hdc, POINT{ (client_.right + client_.left) / 2, (client_.bottom + client_.top) / 2 },
					typing_width, typing_height, str_, DT_TOP | DT_LEFT | DT_SINGLELINE, true);
	return 0;
}

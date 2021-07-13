#include "SceneManager.h"
#include "SceneLogin.h"
#include "SceneMain.h"
//#include "SceneScore.h"

SceneManager::SceneManager()
{
	state_ = NULL;
}

SceneManager::SceneManager(HWND _hWnd_manager)
{
	hWnd_manager_ = _hWnd_manager;
}

SceneManager::~SceneManager()
{
	if (state_ != NULL)
		delete state_;
}


void SceneManager::SetSceneState(int _scene_type)
{
	delete state_;
	switch (_scene_type)
	{
	case LOGIN:
		state_ = new SceneLogin(hWnd_manager_);
		break;
	case MAIN:
		state_ = new SceneMain(hWnd_manager_);
		break;
	case SCORE:
		//state_ = new SceneScore();
		break;
	default:
		break;
	}
}

void SceneManager::Update()
{
	state_->Update(this);
}

void SceneManager::Input()
{
	state_->Input();
}

void SceneManager::Input(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	state_->Input(this, _hWnd, _message, _wParam, _lParam);
}

void SceneManager::Draw(HDC _hdc)
{
	state_->Draw(_hdc);
}

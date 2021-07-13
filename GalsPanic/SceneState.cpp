#include "SceneState.h"

SceneState::SceneState(HWND _hWnd) : hWnd_(_hWnd)
{
	GetClientRect(_hWnd, &client_);
}

void SceneState::ChangeState(SceneManager *_s, int _type)
{
	_s->SetSceneState(_type);

}
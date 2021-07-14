#include <Windows.h>
#include "InputManager.h"

InputManager* InputManager::input_manager_ = 0;

InputManager::InputManager() {}
InputManager::~InputManager()
{
	if (input_manager_ != nullptr)
		delete input_manager_;
}

InputManager * InputManager::Instance()
{
	if (input_manager_ == nullptr)
		input_manager_ = new InputManager();
	return input_manager_;
}

int InputManager::Update()
{
	DWORD cur_time = GetTickCount();
	static DWORD prev_time = cur_time;
	if (prev_time - cur_time < 16)
		return 0;

	UpdateKeyState();

	return 0;
}

int InputManager::UpdateKeyState()
{
	// 키 초기화
	left_pressed_ = 0;
	up_pressed_ = 0;
	right_pressed_ = 0;
	down_pressed_ = 0;
	space_pressed_ = 0;

	// 키 업데이트
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		left_pressed_ = 1;
	}
	if (GetKeyState(VK_UP) & 0x8000)
	{
		up_pressed_ = 1;
	}
	if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		right_pressed_ = 1;
	}
	if (GetKeyState(VK_DOWN) & 0x8000)
	{
		down_pressed_ = 1;
	}
	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		space_pressed_ = 1;
	}
	return 0;
}

int InputManager::GetLeftKeyState()
{
	return left_pressed_;
}

int InputManager::GetUpKeyState()
{
	return up_pressed_;
}

int InputManager::GetRightKeyState()
{
	return right_pressed_;
}

int InputManager::GetDownKeyState()
{
	return down_pressed_;
}

int InputManager::GetSpaceKeyState()
{
	return space_pressed_;
}

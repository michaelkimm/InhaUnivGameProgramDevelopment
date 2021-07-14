#pragma once
#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

class InputManager
{
private:
	int left_pressed_ = 0;
	int up_pressed_ = 0;
	int right_pressed_ = 0;
	int down_pressed_ = 0;
	int space_pressed_ = 0;

	static InputManager* input_manager_;
protected:
	InputManager();
	~InputManager();
public:
	static InputManager* Instance();

	int Update();
	int UpdateKeyState();

	int GetLeftKeyState();
	int GetUpKeyState();
	int GetRightKeyState();
	int GetDownKeyState();

	int GetSpaceKeyState();

};

#endif




#include "GamePadXbox.h"

bool GamePadXbox::is_connected()
{
	// clean the state
	memset(&_controllerState, 0, sizeof(XINPUT_STATE));

	// Get the state
	DWORD Result = XInputGetState(_playerIndex, &_controllerState);

	if (Result == ERROR_SUCCESS)	return true;
	else return false;
}

void GamePadXbox::vibrate(float leftmotor, float rightmotor)
{
	// Create a new Vibraton 
	XINPUT_VIBRATION Vibration;

	memset(&Vibration, 0, sizeof(XINPUT_VIBRATION));

	int leftVib = (int)(leftmotor*65535.0f);
	int rightVib = (int)(rightmotor*65535.0f);

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = leftVib;
	Vibration.wRightMotorSpeed = rightVib;
	// Vibrate the controller
	XInputSetState(_playerIndex, &Vibration);
}

void GamePadXbox::update()
{
	State.reset();
	// The values of the Left and Right Triggers go from 0 to 255. We just convert them to 0.0f=>1.0f
	if (_controllerState.Gamepad.bRightTrigger && _controllerState.Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		State._right_trigger = _controllerState.Gamepad.bRightTrigger / 255.0f;
	}

	if (_controllerState.Gamepad.bLeftTrigger && _controllerState.Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		State._left_trigger = _controllerState.Gamepad.bLeftTrigger / 255.0f;
	}

	// Get the Buttons
	if (_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) State._buttons[GamePad_Button_A] = true;
	if (_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B) State._buttons[GamePad_Button_B] = true;
	if (_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X) State._buttons[GamePad_Button_X] = true;
	if (_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) State._buttons[GamePad_Button_Y] = true;
	if (_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) State._buttons[GamePad_Button_DPAD_DOWN] = true;
	if (_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) State._buttons[GamePad_Button_DPAD_UP] = true;
	if (_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) State._buttons[GamePad_Button_DPAD_LEFT] = true;
	if (_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) State._buttons[GamePad_Button_DPAD_RIGHT] = true;
	if (_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) State._buttons[GamePad_Button_START] = true;
	if (_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) State._buttons[GamePad_Button_BACK] = true;
	if (_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) State._buttons[GamePad_Button_LEFT_THUMB] = true;
	if (_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) State._buttons[GamePad_Button_RIGHT_THUMB] = true;
	if (_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) State._buttons[GamePad_Button_LEFT_SHOULDER] = true;
	if (_controllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) State._buttons[GamePad_Button_RIGHT_SHOULDER] = true;

	if ((_controllerState.Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
		_controllerState.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
		(_controllerState.Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
			_controllerState.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		_controllerState.Gamepad.sThumbLX = 0;
		_controllerState.Gamepad.sThumbLY = 0;
	}

	// Check left thumbStick
	float leftThumbY = _controllerState.Gamepad.sThumbLY;
	if (leftThumbY)
	{
		State._left_thumbstick.y = leftThumbY;
	}
	float leftThumbX = _controllerState.Gamepad.sThumbLX;
	if (leftThumbX)
	{
		State._left_thumbstick.x = leftThumbX;
	}

	if ((_controllerState.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
		_controllerState.Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
		(_controllerState.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
			_controllerState.Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		_controllerState.Gamepad.sThumbRX = 0;
		_controllerState.Gamepad.sThumbRY = 0;
	}

	// Check left thumbStick
	float rightThumbY = _controllerState.Gamepad.sThumbRY;
	if (rightThumbY)
	{
		State._right_thumbstick.y = rightThumbY;
	}
	float rightThumbX = _controllerState.Gamepad.sThumbRX;
	if (rightThumbX)
	{
		State._right_thumbstick.x = rightThumbX;
	}

	for (int i = 0; i < GamePadButton_Max; ++i)
	{
		currStatus.set(i, this->State._buttons[i]);
	}
	if (_controllerState.Gamepad.sThumbLX > 80)
		currStatus.set(GamePad_Button_DPAD_RIGHT, true);
	if (_controllerState.Gamepad.sThumbLX < -80)
		currStatus.set(GamePad_Button_DPAD_LEFT, true);
	if (_controllerState.Gamepad.sThumbLY > 80)
		currStatus.set(GamePad_Button_DPAD_UP, true);
	if (_controllerState.Gamepad.sThumbLY < -80)
		currStatus.set(GamePad_Button_DPAD_DOWN, true);
}

bool GamePadXbox::IsKeyDown(int _slot)
{
	return currStatus.test(_slot);
}

bool GamePadXbox::IsKeyUp(int _slot)
{
	return !currStatus.test(_slot);
}

bool GamePadXbox::IsKeyPressed(int _slot)
{
	return IsKeyDown(_slot) && !prevStatus.test(_slot);
}

bool GamePadXbox::IsKeyReleased(int _slot)
{
	return IsKeyUp(_slot) && prevStatus.test(_slot);
}

void GamePadXbox::EndFrameUpdate()
{
	prevStatus = currStatus;
}
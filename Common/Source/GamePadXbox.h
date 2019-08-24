// stolen from http://www.david-amador.com/2012/04/xbox-360-controller-input-in-c-via-xinput/ yeet
#include "SingletonTemplate.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <XInput.h>
#include "Vector2.h"
#include <bitset>
#pragma comment(lib, "XInput.lib") 


class GamePadXbox : public Singleton <GamePadXbox>
{
public:
	typedef enum
	{
		GamePad_Button_DPAD_UP = 0,
		GamePad_Button_DPAD_DOWN = 1,
		GamePad_Button_DPAD_LEFT = 2,
		GamePad_Button_DPAD_RIGHT = 3,
		GamePad_Button_START = 4,
		GamePad_Button_BACK = 5,
		GamePad_Button_LEFT_THUMB = 6,
		GamePad_Button_RIGHT_THUMB = 7,
		GamePad_Button_LEFT_SHOULDER = 8,
		GamePad_Button_RIGHT_SHOULDER = 9,
		GamePad_Button_A = 10,
		GamePad_Button_B = 11,
		GamePad_Button_X = 12,
		GamePad_Button_Y = 13,
		GamePadButton_Max = 14
	}GamePadButton;

	struct GamePadState
	{
		bool		_buttons[GamePadButton_Max];
		Vector2 	_left_thumbstick;
		Vector2	    _right_thumbstick;
		float		_left_trigger;
		float		_right_trigger;
		void reset()
		{
			for (int i = 0; i < (int)GamePadButton_Max; ++i) _buttons[i] = false;
			_left_thumbstick.Set(0.0f, 0.0f);
			_right_thumbstick.Set(0.0f, 0.0f);
			_left_trigger = _right_trigger = 0.0f;
		}
	};

	GamePadXbox()
	{
		_playerIndex = 0;
		State.reset();
	}

	virtual ~GamePadXbox(void)
	{
		if (is_connected()) vibrate(0.0f, 0.0f);
	}

	bool is_connected();
	void vibrate(float leftmotor = 0.0f, float rightmotor = 0.0f);
	void update();
	bool IsKeyUp(int _slot);
	bool IsKeyDown(int _slot);
	bool IsKeyPressed(int _slot);
	bool IsKeyReleased(int _slot);
	void EndFrameUpdate();

	GamePadState State;
private:
	XINPUT_STATE _controllerState;
	int _playerIndex;
	std::bitset<GamePadButton_Max> currStatus, prevStatus;

};
#pragma once
#include "SpriteEntity.h"
#include "TextEntity.h"
class UIButton
{
public:
	UIButton();
	~UIButton();
	void SetText(std::string text);
	void SetPosition(Vector3 position);
	void SetSelected(bool selection);
	void RenderUI();
private:
	bool b_IsSelected;
	float fontSize;
	SpriteEntity* notSelected;
	SpriteEntity* selected;
	TextEntity* textObj;
};


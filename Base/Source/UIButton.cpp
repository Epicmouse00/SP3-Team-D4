#include "UIButton.h"



UIButton::UIButton()
	: b_IsSelected(false)
	, fontSize(16.f)
{
	notSelected = Create::Sprite2DObject("UI_BOX",
		Vector3(0, 0, 0.0f),
		Vector3(fontSize, fontSize, 0.0f));

	selected = Create::Sprite2DObject("UI_BOX2",
		Vector3(0, 0, 0.0f),
		Vector3(fontSize, fontSize, 0.0f));

	textObj = Create::Text2DObject("text",
		Vector3(0, 0, 0.0f),
		"",
		Vector3(fontSize, fontSize, fontSize), Color(1.0f, 0.0f, 0.0f));

}


UIButton::~UIButton()
{
	delete notSelected;
	notSelected = NULL;
	delete selected;
	selected = NULL;
}

void UIButton::SetText(std::string text)
{
	textObj->SetText(text);//get x-scale then scale the 2 quads accordingly
	selected->SetScale(Vector3(text.size()*fontSize, fontSize-2, 0));
	notSelected->SetScale(Vector3(text.size()*fontSize, fontSize-2, 0));
	SetPosition(textObj->GetPosition());
}

void UIButton::SetPosition(Vector3 position)
{
	textObj->SetPosition(position);
	selected->SetPosition(Vector3(position.x + textObj->GetText().size()*fontSize/2 + fontSize/4, position.y, position.z));
	notSelected->SetPosition(Vector3(position.x + textObj->GetText().size()*fontSize/2 + fontSize/4, position.y, position.z));
}

void UIButton::SetSelected(bool selection)
{
	b_IsSelected = selection;
}

void UIButton::RenderUI()
{
	if (b_IsSelected)
		selected->RenderUI();
	else
		notSelected->RenderUI();
	textObj->RenderUI();
}

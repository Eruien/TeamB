#pragma once
#include "MonoBehaviour.h"
enum class BUTTON_STATE
{
	NONE,
	HOVER,
	PRESS,
	HOLD,
	UP
};
class Button : public MonoBehaviour
{
public:
	Button();
	~Button();
	virtual void Init() override;
	virtual void Frame() override;

protected:
	BUTTON_STATE _buttonState;
};


#pragma once
#include "PWM.h"

namespace SeeSaw
{
	class DAC : public PWM
	{
	  public:
		DAC(SeeSaw & _host);
	};
}
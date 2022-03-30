#include "heart.h"

Heart::Heart()
{
	menu = new Menu();
}

void Heart::init()
{
	menu->initialize(L"AssaultCube");
}

void Heart::pump()
{
	menu->render();
}

// MouseEvent.cpp
//
// (C) 2011 Mike Allgyer.  All Rights Reserved.
//
// Represents a mouse event
//

#include "MouseEvent.h"

MouseEvent::MouseEvent(EventType type, int x, int y, int extraHiData, int extraLoData)
{
	mType = type;
	mX = x;
	mY = y;
	mExtraHiData = extraHiData;
	mExtraLoData = extraLoData;
	mConsumed = false;
}

MouseEvent::EventType MouseEvent::getEvent()
{
	return mType;
}
int MouseEvent::getX()
{
	return mX;
}
int MouseEvent::getY()
{
	return mY;
}

int MouseEvent::getExtraHiData()
{
	return mExtraHiData;
}
int MouseEvent::getExtraLoData()
{
	return mExtraLoData;
}
void MouseEvent::setConsumed(bool consumed)
{
	mConsumed = consumed;
}
bool MouseEvent::getConsumed()
{
	return mConsumed;
}
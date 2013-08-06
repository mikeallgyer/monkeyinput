// MouseEvent.h
//
// (C) 2011 Mike Allgyer.  All Rights Reserved.
//
// Represents a mouse event
//

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>


#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

class MouseEvent
{
public:
	static enum EventType { 
		LBUTTONDOWN = WM_LBUTTONDOWN, 
		LBUTTONUP = WM_LBUTTONUP, 
		LBUTTONDBLCLK = WM_LBUTTONDBLCLK, 
		RBUTTONDOWN = WM_RBUTTONDOWN,
		RBUTTONUP = WM_RBUTTONUP, 
		RBUTTONDBLCLK = WM_RBUTTONDBLCLK, 
		MOUSEWHEEL = WM_MOUSEWHEEL,
		MOUSEMOVE = WM_MOUSEWHEEL + 1
	};

	MouseEvent(EventType type, int x, int y, int extraHiData, int extraLoData);

	EventType getEvent();
	int getX();
	int getY();
	int getExtraHiData();
	int getExtraLoData();

	void setConsumed(bool consumed);
	bool getConsumed();
private:
	EventType mType;
	int mX;
	int mY;
	int mExtraHiData;  // context-sensitive
	int mExtraLoData;  // context-sensitive
	bool mConsumed;
};

#endif
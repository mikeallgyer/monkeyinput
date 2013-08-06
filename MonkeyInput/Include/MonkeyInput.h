// MonkeyInput.h
//
// (C) 2011 Mike Allgyer.  All Rights Reserved.
//
// Main input class..There should be only one instance of this class
//
#define DIRECTINPUT_VERSION 0x0800

//#define _AFXDLL
//#include <afxmt.h>
//#include <Afxwin.h>

#include <dinput.h>
#include <list>
#include <map>
#include <vector>

#include "MouseEvent.h"

#ifndef MONKEY_INPUT_H
#define MONKEY_INPUT_H


#define MONKEY_NONEXCLUSIVE DISCL_NONEXCLUSIVE 
#define MONKEY_EXCLUSIVE DISCL_EXCLUSIVE 
#define MONKEY_FOREGROUND DISCL_FOREGROUND
#define MONKEY_BACKGROUND DISCL_BACKGROUND

class MonkeyInput
{
public:
	static const int MOUSE1 = 0;
	static const int MOUSE2 = 1;
	static const int MOUSE3 = 2;
	static const int MOUSE4 = 3;
	static const int MOUSE5 = 4;
	static const int MOUSE6 = 5;
	static const int MOUSE7 = 6;
	static const int MOUSE8 = 7;

	static const int MOUSE_SCROLLUP = 20;
	static const int MOUSE_SCROLLDOWN = 21;

	MonkeyInput(HWND hwnd);
	~MonkeyInput();

	void setHwnd(HWND hwnd);
	void poll();
	bool isKeyDown(unsigned char key);
	bool keyPressed(unsigned char key);
	bool isMouseButtonDown(int button);
	int mouseDX();
	int mouseDY();
	int currMouseX();
	int currMouseY();
	// true if caller should ignore message
	bool processMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	// these keys will be deected regardless of whether the app is minimized/maximized/etc
	void registerGlobalKey(unsigned char key);
	void unregisterGlobalKey(unsigned char key);

	void addMouseCallback(void* obj, void (*callback) (void* ptrObj, MouseEvent e));

private:
	// no copying
	MonkeyInput(const MonkeyInput& copy);
	MonkeyInput& operator=(const MonkeyInput& copy);

	short mKeyboardState[256];
	short mPrevKeyboardState[256];
	bool mSwitchPrev;

	bool mMouseButtons[8];
	bool mPrevMouseButtons[8];

	int mCurrMouseX;
	int mCurrMouseY;
	int mCurrMouseDX;
	int mCurrMouseDY;

	std::list<unsigned char> mGlobalKeys;
	bool mAppActive;

	// callbacks
	std::vector<void (*)(void* ptrObj, MouseEvent e) > mMouseCallbacks;
	std::vector<void*> mMouseCallbackObj; 

	HWND mHwnd;
};

#endif
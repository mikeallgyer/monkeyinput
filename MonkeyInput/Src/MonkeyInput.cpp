// MonkeyInput.h
//
// (C) 2011 Mike Allgyer.  All Rights Reserved.
//
// Main input class..There should be only one instance of this class
//
#include <dinput.h>
#include <dxerr.h>
#include <windows.h>
#include <windowsx.h>

#include "../Include/MonkeyInput.h"
#include "../Include/MonkeyUtil.h"
#include "../Include/MouseEvent.h"


MonkeyInput::MonkeyInput(HWND hwnd)
{
	mHwnd = hwnd;
	mCurrMouseX = 0;
	mCurrMouseY = 0;
	mCurrMouseDX = 0;
	mCurrMouseDY = 0;
	// create keyboard
	for (int i = 0; i < 256; i++)
	{
		mKeyboardState[i] = false;
		mPrevKeyboardState[i] = false;
	}
	
	for (int i = 0; i < 8; i++)
	{
		mMouseButtons[i] = false;
		mPrevMouseButtons[i] = false;
	}

	// TODO: is this a safe assumption
	mAppActive = true;
}
void MonkeyInput::setHwnd(HWND hwnd)
{
	mHwnd = hwnd;
}
MonkeyInput::~MonkeyInput()
{
}

void MonkeyInput::poll()
{
	// poll keyboard
	memcpy(mPrevKeyboardState, mKeyboardState, 256 * sizeof(short));
	if (mAppActive)
	{
		// check all keys
		for (unsigned char i = 0; i < 256; i++)
		{
			mKeyboardState[i] = GetAsyncKeyState(i);
			if (i == 255)
			{
				break;
			}
		}
	}
	else
	{
		for (std::list<unsigned char>::iterator it = mGlobalKeys.begin(); it != mGlobalKeys.end(); ++it)
		{
			mKeyboardState[*it] = GetAsyncKeyState(*it);
		}
	}
}
bool MonkeyInput::isKeyDown(unsigned char key)
{
	short retValue = mKeyboardState[key];
	if(retValue != 0)
	{
		retValue = retValue >> 8;
		return (retValue & 0x80);
	}
	return false;
}

bool MonkeyInput::keyPressed(unsigned char key)
{
	short retValue = mKeyboardState[key];
	short prevValue = mPrevKeyboardState[key];
	if(retValue != 0)
	{
		retValue = retValue >> 8;
		prevValue = prevValue >> 8;
		return !(prevValue & 0x80) && (retValue & 0x80);
	}
	return false;
}
bool MonkeyInput::isMouseButtonDown(int button)
{
	return mMouseButtons[button];
}
int MonkeyInput::mouseDX()
{
	return mCurrMouseDX;
}
int MonkeyInput::mouseDY()
{
	return mCurrMouseDY;
}

int MonkeyInput::currMouseX()
{
	return mCurrMouseX;

}
int MonkeyInput::currMouseY()
{
	return mCurrMouseY;
}
bool MonkeyInput::processMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	mCurrMouseDX = 0;
	mCurrMouseDY = 0;

	int prevX = mCurrMouseX;
	int prevY = mCurrMouseY;
	tagPOINT p;
	MouseEvent* ev = NULL;
	switch (msg)
	{
		// mouse stuff
	case WM_MOUSEMOVE:
		if (GetCursorPos(&p))
		{
			ScreenToClient(mHwnd, &p);
			mCurrMouseX = p.x;//GET_X_LPARAM(lParam); 
			mCurrMouseY = p.y;//GET_Y_LPARAM(lParam);
			mCurrMouseDX = mCurrMouseX - prevX;
			mCurrMouseDY = mCurrMouseY - prevY;
		}
		ev = new MouseEvent(MouseEvent::MOUSEMOVE, mCurrMouseX, mCurrMouseY, 0, 0);
			break;
	case WM_LBUTTONDOWN:
		ev = new MouseEvent((MouseEvent::EventType)msg, mCurrMouseX, mCurrMouseY, HIWORD(wParam), LOWORD(wParam));
		mMouseButtons[MOUSE1] = true;
		break;
	case WM_LBUTTONUP:
		ev = new MouseEvent((MouseEvent::EventType)msg, mCurrMouseX, mCurrMouseY, HIWORD(wParam), LOWORD(wParam));
		mMouseButtons[MOUSE1] = false;
		break;
	case WM_LBUTTONDBLCLK:
		ev = new MouseEvent((MouseEvent::EventType)msg, mCurrMouseX, mCurrMouseY, HIWORD(wParam), LOWORD(wParam));
		break;
	case WM_RBUTTONDOWN:
		ev = new MouseEvent((MouseEvent::EventType)msg, mCurrMouseX, mCurrMouseY, HIWORD(wParam), LOWORD(wParam));
		mMouseButtons[MOUSE2] = true;
		break;
	case WM_RBUTTONUP:
		ev = new MouseEvent((MouseEvent::EventType)msg, mCurrMouseX, mCurrMouseY, HIWORD(wParam), LOWORD(wParam));
		mMouseButtons[MOUSE2] = false;
		break;
	case WM_RBUTTONDBLCLK:
		ev = new MouseEvent((MouseEvent::EventType)msg, mCurrMouseX, mCurrMouseY, HIWORD(wParam), LOWORD(wParam));
		break;
	case WM_MOUSEWHEEL:
		ev = new MouseEvent((MouseEvent::EventType)msg, mCurrMouseX, mCurrMouseY, GET_WHEEL_DELTA_WPARAM(wParam), LOWORD(wParam));
		break;
		// WM_ACTIVE is sent when the window is activated or deactivated.
	case WM_ACTIVATE:
		mAppActive = (LOWORD(wParam) != WA_INACTIVE);
		if (!mAppActive)
		{
			for (int i = 0; i < 256; i++)
			{
				mKeyboardState[i] = false;
				mPrevKeyboardState[i] = false;
			}
			
			for (int i = 0; i < 8; i++)
			{
				mMouseButtons[i] = false;
				mPrevMouseButtons[i] = false;
			}
		}
		return false; // false, so app can still use the message
	}

	if (ev != NULL)
	{
		for (unsigned int i = 0; i < mMouseCallbackObj.size(); i++)
		{
			mMouseCallbacks[i](mMouseCallbackObj[i], *ev);
		}
		delete ev;
	}
	return false;
}
	// these keys will be deected regardless of whether the app is minimized/maximized/etc
void MonkeyInput::registerGlobalKey(unsigned char key)
{
	unregisterGlobalKey(key);
	mGlobalKeys.push_back(key);
}
void MonkeyInput::unregisterGlobalKey(unsigned char key)
{
	mGlobalKeys.remove(key);
}

void MonkeyInput::addMouseCallback(void* obj, void (*callback) (void* ptrObj, MouseEvent e))
{
	mMouseCallbacks.push_back(callback);
	mMouseCallbackObj.push_back(obj);
}
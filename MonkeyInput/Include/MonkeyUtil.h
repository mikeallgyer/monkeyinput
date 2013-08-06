//
// (C) 2011 Mike Allgyer.  All Rights Reserved.
//
// Some code used from, or inspired by, Frank D. Luna http://www.d3dcoder.net/d3d9c.htm:
//
// d3dUtil.h by Frank Luna (C) 2005 All Rights Reserved.
//
// Contains various utility code for DirectX applications, such as, clean up
// and debugging code.

#ifndef D3DUTIL_H
#define D3DUTIL_H

// debug...may slow things down a bit
#if defined(DEBUG) | defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

#include<d3d9.h>
#include<d3dx9.h>
#include<dxerr.h>
#include<string>
#include<sstream>

// clean up macros
#define MonkeyReleaseCOM(x) { if (x) { x->Release(); x = 0; } }

// debug macro
#if defined(DEBUG) | defined(_DEBUG)
   #ifndef MonkeyHR
		#define MonkeyHR(x)							\
		{											\
			HRESULT hr = x;					\
			if (FAILED(hr))					\
			{										\
				DXTrace(__FILE__, __LINE__, hr, L#x, TRUE); \
			}										\
		}
	#endif
#else
	#ifndef MonkeyHR
		#define MonkeyHR(X) x;
	#endif
#endif
#endif
#ifndef __HACKBASE_H__
#define __HACKBASE_H__

//#define PUBLISHEDVERSION

#define LOGTOCONSOLE
#define LOGTOFILE "C:\\log.txt"
#define DEBUGLOG

#ifdef PUBLISHEDVERSION
#define LOGTOCONSOLE
#undef LOGTOFILE
#undef DEBUGLOG
#endif

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#pragma region Inlcudes
#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <Tlhelp32.h>
#include <vector>
#include <string>
#include <time.h>
#include <algorithm>

#pragma warning (push)
#pragma warning (disable: 4005)
#include <d3d9.h>
#include <d3dx9.h>
#include <d3d10_1.h>
#include <d3d10.h>
#include <d3dx10.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#pragma warning (pop)  
#pragma endregion

#pragma region Classes
class Color;
class D3D9Hook;
class D3D9Renderer;
class D3D10Renderer;
class D3D11Renderer;
class DXGIHook;
class HackBase;
class Imports;
class Point;
class Renderer;
class Size;
class String;
class Vector3;
#pragma endregion

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef std::vector<String> StringVector;

#include "log.h"
#include "datatypes.h"

#pragma region Hook includes
#include "d3d9hook.h"
#include "dxgihook.h"
#pragma endregion

#pragma region Renderer includes
#include "color.h"
#include "renderer.h"
#include "d3d9renderer.h"
#include "d3d10renderer.h"
#include "d3d11renderer.h"  
#pragma endregion

#pragma region Other includes
#include "tools.h"
#include "imports.h"
#include "detourxs.h"
#include "encryption.h"
#pragma endregion


#pragma region Defines
#if defined DEBUG | defined _DEBUG
#define DEBUG_BREAK __asm int 0x3
#else if
#define DEBUG_BREAK
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if((p)) { (p)->Release(); (p) = NULL; } }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(a) { if((a)) { delete (a); (a) = NULL; } }
#endif

#define CONCAT_IMPL(x, y) x##y
#define MACRO_CONCAT(x, y) CONCAT_IMPL(x, y)
#define PAD(SIZE) BYTE MACRO_CONCAT(_pad, __COUNTER__)[SIZE];

#define PI 3.14159265358979f

#define RAD2DEG(a) (((a) * 180.0f) / PI)
#define DEG2RAD(a) (((a) / 180.0f) * PI)  
#pragma endregion


class HackBase {
private:
	HWND mConsoleWindow;
	D3DVersion mD3DVersion;
	Imports *mImports;
	Renderer *mRenderer;

	HackBase();

public:
	D3D9Hook *mD3D9Hook;
	DXGIHook *mDXGIHook;

	void (__cdecl *onRender)(Renderer*);
	void (__cdecl *mOnRender)(Renderer*);

	static HackBase *Singleton();

	bool Initialize(void (__cdecl *onRender)(Renderer*) = NULL, const String ExecuteableFile = NULL, const String WindowName = NULL, const String WindowClass = NULL);
	~HackBase();

	HWND getConsoleWindow() { return this->mConsoleWindow; }
	D3DVersion getD3DVersion() { return this->mD3DVersion; }
	void setD3DVersion(D3DVersion Version) { if(this->mD3DVersion == D3DVersion_None) this->mD3DVersion = Version; }
	void cleanUpD3DHooks();
	void setRenderer(Renderer *pRenderer) { if(!this->mRenderer) this->mRenderer = pRenderer; }
	Renderer *getRenderer() { return this->mRenderer; }

	bool isWindowHooked(String WindowName, String ClassName = 0);
};

static void onRenderEvent(Renderer *renderer);

#endif
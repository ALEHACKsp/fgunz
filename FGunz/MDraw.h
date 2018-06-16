#pragma once

#include <d3dx9.h>

struct MPOINT {
	int x, y;
public:
	MPOINT(void) {}
	MPOINT(int x, int y) { MPOINT::x = x, MPOINT::y = y; }

	void Scale(float x, float y);
	void ScaleRes(void);				///< 648*480���ذ��� ���� �ػ󵵿� ���� �����ϸ�
	void TranslateRes(void);			///< 648*480 �߽������� �Ÿ���ŭ ���� �ػ󵵿� �°� �̵�
};

struct MRECT {
	int	x, y;	///< ��ġ
	int w, h;	///< ũ��
public:
	MRECT(void) {}
	MRECT(int x, int y, int w, int h) {
		Set(x, y, w, h);
	}
	bool InPoint(MPOINT& p) {
		if (p.x >= x && p.x <= x + w && p.y >= y && p.y <= y + h) return true;
		return false;
	}
	void Set(int x, int y, int w, int h) {
		MRECT::x = x, MRECT::y = y;
		MRECT::w = w, MRECT::h = h;
	}

	void ScalePos(float x, float y);	///< �������� ���� �����ϸ�
	void ScaleArea(float x, float y);	///< �������� ���̿� ���� �����ϸ�
	void ScalePosRes(void);				///< 648*480���ذ��� ���� �ػ󵵿� ���� �����ϸ�
	void ScaleAreaRes(void);			///< 648*480���ذ��� ���� �ػ󵵿� ���� �����ϸ�
	void TranslateRes(void);			///< 648*480 �߽������� �Ÿ���ŭ ���� �ػ󵵿� �°� �̵�

	void EnLarge(int w);				///< �¿� ���ϸ� w��ŭ�� ũ�� Ű���
	void Offset(int x, int y);			///< ������ �̵�

	bool Intersect(MRECT* pIntersect, const MRECT& r);	///< �� �簢���� ���� ���� ����
};

#define MINT_ARGB(a,r,g,b)		( ((((unsigned long int)a)&0xFF)<<24) | ((((unsigned long int)r)&0xFF)<<16) | ((((unsigned long int)g)&0xFF)<<8) | (((unsigned long int)b)&0xFF) )
#define MINT_RGB(r,g,b)			( ((((unsigned long int)r)&0xFF)<<16) | ((((unsigned long int)g)&0xFF)<<8) | (((unsigned long int)b)&0xFF) )

struct MCOLOR {
public:
	unsigned char	r;		///< Red
	unsigned char	g;		///< Green
	unsigned char	b;		///< Blue
	unsigned char	a;		///< Alpha
public:
	MCOLOR(void) {
		r = g = b = a = 0;
	}
	MCOLOR(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) {
		MCOLOR::r = r, MCOLOR::g = g, MCOLOR::b = b, MCOLOR::a = a;
	}
	MCOLOR(unsigned long int argb) {
		a = unsigned char((argb & 0xFF000000) >> 24);
		r = unsigned char((argb & 0x00FF0000) >> 16);
		g = unsigned char((argb & 0x0000FF00) >> 8);
		b = unsigned char((argb & 0x000000FF));
	}

	unsigned long int GetARGB(void) {
		return MINT_ARGB(a, r, g, b);
	}
	unsigned long int GetRGB(void) {
		return MINT_RGB(r, g, b);
	}
};

#define MBITMAP_NAME_LENGTH		128
class MBitmap {
public:
#ifdef _DEBUG
	int		m_nTypeID;	// ���� ������ Ŭ����(MFont, MDrawContex)�� ����ϱ� ���� ID
#endif

public:
	char	m_szName[MBITMAP_NAME_LENGTH];
	DWORD	m_DrawMode;
public:
	MBitmap(void);
	virtual ~MBitmap(void);

	void CreatePartial(MBitmap *pBitmap, MRECT rt, const char *szName);		// partial bitmap ���� �����Ѵ�

	virtual bool Create(const char* szName);
	virtual void Destroy(void);

	virtual void SetDrawMode(DWORD md) { m_DrawMode = md; }
	virtual DWORD GetDrawMode() { return m_DrawMode; }

	virtual int GetX(void) { return 0; }
	virtual int GetY(void) { return 0; }
	virtual int GetWidth(void) = 0;
	virtual int GetHeight(void) = 0;

	virtual MBitmap *GetSourceBitmap() { return this; }
};

#define MFONT_NAME_LENGTH		32

// Font abstract class
class MFont {
public:
#ifdef _DEBUG
	int		m_nTypeID;
#endif

public:
	char	m_szName[MFONT_NAME_LENGTH];

	int				m_nOutlineStyle;
	unsigned long	m_ColorArg1;	// For Custom FontStyle like OutlineStyle
	unsigned long	m_ColorArg2;

public:
	MFont(void);
	virtual ~MFont(void);
	virtual bool Create(const char* szName);
	virtual void Destroy(void);
	virtual int GetHeight(void) = 0;
	virtual int GetWidth(const char* szText, int nSize = -1) = 0;
	virtual int GetWidthWithoutAmpersand(const char* szText, int nSize = -1);
	//	virtual int GetLineCount(int nRectWidth, const char* szText);		// dc �� TextMultiLine��ǰ� �������
	//	virtual int GetPossibleCharacterCount(int nRectWidth,const char* szText);	// nRectWidth �ȿ� ������ �ִ� ���ڼ�
	virtual bool Resize(float ratio, int nMinimumHeight) { return true; } // �� assert�� �ʿ�� ����. ���� �������� �����߿� ������ �� �ֵ��� ������
};

typedef void* MDrawEffect;
class MDrawContext {
protected:
#ifdef _DEBUG
	int		m_nTypeID;
#endif
	MCOLOR			m_Color;			///< ���� ���õǾ� �ִ� �÷�
	MCOLOR			m_HighlightColor;	///< ���� ���õǾ� �ִ� ���϶���Ʈ �÷�(&���� ������ ���� ���ڸ� ���϶��������ش�.)
	MCOLOR			m_ColorKey;			///< ��������Ʈ ����� ���� �÷�Ű
	MCOLOR			m_BitmapColor;	/// < ��Ʈ�ʿ� ���� �� (�⺻�� : White(255,255,255,255) )
	MBitmap*		m_pBitmap;			///< ���� ���õǾ� �ִ� ��Ʈ��
	MFont*			m_pFont;			///< ���� ���õǾ� �ִ� ��Ʈ
	MRECT			m_Clip;				///< ��ũ�� ��ǥ�迡���� Ŭ���� ����
	MPOINT			m_Origin;			///< ��ũ���� �׷����� ���� ���� ����
	MDrawEffect		m_Effect;			///< ȿ��
	unsigned char	m_nOpacity;
public:
	MDrawContext(void);
	virtual ~MDrawContext(void);

	// Color Setting
	MCOLOR SetBitmapColor(MCOLOR& color);
	MCOLOR SetBitmapColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	MCOLOR GetBitmapColor(void);
	MCOLOR SetColor(MCOLOR& color);
	MCOLOR SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	MCOLOR GetColor(void);
	MCOLOR SetHighlightColor(MCOLOR& color);
	MCOLOR SetHighlightColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	MCOLOR GetHighlightColor(void);
	MCOLOR SetColorKey(MCOLOR& color);
	MCOLOR SetColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
	MCOLOR GetColorKey(void);

	// Font Setting
	MFont* SetFont(MFont* pFont);
	MFont* GetFont() { return m_pFont; }

	// Bitmap Setting
	MBitmap* SetBitmap(MBitmap* pBitmap);
	MBitmap* GetBitmap() { return m_pBitmap; }

	// Clipping ( Screen Coordinate )
	virtual void SetClipRect(MRECT& r);
	void SetClipRect(int x, int y, int w, int h);
	MRECT GetClipRect(void);

	// Logical Origin
	void SetOrigin(int x, int y);
	void SetOrigin(MPOINT& p);
	MPOINT GetOrigin(void);

	// Draw Effect
	void SetEffect(MDrawEffect effect);
	MDrawEffect GetEffect() { return m_Effect; }

	// Basic Drawing Functions
	virtual void SetPixel(int x, int y, MCOLOR& color) = 0;
	virtual void HLine(int x, int y, int len) = 0;
	virtual void VLine(int x, int y, int len) = 0;
	virtual void Line(int sx, int sy, int ex, int ey) = 0;
	virtual void Rectangle(int x, int y, int cx, int cy);
	void Rectangle(MRECT& r);
	virtual void FillRectangle(int x, int y, int cx, int cy) = 0;
	virtual void FillRectangleW(int x, int y, int cx, int cy) = 0;
	void FillRectangle(MRECT& r);

	// Bitmap Drawing
	void Draw(int x, int y);
	void Draw(int x, int y, int w, int h);
	void Draw(int x, int y, int sx, int sy, int sw, int sh);
	void Draw(MPOINT &p);
	void Draw(MRECT& r);
	void Draw(int x, int y, MRECT& s);
	virtual void Draw(MRECT& d, MRECT& s);
	//# �̰� �������� ����ǵ�, ��� ������ ��� ����� �̹� �ֽ��ϴ� => void MBitmap::SetDrawMode(DWORD md) ����
	virtual void DrawInverse(int x, int y, int w, int h, bool bMirrorX, bool bMirrorY);
	virtual void DrawInverse(int x, int y, int w, int h, int sx, int sy, int sw, int sh, bool bMirrorX, bool bMirrorY);
	virtual void DrawEx(int tx1, int ty1, int tx2, int ty2,
		int tx3, int ty3, int tx4, int ty4);
	virtual void Draw(int x, int y, int w, int h, int sx, int sy, int sw, int sh);

	// Text
	virtual bool BeginFont() = 0;
	virtual bool EndFont() = 0;
	virtual int Text(int x, int y, const char* szText) = 0;
	int Text(MPOINT& p, const char* szText) { return Text(p.x, p.y, szText); }
	virtual int TextMultiLine(MRECT& r, const char* szText, int nLineGap = 0, bool bAutoNextLine = true, int nIndentation = 0, int nSkipLine = 0, MPOINT* pPositions = NULL);
	//virtual int TextMultiLine2(MRECT& r, const char* szText, int nLineGap = 0, bool bAutoNextLine = true, MAlignmentMode am = (MAM_HCENTER | MAM_VCENTER));

	/*
	/// nPos��° ���ڰ� ������ �����ġ�� ���Ѵ�
	static bool GetTextPosition(MPOINT *pOut,MFont *pFont,int nRectWidth, const char* szText,int nPos,bool bColorSupport=false);
	/// �����ġ�� ���° �������� ����
	static int GetCharPosition(MFont *pFont,int nX,int nLine,int nRectWidth,const char* szText,bool bColorSupport=false);
	*/

	/// &�� ����Ͽ� &�κ��� ���϶�����
	/*int TextWithHighlight(int x, int y, const char* szText);
	/// Ư�� �ؽ�Ʈ�� ���ĵǾ����� ���� ��ġ ���
	void GetPositionOfAlignment(MPOINT* p, MRECT& r, const char* szText, MAlignmentMode am, bool bAndInclude = true);
	int Text(MRECT& r, const char* szText, MAlignmentMode am = (MAM_HCENTER | MAM_VCENTER));
	int TextWithHighlight(MRECT& r, const char* szText, MAlignmentMode am = (MAM_HCENTER | MAM_VCENTER));

	// Multiple Color Support
	void TextMC(int x, int y, const char* szText);
	void TextMC(MRECT& r, const char* szText, MAlignmentMode am);*/

	/// Multiple Color ��ȣ�� ������ �ؽ�Ʈ�� ���Ѵ�.
	static char* GetPureText(const char *szText);

	/// Opacity�� �����Ѵ�.
	unsigned char SetOpacity(unsigned char nOpacity);
	unsigned char GetOpacity();

private:
	virtual void Draw(MBitmap *pBitmap, int x, int y, int w, int h, int sx, int sy, int sw, int sh, bool bMirrorX = false, bool bMirrorY = false) = 0;		// User Defined Draw Funtion
};


class MDrawContext;
class MDrawContextR2 : public MDrawContext {
public:
	LPDIRECT3DDEVICE9	m_pd3dDevice;
public:
	virtual ~MDrawContextR2(void);

	// Basic Drawing Functions
	virtual void SetPixel(int x, int y, MCOLOR& color);
	virtual void HLine(int x, int y, int len);
	virtual void VLine(int x, int y, int len);
	virtual void Line(int sx, int sy, int ex, int ey);
	virtual void Rectangle(int x, int y, int cx, int cy);
	virtual void FillRectangle(int x, int y, int cx, int cy);
	virtual void FillRectangleW(int x, int y, int cx, int cy);

	// Bitmap Drawing
	virtual void DrawEx(int tx1, int ty1, int tx2, int ty2, int tx3, int ty3, int tx4, int ty4);

	// Text
	virtual bool BeginFont();
	virtual bool EndFont();

	virtual int Text(int x, int y, const char* szText);

	virtual void SetClipRect(MRECT& r);

private:
	virtual void Draw(MBitmap *pBitmap, int x, int y, int w, int h, int sx, int sy, int sw, int sh, bool bMirrorX = false, bool bMirrorY = false);
};
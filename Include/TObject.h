#pragma once
#include "TBitmapManager.h"
#include "TCollision.h"

typedef std::vector<RECT>  RECT_ARRAY;

enum TObjectType {
	BACKGROUND = 0,
	UICONTROL,
	USER,
	NPC,
};
enum TControlState {
	DEFAULT = 0,
	PUSH,
	SELECT,
	DISABLE,
};
enum TOverlapState {
	OVERLAP_NONE = 0,
	OVERLAP_BEGIN,
	OVERLAPPING,
	OVERLAP_END,
};
struct TObjAttribute
{
	int iObjType;
	wstring szName;
	wstring colorbitmap;
	wstring pushbitmap;
	wstring selectbitmap;
	wstring disbitmap;
	wstring maskbitmap;
	TPoint  pos;
	RECT   rtSrc;
	RECT   rtDesk;
	bool   bColorKey;
	DWORD  dwColor;
	int	   iVisible;
	int	   iEnable;
};
struct TSpriteInfo
{
	wstring colorbitmap;
	wstring maskbitmap;
	RECT_ARRAY rtArray;
	wstring    szName;
};
class TObject
{
public:
	int			m_iIndex;
	DWORD		m_dwSvrID;
	int			m_iObjectType;	
	wstring     m_szName;
	TObjAttribute m_Attribute;
public:
	TBitmap*	m_pMaskBmp;
	TBitmap*	m_pColorBmp;
	std::vector<TBitmap*> m_StateBitmap;
	RECT		m_rtSrc;
	RECT		m_rtDesk;
	bool		m_bColorKey;
	DWORD		m_dwColorKey;
public:
	RECT		m_rtCollide;
	TPoint		m_ptPos;
	float		m_fSpeed;
	TPoint		m_ptDirection;
	TPoint		m_ptPreDirection;
	bool		m_bDead;
	bool		m_bSelect;
	float		m_fLifeTime;
	float		m_fAttackTimer;
public:
	/// object Index, overlap State
	std::map<int, DWORD>	m_OverlapState;
	DWORD					Overlap(int iIndex);
public:
	virtual TObject* Clone();
	virtual bool  Reset();
	virtual bool  Init();
	virtual bool  Frame();
	virtual bool  Render();
	virtual bool  Release();
	virtual void  Update();
	virtual void  SetDir(float* p);
	virtual void  SetPos(float* d);
	virtual void  SetDir(TPoint p);
	virtual void  SetPos(TPoint p);
	virtual void  Set(TPoint p, RECT rtSrc);

	virtual void  SetPosition(TPoint p);
	virtual void  SetSpeed(float fSpeed);
	virtual void  Set(RECT rtSrc, RECT rtDesk);
	virtual bool  Load( const TCHAR* color,
						const TCHAR* mask =nullptr,
						DWORD dwColor= RGB(255,0,255));
	virtual void  DrawColorKey();
public:
	virtual bool Damage() { return false; };
	virtual bool Dead() { return true; };
	virtual void Attack(TObject* pPlayer) {};
public:
	virtual void SetTransition(DWORD dwEevnt) {};
public:
	TObject();
	virtual ~TObject();
};


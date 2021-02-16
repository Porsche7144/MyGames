#pragma once
#include "HBitmapManager.h"
#include "HCollision.h"

typedef std::vector<RECT>  RECT_ARRAY;

enum HObjectType {
	BACKGROUND = 0,
	UICONTROL,
	USER,
	NPC,
};
enum HControlState {
	DEFAULT = 0,
	PUSH,
	SELECT,
	DISABLE,
};
enum HOverlapState {
	OVERLAP_NONE = 0,
	OVERLAP_BEGIN,
	OVERLAP_STAY,
	OVERLAP_END,
};
struct HObjAttribute
{
	int iObjType;
	wstring szName;
	wstring szParentName;
	wstring colorbitmap;
	wstring pushbitmap;
	wstring selectbitmap;
	wstring disbitmap;
	wstring maskbitmap;
	HPoint  pos;
	RECT   rtSrc;
	RECT   rtDesk;
	bool   bColorKey;
	DWORD  dwColor;
	int	   iVisible;
	int	   iEnable;
};
struct HSpriteInfo
{
	wstring		colorbitmap;
	wstring		maskbitmap;
	RECT_ARRAY	rtArray;
	wstring		szName;
	wstring		szParentName;
};
enum HCollisionType
{
	H_IGNORE = 0,		/// �����Ѵ�.
	H_OVERLAP,			/// ��ĥ �� �ִ�.
	H_BLOCKING			/// ��ø�� �� ���� ���ŷ �ȴ�.
};
enum HSelectState
{
	H_DEFAULT	= 0,  /// ���콺�� ���� ���� ��( T_FOCUS���¿��� �ٸ� ���� T_ACTIVE ���� ���°� �Ǿ��� �� ��ȯ�ȴ�.)
	H_HOVER		= 1,	/// ���콺�� ���� ���� ��	
	H_FOCUS		= 2,	/// T_ACTIVE���� ���콺�� ���� ��ư�� �ٸ� ������ ������ ��( �Ǵ� ��, ����Ű ���� ���, ��Ÿ ���)	
	H_ACTIVE	= 4,	/// ���콺 ���� ��ư�� ������ ���� ��(Ű ������ KEY_HOLD�� ����)	
	H_SELECTED	= 8, /// T_ACTIVE���� ���콺�� ���� ��ư�� ������ ������ ��( ���Ŀ��� T_FOCUS�� �ȴ�.)
};
class HObject
{
public:
	int			m_iIndex;			 /// ��� ������Ʈ�� �߱޵Ǵ� ������ �ε���	
	DWORD		m_dwSvrID;			 /// ������ ��� �� ������ �ε���
	int			m_iObjectType;	     /// TObjectType ������Ʈ Ÿ��
public:
	int			m_iCollisionObjectID;/// ������Ʈ �޴����� ��� �� �浹 �ε���
	int			m_iSelectObjectID;	 /// ������Ʈ �޴����� ��ϵ� ���콺 ���� �ε���
	bool		m_bCollisionEnabled; /// �浹ó������ �Ǵ�(���콺 ������ ó���ȴ�.)
	int			m_iCollisionType;	 /// TCollisionType�� ���� ó���Ѵ�.
	DWORD		m_dwOverlapState;	 /// ������ ����
	DWORD		m_dwSelectState;	 /// ���콺 ���� ����
	int			m_iImageState;
public:	
	wstring     m_szName;
	wstring		m_szParentName;
	HObject*	m_pParentObject;
	HObjAttribute m_Attribute;
	std::vector<HObject*>	m_pChildObjects;
public:
	RECT		m_rtSrc;		/// ���� �̹��� ����
	RECT		m_rtDesk;		/// �ʱ� ȭ�� ��ο� ����
	RECT		m_rtDraw;		/// ���� ��ο� ����
	RECT		m_rtCollide;	/// ���� �浹 ����
	bool		m_bColorKey;	/// �÷�Ű ��� ����
	DWORD		m_dwColorKey;	/// �÷�Ű �÷� ��
	int			m_iSortValue;	/// Z�� ���ĵ�
public:	
	HPoint		m_ptPos;
	float		m_fSpeed;
	HPoint		m_ptDirection;
	HPoint		m_ptPreDirection;
	bool		m_bDead;
	bool		m_bSelect;
	float		m_fLifeTime;
	float		m_fAttackTimer;
	float		m_fAlphaBlend;
	bool		m_bDrawDebugRect;
public:
	/// object Index, overlap State
	std::map<int, DWORD>	m_OverlapState;
	DWORD					Overlap(int iIndex);
	virtual void			HitOverlap(HObject* pDesk, DWORD dwState);
	virtual void			MouseOverlap(POINT ptMousepos, DWORD dwState);
public:
	virtual HObject* Clone();
	virtual bool  Reset();
	virtual bool  Init();
	virtual bool  PreFrame();
	virtual bool  Frame();
	virtual bool  PostFrame();
	virtual bool  PreRender();
	virtual bool  Render();
	virtual bool  PostRender();
	virtual bool  Release();
	virtual void  Update();
	virtual void  SetDir(float* p);
	virtual void  SetPos(float* d);
	virtual void  SetDir(HPoint p);
	virtual void  SetPos(HPoint p);
	virtual void  Set(HPoint p, RECT rtSrc);

	virtual void  SetPosTL(HPoint p);
	virtual void  SetSpeed(float fSpeed);
	virtual void  Set(RECT rtSrc, RECT rtDesk);
	virtual bool  Load( const TCHAR* color,
						const TCHAR* mask =nullptr,
						DWORD dwColor= RGB(255,0,255));
	virtual void  DrawColorKey();
public:
	virtual bool Damage() { return false; };
	virtual bool Dead() { return true; };
	virtual void Attack(HObject* pPlayer) {};
public:
	virtual void SetTransition(DWORD dwEevnt) {};
public:
	HObject();
	virtual ~HObject();
};


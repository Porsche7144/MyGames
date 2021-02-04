#pragma once
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <string>
#include <vector>
#pragma comment(lib,"odbc32.lib")

///
//ODBC ������ ����(64��Ʈ) ����̹� ��ġ
//��ġ���α׷�->Microsoft Access Database Engine 2010 ����� ���� ��Ű��
//https ://www.microsoft.com/ko-kr/download/details.aspx?id=13255
///

class TQuery
{
public:
	// ��� �÷��� ���� ��Ʈ������ �����Ѵ�.
	struct ColDescription
	{
		SQLUSMALLINT       icol;
		SQLWCHAR		   szColName[80];
		SQLSMALLINT        cchColNameMax;
		SQLSMALLINT        pcchColName;
		SQLSMALLINT        pfSqlType;
		SQLULEN            pcbColDef;
		SQLSMALLINT        pibScale;
		SQLSMALLINT        pfNullable;
		SQLWCHAR		   bindData[80];
		//TargetValuePtr ���ۿ��� ������ �� �ִ� ����Ʈ ���� ǥ���ϴ� ��(�Ǵ� �� �迭)�� ������
		SQLLEN			   byteSize; // bite
	};
public:
	// 1)�ڵ鼱��
	SQLHENV		m_hEnv = SQL_NULL_HENV; // ȯ���ڵ�
	SQLHDBC		m_hDbc = SQL_NULL_HDBC; // �����ڵ�
	SQLHSTMT	m_hStmt = SQL_NULL_HSTMT;// ����ڵ�
	SQLINTEGER  m_iRetRaram;
public:
	TCHAR		m_szUserID[32] = { 0, };
	TCHAR		m_szUserPS[32] = { 0, };
	SQLHSTMT	m_hLoginStmt = SQL_NULL_HSTMT;// ����ڵ�
	SQLHSTMT	m_hLoginStmtCall = SQL_NULL_HSTMT;// ����ڵ�
	SQLHSTMT	m_hAccountStmt = SQL_NULL_HSTMT;// ����ڵ�
	SQLHSTMT	m_hUserListStmt = SQL_NULL_HSTMT;// ����ڵ�
	int			m_iNameLength;
	SQLLEN		m_cbColumn = SQL_NTS;
	SQLLEN		m_cbColumnAccount[2];
	typedef std::vector< std::wstring> RECORD;	
	std::vector< ColDescription>	m_ColumnList;
	RECORD							m_ColumnResultList;
	std::vector< RECORD >			m_RecordResultList;
public:
	void		CreatePrepare();
	SQLRETURN	GetDescribeCol(SQLHSTMT stmt);
	SQLRETURN	GetColData(SQLHSTMT stmt, int column, std::wstring& rt);
	bool		GetRecordData(SQLHSTMT	hStmt);
	bool		ExecPrepare(SQLHSTMT stmt, bool bReturn=false);
	bool		ExecLogin(std::wstring szID);
	bool		ExecLogin(std::wstring szID, std::wstring szPass);
	bool		ExecAccount(std::wstring szID, std::wstring szPS);
public:
	bool	GetDiagRec(SQLHANDLE handle, SQLSMALLINT fHandleType = SQL_HANDLE_STMT);
	void	Check(SQLHSTMT stmt);
public:
	bool	Connect(int iType=0);
	bool	Exec(const TCHAR* sql);
	bool	ExecSelect(const TCHAR * sql, std::wstring& szID, std::wstring& szPS);
	bool	ExecUpdate(const TCHAR * sql);
	bool	ExecDelete(const TCHAR * sql);
	bool	ExecInsert(const TCHAR * sql);
public:
	virtual bool Init();
	virtual bool Release();
public:
	TQuery();
	virtual ~TQuery();
};


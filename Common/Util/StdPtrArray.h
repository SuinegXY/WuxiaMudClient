#ifndef _CStdPtrArray_h_
#define _CStdPtrArray_h_

class CStdPtrArray
{
public:
	CStdPtrArray(int iPreallocSize = 0);
	CStdPtrArray(const CStdPtrArray& src);
	~CStdPtrArray();

	void Empty();
	void Resize(int iSize);
	bool IsEmpty() const;
	int Find(LPVOID iIndex) const;
	bool Add(LPVOID pData);
	bool SetAt(int iIndex, LPVOID pData);
	bool InsertAt(int iIndex, LPVOID pData);
	bool Remove(int iIndex);
	int GetSize() const;
	LPVOID* GetData();

	LPVOID GetAt(int iIndex) const;
	LPVOID operator[] (int nIndex) const;

protected:
	LPVOID* m_ppVoid;
	int m_nCount;
	int m_nAllocated;
};

#endif
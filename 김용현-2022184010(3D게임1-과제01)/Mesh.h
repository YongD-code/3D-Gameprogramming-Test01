#pragma once

#include "Camera.h"
#include <memory>

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
class CVertex
{
public:
	CVertex() { }
	CVertex(float x, float y, float z) { m_xmf3Position = XMFLOAT3(x, y, z); }

	XMFLOAT3					m_xmf3Position;
};

class CPolygon
{
public:
	CPolygon(int nVertices);
	virtual ~CPolygon();

	int							m_nVertices;
	std::unique_ptr<CVertex[]>	m_pVertices;

	void SetVertex(int nIndex, const CVertex& vertex);
};

class CMesh
{
public:
	CMesh(int nPolygons);
	virtual ~CMesh();

private:
	int							m_nReferences;

	int							m_nPolygons;
	std::unique_ptr<std::unique_ptr<CPolygon>[]> m_ppPolygons;

public:
	BoundingOrientedBox			m_xmOOBB;

public:
	void AddRef() { m_nReferences++; }
	void Release() { m_nReferences--; if (m_nReferences <= 0) delete this; }

public:
	void SetPolygon(int nIndex, std::unique_ptr<CPolygon> pPolygon);
	virtual void Render(HDC hDCFrameBuffer, XMFLOAT4X4& xmf4x4World, CCamera *pCamera);

	BOOL RayIntersectionByTriangle(XMVECTOR& xmRayOrigin, XMVECTOR& xmRayDirection, XMVECTOR v0, XMVECTOR v1, XMVECTOR v2, float *pfNearHitDistance);
	int CheckRayIntersection(XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection, float *pfNearHitDistance);
};

class CCubeMesh : public CMesh
{
public:
	CCubeMesh(float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f);
	virtual ~CCubeMesh();
};

class CWallMesh : public CMesh
{
public:
	CWallMesh(float fWidth = 4.0f, float fHeight = 4.0f, float fDepth = 4.0f, int nSubRects = 20);
	virtual ~CWallMesh();
};

class CAirplaneMesh : public CMesh
{
public:
	CAirplaneMesh(float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f);
	virtual ~CAirplaneMesh();
};



#include "stdafx.h"
#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
		case WM_KEYDOWN:
			switch (wParam)
			{
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
				{
					int index = int(wParam - '1');

					if (index >= 0 && index < static_cast<int>(m_ppObjects.size()))
					{
						CExplosiveObject* pExplosiveObject =
							static_cast<CExplosiveObject*>(m_ppObjects[index].get());
						pExplosiveObject->m_bBlowingUp = true;
					}
					break;
				}
				case 'B':
					for (auto& object : m_ppObjects)
					{
						CExplosiveObject *pExplosiveObject = static_cast<CExplosiveObject *>(object.get());
						pExplosiveObject->m_bBlowingUp = true;
					}
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void CScene::BuildObjects()
{
	CExplosiveObject::PrepareExplosion();

	float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 110.0f;
	CWallMesh *pWallCubeMesh = new CWallMesh(fHalfWidth * 2.0f, fHalfHeight * 2.0f, fHalfDepth * 2.0f, 30);

	m_pWallsObject = std::make_unique<CWallsObject>();
	m_pWallsObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_pWallsObject->SetMesh(pWallCubeMesh);
	m_pWallsObject->SetColor(RGB(0, 128, 255));
	m_pWallsObject->m_pxmf4WallPlanes[0] = XMFLOAT4(+1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[1] = XMFLOAT4(-1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[2] = XMFLOAT4(0.0f, +1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[3] = XMFLOAT4(0.0f, -1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[4] = XMFLOAT4(0.0f, 0.0f, +1.0f, fHalfDepth);
	m_pWallsObject->m_pxmf4WallPlanes[5] = XMFLOAT4(0.0f, 0.0f, -1.0f, fHalfDepth);
	m_pWallsObject->m_xmOOBBPlayerMoveCheck = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth * 0.05f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CCubeMesh *pObjectCubeMesh = new CCubeMesh(4.0f, 4.0f, 4.0f);

	m_nObjects = 10;
	m_ppObjects.clear();
	m_ppObjects.reserve(m_nObjects);

	auto obj0 = std::make_unique<CExplosiveObject>();
	obj0->SetMesh(pObjectCubeMesh);
	obj0->SetColor(RGB(255, 0, 0));
	obj0->SetPosition(-13.5f, 0.0f, -14.0f);
	obj0->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 1.0f));
	obj0->SetRotationSpeed(90.0f);
	obj0->SetMovingDirection(XMFLOAT3(1.0f, 0.0f, 0.0f));
	obj0->SetMovingSpeed(10.5f);
	m_ppObjects.push_back(std::move(obj0));

	auto obj1 = std::make_unique<CExplosiveObject>();
	obj1->SetMesh(pObjectCubeMesh);
	obj1->SetColor(RGB(0, 0, 255));
	obj1->SetPosition(+13.5f, 0.0f, -14.0f);
	obj1->SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));
	obj1->SetRotationSpeed(180.0f);
	obj1->SetMovingDirection(XMFLOAT3(-1.0f, 0.0f, 0.0f));
	obj1->SetMovingSpeed(8.8f);
	m_ppObjects.push_back(std::move(obj1));	

	auto obj2 = std::make_unique<CExplosiveObject>();
	obj2->SetMesh(pObjectCubeMesh);
	obj2->SetColor(RGB(0, 255, 0));
	obj2->SetPosition(0.0f, +5.0f, 20.0f);
	obj2->SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));
	obj2->SetRotationSpeed(30.15f);
	obj2->SetMovingDirection(XMFLOAT3(1.0f, -1.0f, 0.0f));
	obj2->SetMovingSpeed(5.2f);
	m_ppObjects.push_back(std::move(obj2));

	auto obj3 = std::make_unique<CExplosiveObject>();
	obj3->SetMesh(pObjectCubeMesh);
	obj3->SetColor(RGB(0, 255, 255));
	obj3->SetPosition(0.0f, 0.0f, 0.0f);
	obj3->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 1.0f));
	obj3->SetRotationSpeed(40.6f);
	obj3->SetMovingDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));
	obj3->SetMovingSpeed(20.4f);
	m_ppObjects.push_back(std::move(obj3));

	auto obj4 = std::make_unique<CExplosiveObject>();
	obj4->SetMesh(pObjectCubeMesh);
	obj4->SetColor(RGB(128, 0, 255));
	obj4->SetPosition(10.0f, 0.0f, 0.0f);
	obj4->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	obj4->SetRotationSpeed(50.06f);
	obj4->SetMovingDirection(XMFLOAT3(0.0f, 1.0f, 1.0f));
	obj4->SetMovingSpeed(6.4f);
	m_ppObjects.push_back(std::move(obj4));

	auto obj5 = std::make_unique<CExplosiveObject>();
	obj5->SetMesh(pObjectCubeMesh);
	obj5->SetColor(RGB(255, 0, 255));
	obj5->SetPosition(-10.0f, 0.0f, -10.0f);
	obj5->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	obj5->SetRotationSpeed(60.06f);
	obj5->SetMovingDirection(XMFLOAT3(1.0f, 0.0f, 1.0f));
	obj5->SetMovingSpeed(8.9f);
	m_ppObjects.push_back(std::move(obj5));

	auto obj6 = std::make_unique<CExplosiveObject>();
	obj6->SetMesh(pObjectCubeMesh);
	obj6->SetColor(RGB(255, 0, 255));
	obj6->SetPosition(-10.0f, 10.0f, -10.0f);
	obj6->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	obj6->SetRotationSpeed(60.06f);
	obj6->SetMovingDirection(XMFLOAT3(1.0f, 1.0f, 1.0f));
	obj6->SetMovingSpeed(9.7f);
	m_ppObjects.push_back(std::move(obj6));

	auto obj7 = std::make_unique<CExplosiveObject>();
	obj7->SetMesh(pObjectCubeMesh);
	obj7->SetColor(RGB(255, 0, 128));
	obj7->SetPosition(-10.0f, 10.0f, -20.0f);
	obj7->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	obj7->SetRotationSpeed(70.06f);
	obj7->SetMovingDirection(XMFLOAT3(-1.0f, 1.0f, 1.0f));
	obj7->SetMovingSpeed(15.6f);
	m_ppObjects.push_back(std::move(obj7));	

	auto obj8 = std::make_unique<CExplosiveObject>();
	obj8->SetMesh(pObjectCubeMesh);
	obj8->SetColor(RGB(128, 0, 255));
	obj8->SetPosition(-15.0f, 10.0f, -30.0f);
	obj8->SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));
	obj8->SetRotationSpeed(90.06f);
	obj8->SetMovingDirection(XMFLOAT3(0.0f, 0.0f, -1.0f));
	obj8->SetMovingSpeed(15.0f);
	m_ppObjects.push_back(std::move(obj8));

	auto obj9 = std::make_unique<CExplosiveObject>();
	obj9->SetMesh(pObjectCubeMesh);
	obj9->SetColor(RGB(255, 64, 64));
	obj9->SetPosition(+15.0f, 10.0f, 0.0f);
	obj9->SetRotationAxis(XMFLOAT3(1.0f, 1.0f, 0.0f));
	obj9->SetRotationSpeed(90.06f);
	obj9->SetMovingDirection(XMFLOAT3(-0.0f, 0.0f, -1.0f));
	obj9->SetMovingSpeed(15.0f);
	m_ppObjects.push_back(std::move(obj9));
}

void CScene::ReleaseObjects()
{
	if (CExplosiveObject::m_pExplosionMesh) CExplosiveObject::m_pExplosionMesh->Release();

	m_ppObjects.clear();
	m_pWallsObject.reset();
}

CGameObject *CScene::PickObjectPointedByCursor(int xClient, int yClient, CCamera *pCamera)
{
	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / pCamera->m_d3dViewport.Width) - 1) / pCamera->m_xmf4x4Projection._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / pCamera->m_d3dViewport.Height) - 1) / pCamera->m_xmf4x4Projection._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmmtxView = XMLoadFloat4x4(&pCamera->m_xmf4x4View);

	int nIntersected = 0;
	float fNearestHitDistance = FLT_MAX;
	CGameObject *pNearestObject = NULL;


	for (auto& object : m_ppObjects)
	{
		float fHitDistance = FLT_MAX;
		nIntersected = object->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, &fHitDistance);
		if ((nIntersected > 0) && (fHitDistance < fNearestHitDistance))
		{
			fNearestHitDistance = fHitDistance;
			pNearestObject = object.get();
		}
	}
	return(pNearestObject);
}

void CScene::CheckObjectByObjectCollisions()
{
	for (size_t i = 0; i < m_ppObjects.size(); i++) m_ppObjects[i]->m_pObjectCollided = NULL;

	for (size_t i = 0; i < m_ppObjects.size(); i++)
	{
		for (size_t j = i + 1; j < m_ppObjects.size(); j++)
		{
			if (m_ppObjects[i]->m_xmOOBB.Intersects(m_ppObjects[j]->m_xmOOBB))
			{
				m_ppObjects[i]->m_pObjectCollided = m_ppObjects[j].get();
				m_ppObjects[j]->m_pObjectCollided = m_ppObjects[i].get();
			}
		}
	}
	for (int i = 0; i < m_nObjects; i++)
	{
		if (m_ppObjects[i]->m_pObjectCollided)
		{
			XMFLOAT3 xmf3MovingDirection = m_ppObjects[i]->m_xmf3MovingDirection;
			float fMovingSpeed = m_ppObjects[i]->m_fMovingSpeed;
			m_ppObjects[i]->m_xmf3MovingDirection = m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection;
			m_ppObjects[i]->m_fMovingSpeed = m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection = xmf3MovingDirection;
			m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed = fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_pObjectCollided = NULL;
			m_ppObjects[i]->m_pObjectCollided = NULL;
		}
	}
}

void CScene::CheckObjectByWallCollisions()
{
	for (int i = 0; i < m_nObjects; i++)
	{
		ContainmentType containType = m_pWallsObject->m_xmOOBB.Contains(m_ppObjects[i]->m_xmOOBB);
		switch (containType)
		{
			case DISJOINT:
			{
				int nPlaneIndex = -1;
				for (int j = 0; j < 6; j++)
				{
					PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
					if (intersectType == BACK)
					{
						nPlaneIndex = j;
						break;
					}
				}
				if (nPlaneIndex != -1)
				{
					XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
					XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
					XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
				}
				break;
			}
			case INTERSECTS:
			{
				int nPlaneIndex = -1;
				for (int j = 0; j < 6; j++)
				{
					PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
					if (intersectType == INTERSECTING)
					{
						nPlaneIndex = j;
						break;
					}
				}
				if (nPlaneIndex != -1)
				{
					XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
					XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
					XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
				}
				break;
			}
			case CONTAINS:
				break;
		}
	}
}

void CScene::CheckPlayerByWallCollision()
{
	BoundingOrientedBox xmOOBBPlayerMoveCheck;
	m_pWallsObject->m_xmOOBBPlayerMoveCheck.Transform(xmOOBBPlayerMoveCheck, XMLoadFloat4x4(&m_pWallsObject->m_xmf4x4World));
	XMStoreFloat4(&xmOOBBPlayerMoveCheck.Orientation, XMQuaternionNormalize(XMLoadFloat4(&xmOOBBPlayerMoveCheck.Orientation)));

	if (!xmOOBBPlayerMoveCheck.Intersects(m_pPlayer->m_xmOOBB)) m_pWallsObject->SetPosition(m_pPlayer->m_xmf3Position);
}

void CScene::CheckObjectByBulletCollisions()
{
	auto* pAirplanePlayer = static_cast<CAirplanePlayer*>(m_pPlayer);

	for (auto& object : m_ppObjects)
	{
		for (auto& bullet : pAirplanePlayer->m_ppBullets)
		{
			if (bullet->m_bActive && object->m_xmOOBB.Intersects(bullet->m_xmOOBB))
			{
				CExplosiveObject* pExplosiveObject = static_cast<CExplosiveObject*>(object.get());
				pExplosiveObject->m_bBlowingUp = true;
				bullet->m_bActive = false;
			}
		}
	}
}

void CScene::Animate(float fElapsedTime)
{
	m_pWallsObject->Animate(fElapsedTime);
	for (size_t i = 0; i < m_ppObjects.size(); i++)
	{
		m_ppObjects[i]->Animate(fElapsedTime);
	}

	CheckPlayerByWallCollision();

	CheckObjectByWallCollisions();

	CheckObjectByObjectCollisions();

	CheckObjectByBulletCollisions();
}

void CScene::Render(HDC hDCFrameBuffer, CCamera *pCamera)
{
	m_pWallsObject->Render(hDCFrameBuffer, pCamera);

	for (size_t i = 0; i < m_ppObjects.size(); i++)
	{
		m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);
	}
}


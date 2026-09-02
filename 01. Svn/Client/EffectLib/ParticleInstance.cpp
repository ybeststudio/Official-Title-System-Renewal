// The same scale block is used twice in this file: once in the billboard branch of
// CParticleInstance::Transform, once in the z-rotation branch. Patch both.

// In `void CParticleInstance::Transform(const D3DXMATRIX* c_matLocal)`, find this block:
		CCamera* pCurrentCamera = CCameraManager::Instance().GetCurrentCamera();
		const D3DXVECTOR3& c_rv3View = pCurrentCamera->GetView();
		D3DXVec3Cross(&v3Cross, &v3Up, &c_rv3View);
		D3DXVec3Normalize(&v3Cross, &v3Cross);

	}

// Replace the two scale lines that follow it with:
#if defined(ENABLE_TITLE_SYSTEM)
	float fScale = 1.0f;
	if (c_matLocal)
	{
		fScale = D3DXVec3Length((const D3DXVECTOR3*)&c_matLocal->_11);
	}

	v3Cross = -(m_v2HalfSize.x * m_v2Scale.x * fScale) * v3Cross;
	v3Up = (m_v2HalfSize.y * m_v2Scale.y * fScale) * v3Up;
#else
	v3Cross = -(m_v2HalfSize.x * m_v2Scale.x) * v3Cross;
	v3Up = (m_v2HalfSize.y * m_v2Scale.y) * v3Up;
#endif

// In the second occurrence, find this block:
		x = v3Cross.x;
		y = v3Cross.y;
		v3Cross.x = x * fCos - y * fSin;
		v3Cross.y = y * fCos + x * fSin;
	}

// Replace the two scale lines that follow it with:
#if defined(ENABLE_TITLE_SYSTEM)
	float fScale = 1.0f;
	if (c_matLocal)
	{
		fScale = D3DXVec3Length((const D3DXVECTOR3*)&c_matLocal->_11);
	}

	v3Cross = -(m_v2HalfSize.x * m_v2Scale.x * fScale) * v3Cross;
	v3Up = (m_v2HalfSize.y * m_v2Scale.y * fScale) * v3Up;
#else
	v3Cross = -(m_v2HalfSize.x * m_v2Scale.x) * v3Cross;
	v3Up = (m_v2HalfSize.y * m_v2Scale.y) * v3Up;
#endif

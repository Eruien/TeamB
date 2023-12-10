#include "LNode.h"

bool LNode::IntersectRay(LRay& ray)
{
    float tmin, tmax, tymin, tymax, tzmin, tzmax;

    ray.m_InvDir.x = 1.0f / ray.m_Direction.x;
    ray.m_InvDir.y = 1.0f / ray.m_Direction.y;
    ray.m_InvDir.z = 1.0f / ray.m_Direction.z;

    tmin = (m_tBox.vMin.x - ray.m_Origin.x) * ray.m_InvDir.x;
    tmax = (m_tBox.vMax.x - ray.m_Origin.x) * ray.m_InvDir.x;
    tymin = (m_tBox.vMin.y - ray.m_Origin.y) * ray.m_InvDir.y;
    tymax = (m_tBox.vMax.y - ray.m_Origin.y) * ray.m_InvDir.y;

    if ((tmin > tymax) || (tymin > tmax))
        return false;

    if (tymin > tmin)
        tmin = tymin;

    if (tymax < tmax)
        tmax = tymax;

    tzmin = (m_tBox.vMin.z - ray.m_Origin.z) * ray.m_InvDir.z;
    tzmax = (m_tBox.vMax.z - ray.m_Origin.z) * ray.m_InvDir.z;

    if ((tmin > tzmax) || (tzmin > tmax))
        return false;

    return true;
}

void LNode::SetParent(LNode* pParent)
{
	if (pParent == nullptr) return;
	m_pParent = pParent;
	m_RowCellSize = pParent->m_RowCellSize * 0.5f;
	m_ColCellSize = pParent->m_ColCellSize * 0.5f;
	m_iDepth = pParent->m_iDepth + 1;
}

LNode::~LNode()
{
	for (int i = 0; i < m_pChild.size(); i++)
	{
		delete m_pChild[i];
	}	
}


void LNode::UpdateIndexList(LHeightMap* map)
{
    // ���õ� ����� �ε��� ����Ʈ ������Ʈ ���� �߰�
    // ���ο� ���̸� ����Ͽ� �ε��� ����Ʈ�� ����մϴ�.

    for (size_t i = 0; i < m_IndexList.size(); ++i)
    {
        /*DWORD vertexIndex = m_IndexList[i];
        map->m_VertexList[vertexIndex].p.y += 100;*/

        DWORD vertexIndex = m_IndexList[i];
        TVector3& vertex = map->m_VertexList[vertexIndex].p;
        // ���õ� ����� ���̸� �����ϵ��� ����
        vertex.y += 100;
    }


}

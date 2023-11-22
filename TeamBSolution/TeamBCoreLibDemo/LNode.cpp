#include "LNode.h"

void LNode::SetParent(LNode* pParent)
{
	if (pParent == nullptr) return;
	m_pParent = pParent;
	m_RowCellSize = pParent->m_RowCellSize * 0.5f;
	m_ColCellSize = pParent->m_ColCellSize * 0.5f;
	m_iDepth = pParent->m_iDepth + 1;
}

#include "LMap.h"

bool LMap::Load(LMapDesc& mapDesc)
{
	m_MapDesc = mapDesc;
	m_iNumRows = mapDesc.iNumCols;
	m_iNumCols = mapDesc.iNumRows;
	m_iNumCellRows = m_iNumRows - 1;
	m_iNumCellCols = m_iNumCols - 1;
	m_iNumFace = m_iNumCellRows * m_iNumCellCols * 2;
	m_fCellDistance = mapDesc.fCellDistance;
	m_iNumVertices = m_iNumRows * m_iNumCols;
	LDXObject::Create(mapDesc.ShaderFilePath, mapDesc.TextureFilePath);

	return true;
}

bool LMap::CreateVertexData()
{
	m_VertexList.resize(m_MapDesc.iNumCols * m_MapDesc.iNumRows);
	float fHalfCols = (m_MapDesc.iNumCols - 1) * 0.5f;
	float fHalfRows = (m_MapDesc.iNumRows - 1) * 0.5f;
	
	for (int iRow = 0; iRow < m_MapDesc.iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_MapDesc.iNumCols; iCol++)
		{
			int  iVertexIndex = iRow * m_MapDesc.iNumCols + iCol;
			m_VertexList[iVertexIndex].p.x = (iCol - fHalfCols) * m_MapDesc.fCellDistance;
			m_VertexList[iVertexIndex].p.z = -((iRow - fHalfRows) * m_MapDesc.fCellDistance);
			m_VertexList[iVertexIndex].p.y = GetHeightOfVertex(iVertexIndex);
			m_VertexList[iVertexIndex].n = GetNormalOfVertex(iVertexIndex);
			m_VertexList[iVertexIndex].c = GetColorOfVertex(iVertexIndex);
			m_VertexList[iVertexIndex].t =
				GetTextureOfVertex(iRow, iCol);
		}
	}

	return true;
}

bool LMap::CreateIndexData()
{
	m_IndexList.resize(m_iNumFace * 3);

	int iCurIndex = 0;
	for (int iRow = 0; iRow < m_iNumCellRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCellCols; iCol++)
		{
			//0	1    4   
			//2	   3 5
			int iNextRow = iRow + 1;
			int iNextCol = iCol + 1;
			m_IndexList[iCurIndex + 0] = iRow * m_iNumCols + iCol;
			m_IndexList[iCurIndex + 1] = iRow * m_iNumCols + iNextCol;
			m_IndexList[iCurIndex + 2] = iNextRow * m_iNumCols + iCol;
			m_IndexList[iCurIndex + 3] = m_IndexList[iCurIndex + 2];
			m_IndexList[iCurIndex + 4] = m_IndexList[iCurIndex + 1];
			m_IndexList[iCurIndex + 5] = iNextRow * m_iNumCols + iNextCol;

			iCurIndex += 6;
		}
	}

	GenVertexNormal();
	return true;
}

bool LMap::GenVertexNormal()
{
	///////////////////////////////////////////////////////////////////
	//// ���̽� �븻 ��� ��  �̿� ���̽� �ε��� �����Ͽ� ���� �븻 ���
	///////////////////////////////////////////////////////////////////
	InitFaceNormals();
	GenNormalLookupTable();
	CalcPerVertexNormalsFastLookup();
	return true;
}

void LMap::InitFaceNormals()
{
	m_pFaceNormals = new LVector[m_iNumFace];

	for (int i = 0; i < m_iNumFace; i++)
	{
		m_pFaceNormals[i] = LVector(0.0f, 0.0f, 0.0f);
	}
}

void LMap::GenNormalLookupTable()
{
	// We're going to create a table that lists, for each vertex, the
	// triangles which that vertex is a part of.

	if (m_pNormalLookupTable != NULL)
	{
		free(m_pNormalLookupTable);
		m_pNormalLookupTable = NULL;
	}

	// Each vertex may be a part of up to 6 triangles in the grid, so
	// create a buffer to hold a pointer to the normal of each neighbor.
	int buffersize = m_iNumRows * m_iNumCols * 6;

	m_pNormalLookupTable = (int*)malloc(sizeof(void*) * buffersize);
	for (int i = 0; i < buffersize; i++)
		m_pNormalLookupTable[i] = -1;

	// Now that the table is initialized, populate it with the triangle data.

	// For each triangle
	//   For each vertex in that triangle
	//     Append the triangle number to lookuptable[vertex]
	// �� X �� X 2 FACE�� ����
	for (int i = 0; i < m_iNumFace; i++)
	{
		// FACE�� ���� X 3 ������ ����
		for (int j = 0; j < 3; j++)
		{
			// ������ 6���� ���̽� �븻�� ������ ������ �������
			// Find the next empty slot in the vertex's lookup table "slot"
			for (int k = 0; k < 6; k++)
			{
				int vertex = m_IndexList[i * 3 + j];
				if (m_pNormalLookupTable[vertex * 6 + k] == -1)
				{
					m_pNormalLookupTable[vertex * 6 + k] = i;
					break;
				}
			}
		}  // For each vertex that is part of the current triangle
	} // For each triangle
}

void LMap::CalcPerVertexNormalsFastLookup()
{
	// First, calculate the face normals for each triangle.
	CalcFaceNormals();

	// For each vertex, sum the normals indexed by the normal lookup table.
	int j = 0;
	for (int i = 0; i < m_iNumVertices; i++)
	{
		LVector avgNormal;
		avgNormal = LVector(0.0f, 0.0f, 0.0f);

		// Find all the triangles that this vertex is a part of.
		for (j = 0; j < 6; j++)
		{
			int triIndex;
			triIndex = m_pNormalLookupTable[i * 6 + j];

			// If the triangle index is valid, get the normal and average it in.
			if (triIndex != -1)
			{
				avgNormal = avgNormal + m_pFaceNormals[triIndex];
			}
			else
				break;
		}

		// Complete the averaging step by dividing & normalizing.
		_ASSERT(j > 0);
		avgNormal.x /= (float)j;//.DivConst( (float)(j) );
		avgNormal.y /= (float)j;
		avgNormal.z /= (float)j;
		avgNormal = avgNormal.NormalVector();

		// Set the vertex normal to this new normal.
		m_VertexList[i].n.x = avgNormal.x;
		m_VertexList[i].n.y = avgNormal.y;
		m_VertexList[i].n.z = avgNormal.z;

	}  // For each vertex

	/////////////////////////////////////////////////////////////////
	// ���̽� �븻 ��� ��  �̿� ���̽� �ε��� �����Ͽ� ���� �븻 ���
	/////////////////////////////////////////////////////////////////
	// ���� �� ����� �� �ʿ� ������	
	//if (m_bStaticLight) CalcVertexColor(m_vLightDir);
}

void LMap::CalcFaceNormals()
{
	// Loop over how many faces there are
	int j = 0;
	for (int i = 0; i < m_iNumFace * 3; i += 3)
	{
		DWORD i0 = m_IndexList[i];
		DWORD i1 = m_IndexList[i + 1];
		DWORD i2 = m_IndexList[i + 2];
		m_pFaceNormals[j] = ComputeFaceNormal(i0, i1, i2);
		j++;
	}
}

LVector LMap::ComputeFaceNormal(DWORD dwIndex0, DWORD dwIndex1, DWORD dwIndex2)
{
	LVector vNormal;
	LVector v0 = m_VertexList[dwIndex1].p - m_VertexList[dwIndex0].p;
	LVector v1 = m_VertexList[dwIndex2].p - m_VertexList[dwIndex0].p;

	vNormal = v0 ^ v1;
	vNormal = vNormal.NormalVector();
	return vNormal;
}

float LMap::GetHeightOfVertex(int iVertexIndex)
{
	return 0.0f;
}

LVector LMap::GetNormalOfVertex(int iVertexIndex)
{
	return LVector(0.0f, 1.0f, 0.0f);
}

LVector4 LMap::GetColorOfVertex(int iVertexIndex)
{
	return LVector4(1.0f, 1.0f, 1.0f, 1.0f);
}

LVector2 LMap::GetTextureOfVertex(int row, int col)
{
	return LVector2((float)col / (float)(m_iNumCols - 1),
		(float)row / (float)(m_iNumRows - 1));
}
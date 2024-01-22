#include "LExportIO.h"

void LExportIO::SetCharacterFbxPath(std::wstring characterFbxPath, TMatrix characterPos)
{
    WriteStringData(wtm(characterFbxPath).c_str(), m_TextMaxSize, pWritePos, iWritePos, m_ExportForm.characterFbxPath);
    m_ExportForm.characterPos = characterPos;
    iWritePos = 0;
}

void LExportIO::SetDefaultPoseAnimationPath(std::wstring defaultAnimationPath)
{
    WriteStringData(wtm(defaultAnimationPath).c_str(), m_TextMaxSize, pWritePos, iWritePos, m_ExportForm.defaultposeAnimation);
    iWritePos = 0;
}

void LExportIO::SetAnimationPath(std::wstring AnimationPath)
{
    WriteStringData(wtm(AnimationPath).c_str(), m_TextMaxSize, pWritePos, iWritePos, m_ExportForm.animationList);
}

void LExportIO::SetItem(std::wstring ItemPath, std::wstring ParentName, TMatrix matScale, TMatrix matRotation, TMatrix matTranslation)
{
    WriteStringData(wtm(ItemPath).c_str(), m_TextMaxSize, pWritePos, iWritePos, m_ExportForm.itemList);
    WriteStringData(wtm(ParentName).c_str(), m_TextMaxSize, pWritePos, iWritePos2, m_ExportForm.itemMatParentName);
    m_ExportForm.itemScale[itemCount] = matScale;
    m_ExportForm.itemRotation[itemCount] = matRotation;
    m_ExportForm.itemTranslation[itemCount] = matTranslation;
    for (int i = 0; i < 9; i++)
    {
        //m_ExportForm.matNumber[i] = CharacterToolForm::g_matItemNumber[i];
    }
    itemCount++;
}

void LExportIO::Reset()
{
    ZeroMemory(&m_ExportHeader, sizeof(m_ExportHeader));
    ZeroMemory(&m_ExportForm, sizeof(m_ExportForm));
    iWritePos = 0;
    iWritePos2 = 0;
    iReadPos = 0;
    iReadPos2 = 0;
    itemCount = 0;
}

void LExportIO::ResetiPos()
{
    iWritePos = 0;
    iWritePos2 = 0;
    iReadPos = 0;
    iReadPos2 = 0;
    itemCount = 0;
}

void LExportIO::WriteStringData(const char* data, int dataSize, void* pPos, int& iPos, char* changePtr)
{
    pPos = &changePtr[iPos];
    CopyMemory(pPos, data, dataSize);
    iPos += dataSize;

    return;
}

void LExportIO::ReadStringData(char* buffer, int bufferSize, void* pReadPos, int& iReadPos, char* changePtr)
{
    pReadPos = &changePtr[iReadPos];
    CopyMemory(buffer, pReadPos, bufferSize);
    iReadPos += bufferSize;

    return;
}

bool LExportIO::ExportWrite(std::wstring FormFileName, int animationListSize, int itemListSize)
{
    std::wstring saveFilePath = L"../../res/UserFile/Form/";
    saveFilePath += FormFileName;
    saveFilePath += L".bin";

    m_WriteFile.open(saveFilePath, std::ios::binary);

    m_ExportHeader.animationListSize = animationListSize;
    m_ExportHeader.itemListSize = itemListSize;

    m_WriteFile.write(reinterpret_cast<char*>(&m_ExportHeader), sizeof(m_ExportHeader));
    m_WriteFile.write(reinterpret_cast<char*>(&m_ExportForm), sizeof(m_ExportForm));
    m_WriteFile.close();
    return true;
}

bool LExportIO::ExportRead(std::wstring FormFileName)
{
    Reset();

    std::wstring saveFilePath = L"../../res/UserFile/Form/";
    saveFilePath += FormFileName;

    m_ReadFile.open(saveFilePath, std::ios::binary);
    m_ReadFile.read(reinterpret_cast<char*>(&m_ExportHeader), sizeof(m_ExportHeader));
    m_ReadFile.read(reinterpret_cast<char*>(&m_ExportForm), sizeof(m_ExportForm));

    int animationListSize = m_ExportHeader.animationListSize;
    int itemListSize = m_ExportHeader.itemListSize;

    for (int i = 0; i < animationListSize; i++)
    {
        ReadStringData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_ExportForm.animationList);
        std::wstring animationName = mtw(pReadTextBuffer);
        m_AnimationList.push_back(animationName);

    }

    ResetiPos();
    for (int i = 0; i < itemListSize; i++)
    {
        ReadStringData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_ExportForm.itemList);
        std::wstring itemName = mtw(pReadTextBuffer);
        m_ItemList.push_back(itemName);

        ReadStringData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos2, m_ExportForm.itemMatParentName);
        std::wstring itemParentName = mtw(pReadTextBuffer);
        m_ItemParentName.push_back(itemParentName);

        m_ItemScale.push_back(m_ExportForm.itemScale[i]);
        m_ItemRotation.push_back(m_ExportForm.itemRotation[i]);
        m_ItemTranslation.push_back(m_ExportForm.itemTranslation[i]);
    }

    m_ReadFile.close();
    return true;
}

LExportIO::LExportIO()
{
    Reset();
}

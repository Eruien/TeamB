#include "TextureList.h"
#include "tinyxml2.h"
TextureList::TextureList() : ResourceBase(ResourceType::TextureList)
{
}

TextureList::~TextureList()
{
}

void TextureList::Save(wstring path)
{

    tinyxml2::XMLDocument doc;

    tinyxml2::XMLElement* root = doc.NewElement("Animation");
    doc.LinkEndChild(root);
  
    
    for (int i=0; i < _texList.size(); i++)
    {
        tinyxml2::XMLElement* node = doc.NewElement("TexPaths");
        root->LinkEndChild(node);
        node->SetAttribute("texFilePath", wtm(_texList[i]).c_str());
   

    }

    string pathStr(path.begin(), path.end());
    auto result = doc.SaveFile(pathStr.c_str());
}

void TextureList::Load(wstring path)
{
    tinyxml2::XMLDocument xmlDoc;
    if (xmlDoc.LoadFile("output.xml") != tinyxml2::XML_SUCCESS) {
        std::cerr << "XML ������ �ҷ����� �� �����߽��ϴ�." << std::endl;
    }

    // ��Ʈ ������Ʈ ���
    tinyxml2::XMLElement* root = xmlDoc.FirstChildElement("Images");
    if (!root) {
        std::cerr << "��Ʈ ������Ʈ�� ã�� �� �����ϴ�." << std::endl;
    }

    // �ڽ� ������Ʈ���� �ݺ��ϸ鼭 ������ �б�
   
    for (tinyxml2::XMLElement* imageElement = root->FirstChildElement("Image"); imageElement; imageElement = imageElement->NextSiblingElement("Image")) {
        const char* imagePath = imageElement->GetText();
        if (imagePath) {
            // char�� wstring���� ��ȯ�Ͽ� ���Ϳ� �߰�
            std::wstring wImagePath = mtw(imagePath);
            _texList.push_back(wImagePath);
        }
    }



}

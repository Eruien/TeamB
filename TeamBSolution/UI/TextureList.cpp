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
        std::cerr << "XML 파일을 불러오는 데 실패했습니다." << std::endl;
    }

    // 루트 엘리먼트 얻기
    tinyxml2::XMLElement* root = xmlDoc.FirstChildElement("Images");
    if (!root) {
        std::cerr << "루트 엘리먼트를 찾을 수 없습니다." << std::endl;
    }

    // 자식 엘리먼트들을 반복하면서 데이터 읽기
   
    for (tinyxml2::XMLElement* imageElement = root->FirstChildElement("Image"); imageElement; imageElement = imageElement->NextSiblingElement("Image")) {
        const char* imagePath = imageElement->GetText();
        if (imagePath) {
            // char를 wstring으로 변환하여 벡터에 추가
            std::wstring wImagePath = mtw(imagePath);
            _texList.push_back(wImagePath);
        }
    }



}

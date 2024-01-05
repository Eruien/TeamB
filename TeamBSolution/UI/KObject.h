#pragma once
#include "LObject.h"
#include "MonoBehaviour.h"


class KObject : public LObject , public enable_shared_from_this<KObject>
{
public:
	virtual ~KObject() {};
public:
	virtual bool Init() override;
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;

	void SetName(string name) { _name = name; };
	string GetName() {	return _name;};
	
	// Rect
	void SetRect(TVector3 pos, TVector3 scale);
	TRectangle GetRect() { return _rect; };
	bool CreateVertexBuffer() override;
	//��ũ��Ʈ
	void AddScripts(shared_ptr<MonoBehaviour> script) { script->SetGameObject(shared_from_this()); _scripts.push_back(script); };
	template<class T>
	shared_ptr<T> GetScript(wstring name);

	void SetAnimation();

//Rect
public:
	TRectangle _rect;


protected:
	string _name;
	vector<shared_ptr<MonoBehaviour>> _scripts;
};

template<class T>
inline shared_ptr<T> KObject::GetScript(wstring name)
{
	for (int i = 0; i < _scripts.size(); i++)
	{
		if (name == _scripts[i]->_name)
		{
			auto temp = dynamic_pointer_cast<T>(_scripts[i]);
			
			if (temp != nullptr)
			{
				return temp;
			}
		}
	}
	return nullptr;
}

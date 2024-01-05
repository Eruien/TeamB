#pragma once
#include "LStd.h"
class KObject;
class MonoBehaviour
{
public:
	MonoBehaviour(wstring ScriptName);
	 ~MonoBehaviour();
public:
	 virtual void Awake() { };
	 virtual void Init() { };
	 virtual void Frame() { };
	
private:
	friend class KObject;
public:
	void SetGameObject(shared_ptr<KObject> gameObject) { _gameObject = gameObject; }
	shared_ptr<KObject> GetGameObject() {return _gameObject.lock();};
protected:
	wstring _name;
	weak_ptr<KObject> _gameObject;
};


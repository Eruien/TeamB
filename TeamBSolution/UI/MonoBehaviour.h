#pragma once
#include "LStd.h"
class KSpriteObj;
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
	friend class KSpriteObj;
public:
	void SetGameObject(shared_ptr<KSpriteObj> gameObject) { _gameObject = gameObject; }
	shared_ptr<KSpriteObj> GetGameObject() {return _gameObject.lock();};
protected:
	wstring _name;
	weak_ptr<KSpriteObj> _gameObject;
};


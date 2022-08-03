#include <BFCKeyManager.h>



BFCKeyManager::BFCKeyManager(): CtrlDown(false), AltDown(false), LMouseDown(false), MMouseDown(false), RMouseDown(false),
								XDown(false), YDown(false), ZDown(false), ShiftDown(false), RDown(false), ODown(false),
								MDown(false), PrevDown(false), NextDown(false), HomeDown(false), EndDown(false), PDown(false)
{
	Flush();
}

BFCVoid BFCKeyManager::Flush()
{
	for (BFCInt i=0;i<4;i++)
	{
		KeyPressed[i] = bfc_none;
	}
}

BFCKeyManager::AddNewKey(BFCKey key)
{
	BFCKey _key,_key2;
	_key = KeyPressed[0];
	KeyPressed[0] = key;
	_key2 = KeyPressed[1];
	KeyPressed[1] = _key;
	_key = KeyPressed[2];
	KeyPressed[2] = _key2;
	KeyPressed[3] = _key;
}
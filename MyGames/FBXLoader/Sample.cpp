#include "Sample.h"

bool Sample::Init()
{
	m_FbxObj.Load("../../Image/FBX_Image/Card.fbx");
	return true;
};

bool Sample::Frame()
{
	return true;
};

bool Sample::Render()
{
	return true;
};

bool Sample::Release()
{
	return true;
};

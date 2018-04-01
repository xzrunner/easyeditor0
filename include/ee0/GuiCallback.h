#pragma once

namespace ee0
{

class VariantSet;

class GuiCallback
{
public:
	virtual ~GuiCallback() {}
	virtual void SetValue(int key, const VariantSet& variants) = 0;
	virtual void GetValue(int key, VariantSet& variants) = 0;

}; // GuiCallback

}
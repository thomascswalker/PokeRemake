#pragma once

class ISelectable
{
protected:
	bool mSelected = false;

public:
	bool GetSelected() { return mSelected; }
	void SetSelected(bool value) { mSelected = value; }
};
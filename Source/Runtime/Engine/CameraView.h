#pragma once

#define DEFAULT_FOV 90.0f

enum EViewMode
{
	VM_Perspective,
	VM_Orthographic,
};

class ICameraView
{
protected:
	float	  FOV = DEFAULT_FOV;
	EViewMode ViewMode = VM_Perspective;

public:
	float	  GetFOV() const { return FOV; }
	EViewMode GetViewMode() const { return ViewMode; }
	void	  SetFOV(float InFOV) { FOV = InFOV; }
	void	  SetViewMode(EViewMode InViewMode) { ViewMode = InViewMode; }
};
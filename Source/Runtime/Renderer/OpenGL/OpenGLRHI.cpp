#include "OpenGLRHI.h"

#include "Core/Logging.h"

bool OpenGLRHI::Initialize(SDL_Window* Window)
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

	mContext = SDL_GL_CreateContext(Window);
	if (!mContext)
	{
		Error("Failed to create OpenGL context: {}", SDL_GetError());
		return false;
	}
	Debug("OpenGL context created.");

	return true;
}
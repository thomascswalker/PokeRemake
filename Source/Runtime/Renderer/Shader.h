#pragma once

#include "SDL3/SDL.h"
#include <string>
#include <vector>

bool CompileShaders(SDL_GPUDevice* Device);
bool LoadShaders(SDL_GPUDevice* Device, std::vector<SDL_GPUShader*>* OutShaders);
void ReleaseShaders(SDL_GPUDevice* Device, const std::vector<SDL_GPUShader*>* Shaders);

bool		   CompileShaderFile(const std::string& FileName, SDL_GPUDevice* Device);
SDL_GPUShader* LoadShaderFromFile(const std::string& FileName, SDL_GPUDevice* Device);
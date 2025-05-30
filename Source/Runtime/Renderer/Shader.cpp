#include "Shader.h"

#include "Core/Logging.h"

#define COMPILE_SHADER_CMD "glslc.exe {} -o {}.spv"

bool CompileShaders(SDL_GPUDevice* Device)
{
	// Get all shader files in the current directory
	for (const auto& Entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
	{
		if (Entry.is_regular_file()
			&& (Entry.path().extension() == ".vert" || Entry.path().extension() == ".frag"))
		{
			if (!CompileShaderFile(Entry.path().filename().string(), Device))
			{
				LogError("Failed to compile shader file: {}", Entry.path().filename().string());
				return false;
			}
		}
	}
	return true;
}

bool LoadShaders(SDL_GPUDevice* Device, std::vector<SDL_GPUShader*>* OutShaders)
{
	// Load all shader files in the current directory
	for (const auto& Entry : std::filesystem::directory_iterator(std::filesystem::current_path()))
	{
		if (Entry.is_regular_file()
			&& (Entry.path().extension() == ".vert" || Entry.path().extension() == ".frag"))
		{
			const auto Shader = LoadShaderFromFile(Entry.path().filename().string(), Device);
			if (!Shader)
			{
				LogError("Failed to load shader file: {}", Entry.path().filename().string());
				return false;
			}
			OutShaders->push_back(std::move(Shader));
		}
	}
	return true;
}
void ReleaseShaders(SDL_GPUDevice* Device, const std::vector<SDL_GPUShader*>* Shaders)
{
	for (SDL_GPUShader* Shader : *Shaders)
	{
		SDL_ReleaseGPUShader(Device, Shader);
	}
}

bool CompileShaderFile(const std::string& FileName, SDL_GPUDevice* Device)
{
	LogDebug("Compiling Shader: {}", FileName);
	auto AbsPath = std::filesystem::current_path() / FileName;
	auto AbsString = AbsPath.string();
	switch (SDL_GetGPUShaderFormats(Device))
	{
		case SDL_GPU_SHADERFORMAT_SPIRV:
			{
				const std::string Cmd =
					std::format(COMPILE_SHADER_CMD, AbsString.c_str(), FileName.c_str());
				system(Cmd.c_str());
				if (!std::filesystem::exists(FileName + ".spv"))
				{
					LogError("Failed to compile shader file: {}", FileName);
					return false;
				}
				break;
			}
		default:
			return false;
	}
	return true;
}
/**
 * @brief Loads a shader file from `Resources/Shaders/`.
 * @param FileName The name of the shader file to load, e.g., "Default.vert".
 * @param Device The SDL GPU device to use for loading the shader.
 * @return A pointer to the loaded SDL_GPUShader, or nullptr if loading failed.
 */
SDL_GPUShader* LoadShaderFromFile(const std::string& FileName, SDL_GPUDevice* Device)
{
	SDL_GPUShaderStage Stage;
	if (FileName.ends_with(".vert"))
	{
		Stage = SDL_GPU_SHADERSTAGE_VERTEX;
	}
	else if (FileName.ends_with(".frag"))
	{
		Stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
	}
	else
	{
		LogError("Unknown shader file stage: {}", FileName);
		return nullptr;
	}

	SDL_GPUShaderFormat Format = SDL_GetGPUShaderFormats(Device);
	std::string			EntryPoint = "main";
	std::string			Ext;

	switch (Format)
	{
		case SDL_GPU_SHADERFORMAT_SPIRV: // Vulkan
			EntryPoint = "main";
			Ext = ".spv";
			break;
		case SDL_GPU_SHADERFORMAT_MSL: // Metal
			EntryPoint = "main0";
			Ext = ".msl";
			break;
		case SDL_GPU_SHADERFORMAT_DXBC: // DX12 SM5.1
		case SDL_GPU_SHADERFORMAT_DXIL: // DX12 SM6.0
			EntryPoint = "main";
			Ext = ".cso";
			break;
		default:
			LogError("Unsupported shader format for file {}: {}", FileName, Format);
			return nullptr;
	}

	size_t	   FileSize;
	const auto Code = SDL_LoadFile((FileName + Ext).c_str(), &FileSize);
	if (!Code)
	{
		LogError("Failed to load shader file {}", FileName);
		return nullptr;
	}

	const SDL_GPUShaderCreateInfo ShaderInfo = {
		.code_size = FileSize,
		.code = static_cast<uint8_t*>(Code),
		.entrypoint = EntryPoint.c_str(),
		.format = Format,
		.stage = Stage,
		.num_samplers = 0,
		.num_storage_textures = 0,
		.num_storage_buffers = 0,
		.num_uniform_buffers = 0,
	};

	SDL_GPUShader* Shader = SDL_CreateGPUShader(Device, &ShaderInfo);
	SDL_free(Code);
	if (!Shader)
	{
		LogError("Failed to create shader {}: {}", FileName, SDL_GetError());
		return nullptr;
	}

	return Shader;
}
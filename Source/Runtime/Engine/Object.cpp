#include "Object.h"

#include <cxxabi.h>
#include <format>

std::string PObject::GetClassName() const
{
	auto Name = typeid(*this).name();
	int	 Status;

	// Create a std::unique_ptr for the resulting demangled name in order to properly free the allocated memory and
	// pass back a std::string.
	std::unique_ptr<char, void (*)(void*)> RetValue(abi::__cxa_demangle(Name, nullptr, nullptr, &Status), std::free);

	// Return either the demangled name if it's valid, or the mangled name if it's invalid.
	return Status == 0 ? RetValue.get() : Name;
}

void PObject::GenerateInternalName()
{
	static int Count = 0;
	mInternalName = std::format("{}_{}", GetClassName(), Count++);
}

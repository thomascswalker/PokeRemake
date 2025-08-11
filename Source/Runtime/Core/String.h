#pragma once
#include <format>
#include <string>
#include <vector>

namespace String
{
    template <typename T>
    std::string Join(const std::vector<T>& Container, const std::string& Delimiter)
    {
        std::string Result = "";
        for (auto Iter = Container.begin(); Iter != Container.end(); ++Iter)
        {
            // Add element to the result.
            Result += std::to_string(*Iter);

            // If the current element is _not_ the last element, add
            // the delimiter and a space to the result.
            if (Iter != --Container.end())
            {
                Result += Delimiter + " ";
            }
        }
        return Result;
    }

    template <typename T>
    std::string ToString(const std::vector<T>& Container)
    {
        return std::format("[{}]", String::Join(Container, ",").c_str());
    }
}

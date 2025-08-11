#pragma once

#include <format>
#include <string>
#include <vector>
#include <type_traits>

namespace String
{
    template <typename T>
    std::string Join(const std::vector<T>& Container, const std::string& Delimiter)
    {
        std::string Result = "";
        for (auto Iter = Container.begin(); Iter != Container.end(); ++Iter)
        {
            // Add element to the result.
            Result += std::string(*Iter);

            // If the current element is _not_ the last element, add
            // the delimiter and a space to the result.
            if (Iter != --Container.end())
            {
                Result += Delimiter + " ";
            }
        }
        return Result;
    }

    inline std::vector<std::string> Split(const std::string& S, const std::string& D)
    {
        std::vector<std::string> Result;
        size_t Pos = 0;
        while (true)
        {
            const size_t Colon = S.find(D, Pos);
            if (Colon == std::string::npos)
            {
                Result.push_back(S.substr(Pos));
                break;
            }

            Result.push_back(S.substr(Pos, Colon - Pos));
            Pos = Colon + D.size();
        }
        return Result;
    }

    template <typename T>
    std::string ToString(const std::vector<T>& Container)
    {
        return std::format("[{}]", String::Join(Container, ",").c_str());
    }
}

#include "Core/Array.h"

#include "gtest/gtest.h"

TEST(ArrayTestSuite, TestArray)
{
    TArray IntArr = {1, 2, 3, 4, 5};
    std::cout << IntArr.ToString() << std::endl;

    for (auto& Value : IntArr)
    {
        std::cout << Value << '\n';
    }

    IntArr.Add(6);
    IntArr.Add(7);
    IntArr.Add(8);

    std::cout << IntArr.ToString() << std::endl;

    IntArr.AddFront(0);
    IntArr.AddFront(-1);
    IntArr.AddFront(-2);

    std::cout << IntArr.ToString() << std::endl;

    auto Filtered = IntArr.Filter([](auto& Value)
    {
        return Value > 0;
    });
    std::cout << Filtered.ToString() << std::endl;

    std::cout << "Contains 5: " << (IntArr.Contains(5) ? "True" : "False") << std::endl;
    std::cout << "Contains 12: " << (IntArr.Contains(12) ? "True" : "False") << std::endl;
}

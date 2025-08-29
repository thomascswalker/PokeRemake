#include "Core/Logging.h"
#include "Engine/ParamBlock.h"

#include "gtest/gtest.h"

TEST(ParamBlockTests, Default)
{
    LogInfo("Testing!");
    PParamBlock Block;
    float Var = 5;
    Block.AddParameter("Test", &Var);

    auto Param = Block.GetParameter("Test");
    ASSERT_EQ(Param->Get<float>(), 5.0f);
    Param->Set(4.0f);
    ASSERT_EQ(Param->Get<float>(), 4.0f);
    Var = 10;
    ASSERT_EQ(Param->Get<float>(), 10.0f);
}

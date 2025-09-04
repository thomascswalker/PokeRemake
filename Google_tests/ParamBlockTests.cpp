#include "Core/Logging.h"
#include "Engine/ParamBlock.h"

#include "gtest/gtest.h"

class TestObject : public IParamBlock
{
    float mFloatValue      = 0.0f;
    FVector2 mVector2Value = {0.0f, 0.0f};

public:
    TestObject()
    {
        DECLARE_PARAMETER(FloatValue, Float);
        DECLARE_PARAMETER(Vector2Value, FVector2);
    }
};

TEST(ParamBlockTests, Default)
{
    LogInfo("Testing!");
    TestObject Block;
    auto BP = &Block;

    auto FP = Block.GetParameter("FloatValue");
    LogInfo("Value: {}", FP->Get<float>());
    FP->Set(3.14159f);
    LogInfo("Value: {}", FP->Get<float>());
    PARAM(BP, FloatValue)->Set(15.0f);
    LogInfo("Value: {}", FP->Get<float>());

    float Var = 5;
    Block.AddParameter("Test", &Var, PT_Float);

    auto Param = Block.GetParameter("Test");
    LogInfo("{}: {}", Param->GetName().c_str(), Param->ToString().c_str());
    ASSERT_EQ(Param->Get<float>(), 5.0f);
    Param->Set(4.0f);
    LogInfo("{}: {}", Param->GetName().c_str(), Param->ToString().c_str());
    ASSERT_EQ(Param->Get<float>(), 4.0f);
    Var = 10;
    LogInfo("{}: {}", Param->GetName().c_str(), Param->ToString().c_str());
    ASSERT_EQ(Param->Get<float>(), 10.0f);
}

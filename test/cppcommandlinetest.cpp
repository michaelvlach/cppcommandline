#include <gtest/gtest.h>
#include "cppcommandline.h"

using namespace cppcommandline;

TEST(ConfiguringOption, Names)
{
    try
    {
        Option option("Name");
    }
    catch(std::logic_error&)
    {
        FAIL();
    }

    try
    {
        Option option("Name.");
        FAIL();
    }
    catch (std::logic_error&)
    {

    }

    try
    {
        Option option("Name");
        option.asShortName("n");
    }
    catch(std::logic_error&)
    {
        FAIL();
    }

    try
    {
        Option option("Name");
        option.asShortName("na");
        FAIL();
    }
    catch(std::logic_error&)
    {
    }
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

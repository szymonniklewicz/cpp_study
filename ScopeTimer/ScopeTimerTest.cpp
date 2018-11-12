#include <gtest/gtest.h>
#include <regex>
#include <map>
#include "ScopeTimer.hpp"

// helper functions

// Map g_exectedString contains expected string result for each test case name
std::map<std::string, std::string> g_expectedString =
    {
        {
            "SingleVar",                
            "\\{\\n"
            "    \"name\": \"Test timer\",\\n"
            "    \"subresults\": \\[\\n"
            "    \\],\\n"
            "    \"duration\": -?([0-9]+(\\.[0-9eE+\\-]+)?)\\n"
            "\\}\\n" 
        },
        {
            "DoubleVarsSameRootStreamSameScope", 
            "\\{\\n"
            "    \"name\": \"Test timer1\",\\n"
            "    \"subresults\": \\[\\n"
            "        \\{\\n"
            "            \"name\": \"Test timer1.1\",\\n"
            "            \"subresults\": \\[\\n"
            "            \\],\\n"
            "            \"duration\": -?([0-9]+(\\.[0-9eE+\\-]+)?)\\n"
            "        \\}\\n"
            "    \\],\\n"
            "    \"duration\": -?([0-9]+(\\.[0-9eE+\\-]+)?)\\n"
            "\\}\\n" 
        },
    };

// Makes regex string printable to std::cout with proper formatting
std::string toRawString(const std::string& p_input)
{
    std::string l_output = "";
    std::string::const_iterator l_it = p_input.begin();
    while (l_it != p_input.end())
    {
        char l_currentChar = *l_it++;
        if (l_currentChar == '\\' && l_it != p_input.end())
        {
            char cc = *l_it++;
            switch (cc) 
            {
                case '\\': l_currentChar = '\\'; break;
                case 'n': l_currentChar = '\n'; break;
                case 't': l_currentChar = '\t'; break;
                default: l_currentChar = cc; break;
            }
        }
    l_output += l_currentChar;
    }
    return l_output;
}

std::string getTestName()
{
    return ::testing::UnitTest::GetInstance()->current_test_info()->name();
}

std::string getExpected()
{
    return g_expectedString[getTestName()];
}
    
std::regex getRegex()
{
    return std::regex{getExpected()};
}

struct ScopeTimerTest : public testing::Test
{
    std::stringstream m_stream;
    
    std::string getResult()
    {
        return m_stream.str();
    }
    
    std::string buildErrorMsg()
    {
        return "Expected: \n" + toRawString(getExpected()) + "\ngot: \n" + getResult() + "\n";
    }
    
    bool checkRegex()
    {
        return std::regex_match(getResult(), getRegex());
    }
};

// test cases

TEST_F(ScopeTimerTest, CanCreateScopeTimer)
{
    ScopeTimer l_scopeTimerWithStream{m_stream, "Test timer2"};
    ScopeTimer l_scopeTimer{"Test timer"};    
}

TEST_F(ScopeTimerTest, SingleVar)
{
    {
        ScopeTimer l_scopeTimer{m_stream, "Test timer"};
    }
    
    EXPECT_TRUE(checkRegex()) << buildErrorMsg();
}

TEST_F(ScopeTimerTest, DoubleVarsSameRootStreamSameScope)
{
    {
        ScopeTimer l_timer1{m_stream, "Test timer1"};
        ScopeTimer l_timer2{"Test timer1.1"};
    }
    
    EXPECT_TRUE(checkRegex()) << buildErrorMsg();
}

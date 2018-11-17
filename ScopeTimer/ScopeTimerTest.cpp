#include <gtest/gtest.h>
#include <regex>
#include <map>
#include <vector>
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
        {
            "DoubleVarsSameRootStreamInnerScope",
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
        {
            "ThreeVarsSameRootStreamTwoVarsInSameScope",
            "\\{\\n"
            "    \"name\": \"Test timer1\",\\n"
            "    \"subresults\": \\[\\n"
            "        \\{\\n"
            "            \"name\": \"Test timer1.1\",\\n"
            "            \"subresults\": \\[\\n"
            "                \\{\\n"
            "                    \"name\": \"Test timer1.2\",\\n"
            "                    \"subresults\": \\[\\n"
            "                    \\],\\n"
            "                    \"duration\": -?([0-9]+(\\.[0-9eE+\\-]+)?)\\n"
            "                \\}\\n"
            "            \\],\\n"
            "            \"duration\": -?([0-9]+(\\.[0-9eE+\\-]+)?)\\n"
            "        \\}\\n"
            "    \\],\\n"
            "    \"duration\": -?([0-9]+(\\.[0-9eE+\\-]+)?)\\n"
            "\\}\\n"             
        },
        {
            "ThreeVarsSameRootStreamDifferentScope",
            "\\{\\n"
            "    \"name\": \"Test timer1\",\\n"
            "    \"subresults\": \\[\\n"
            "        \\{\\n"
            "            \"name\": \"Test timer1.1\",\\n"
            "            \"subresults\": \\[\\n"
            "            \\],\\n"
            "            \"duration\": -?([0-9]+(\\.[0-9eE+\\-]+)?)\\n"
            "        \\},\\n"
            "        \\{\\n"
            "            \"name\": \"Test timer1.2\",\\n"
            "            \"subresults\": \\[\\n"
            "            \\],\\n"
            "            \"duration\": -?([0-9]+(\\.[0-9eE+\\-]+)?)\\n"
            "        \\}\\n"
            "    \\],\\n"
            "    \"duration\": -?([0-9]+(\\.[0-9eE+\\-]+)?)\\n"
            "\\}\\n"   
        },
        {
            "FiveRecursiveVarsSameRootStream",
            "\\{\\n"
            "    \"name\": \"Test timer1\",\\n"
            "    \"subresults\": \\[\\n"
            "        \\{\\n" 
            "            \"name\": \"Test timer2\",\\n"
            "            \"subresults\": \\[\\n"
            "                \\{\\n" 
            "                    \"name\": \"Test timer3\",\\n"
            "                    \"subresults\": \\[\\n"
            "                        \\{\\n" 
            "                            \"name\": \"Test timer4\",\\n"
            "                            \"subresults\": \\[\\n"
            "                                \\{\\n" 
            "                                    \"name\": \"Test timer5\",\\n"
            "                                    \"subresults\": \\[\\n"
            "                                    \\],\\n"
            "                                    \"duration\": -?([0-9]+(\\.[0-9eE+\\-]+)?)\\n"
            "                                \\}\\n"
            "                            \\],\\n"
            "                            \"duration\": -?([0-9]+(\\.[0-9eE+\\-]+)?)\\n"
            "                        \\}\\n"
            "                    \\],\\n"
            "                    \"duration\": -?([0-9]+(\\.[0-9eE+\\-]+)?)\\n"
            "                \\}\\n"
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
    std::vector<std::stringstream> m_streams;
    
    ScopeTimerTest() : m_streams(std::vector<std::stringstream>())
    {
        m_streams.push_back(std::stringstream{});
    }

    std::string getResult(int p_index = 0)
    {
        return m_streams[p_index].str();
    }
    
    std::string buildErrorMsg()
    {
        return "Expected: \n" + toRawString(getExpected()) + "\ngot: \n" + getResult() + "\n";
    }
    
    bool checkRegex() //TODO add multiple streams support
    {
        return std::regex_match(getResult(), getRegex());
    }

    std::stringstream& getStream(int p_index = 0) //TODO Need to proper check this func
    {
        if (p_index == 0)
        {
            return m_streams[0];
        }
        else if (p_index < m_streams.size())
        {
            return m_streams[p_index];
        }
        else
        {
            m_streams.push_back(std::stringstream{});
            return getStream(p_index);
        }
    }
};

// test cases

TEST_F(ScopeTimerTest, CanCreateScopeTimer)
{
    ScopeTimer l_scopeTimerWithStream{getStream(), "Test timer2"};
    ScopeTimer l_scopeTimer{"Test timer"};    
}

TEST_F(ScopeTimerTest, SingleVar)
{
    {
        ScopeTimer l_scopeTimer{getStream(), "Test timer"};
    }
    
    EXPECT_TRUE(checkRegex()) << buildErrorMsg();
}

TEST_F(ScopeTimerTest, DoubleVarsSameRootStreamSameScope)
{
    {
        ScopeTimer l_timer1{getStream(), "Test timer1"};
        ScopeTimer l_timer2{"Test timer1.1"};
    }
    
    EXPECT_TRUE(checkRegex()) << buildErrorMsg();
}

TEST_F(ScopeTimerTest, DoubleVarsSameRootStreamInnerScope)
{
    {
        ScopeTimer l_timer1{getStream(), "Test timer1"};
        {
            ScopeTimer l_timer2{"Test timer1.1"};
        }
    }
    
    EXPECT_TRUE(checkRegex()) << buildErrorMsg();
}

TEST_F(ScopeTimerTest, ThreeVarsSameRootStreamTwoVarsInSameScope)
{
    {
        ScopeTimer l_timer1{getStream(), "Test timer1"};
        {
            ScopeTimer l_timer2{"Test timer1.1"};
            ScopeTimer l_timer3{"Test timer1.2"};
        }
    }
    
    EXPECT_TRUE(checkRegex()) << buildErrorMsg();
}

TEST_F(ScopeTimerTest, ThreeVarsSameRootStreamDifferentScope)
{
    {
        ScopeTimer l_timer1{getStream(), "Test timer1"};
        {
            {
                ScopeTimer l_timer2{"Test timer1.1"};
            }
            {
                ScopeTimer l_timer3{"Test timer1.2"};
            }
        }
    }
    
    EXPECT_TRUE(checkRegex()) << buildErrorMsg(); 
}

TEST_F(ScopeTimerTest, FiveRecursiveVarsSameRootStream)
{
    {
        ScopeTimer l_timer1{getStream(), "Test timer1"};
        {
            ScopeTimer l_timer2{"Test timer2"};
            {
                ScopeTimer l_timer3{"Test timer3"};
                {
                    ScopeTimer l_timer4{"Test timer4"};
                    {
                        ScopeTimer l_timer5{"Test timer5"};
                    }
                }
            }
        }
    }

    EXPECT_TRUE(checkRegex()) << buildErrorMsg();
}

TEST_F(ScopeTimerTest, TwoVarsSameScopeDifferentStream)
{
    {
        ScopeTimer l_scopeTimer0{getStream(0), "ScopeTimer0"};
        ScopeTimer l_scopeTimer1{getStream(1), "ScopeTimer1"};
        EXPECT_TRUE(false);
    }
}

// this test should produce cumulative output in each stream
// TEST_F(ScopeTimerTest, FiveRecursiveVarsSameRootStream)
// {
//     {
//         ScopeTimer l_timer1{m_stream, "Test timer1"};
//         {
//             ScopeTimer l_timer2{m_stream, "Test timer2"};
//             {
//                 ScopeTimer l_timer3{m_stream, "Test timer3"};
//                 {
//                     ScopeTimer l_timer4{m_stream, "Test timer4"};
//                     {
//                         ScopeTimer l_timer5{m_stream, "Test timer5"};
//                     }
//                 }
//             }
//         }
//     }

//     EXPECT_TRUE(checkRegex()) << buildErrorMsg();
// }
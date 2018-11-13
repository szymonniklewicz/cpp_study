#include "ScopeTimer.hpp"

ScopeTimer* ScopeTimer::s_currentTop = nullptr;

ScopeTimer::ScopeTimer(std::string p_name) : m_startTime(std::chrono::high_resolution_clock::now()),
                                             m_stream(std::nullopt),
                                             m_parent(ScopeTimer::s_currentTop)
{
    m_result.m_name = p_name;
    ScopeTimer::s_currentTop = this;
}

ScopeTimer::ScopeTimer(std::stringstream& p_stream, std::string p_name) : m_startTime(std::chrono::high_resolution_clock::now()),
                                                                          m_stream(p_stream),
                                                                          m_parent(nullptr)
{
    m_result.m_name = p_name;
    ScopeTimer::s_currentTop = this;
}

ScopeTimer::~ScopeTimer()
{
    if (m_parent)
    {
        m_result.m_duration = 0.124;
        m_parent->m_result.m_subresults.push_back(m_result);
        ScopeTimer::s_currentTop = m_parent;
    }
    else
    {
        writeToStream(m_result);
    }
}

void ScopeTimer::writeToStream(const TimerResult& p_result, int p_deep, bool p_lastChild)
{
    auto l_ws = ws(p_deep);
    m_stream->get() << l_ws << "{\n";
    m_stream->get() << l_ws << "    \"name\": \"" + p_result.m_name + "\",\n";
    m_stream->get() << l_ws << "    \"subresults\": [\n";
    for (int i=0; i<p_result.m_subresults.size(); i++)
    {
        writeToStream(p_result.m_subresults[i], p_deep+1, i+1 == p_result.m_subresults.size());
    }
    m_stream->get() << l_ws << "    ],\n";
    m_stream->get() << l_ws << "    \"duration\": 0.123\n";
    m_stream->get() << l_ws << (!p_lastChild && p_deep != 0 ? "},\n" : "}\n");
}

std::string ScopeTimer::ws(int p_deep)
{
    std::string l_ws("");
    for (auto i=0; i < p_deep; i++)
    {
        l_ws += "        ";
    }
    return l_ws;
}
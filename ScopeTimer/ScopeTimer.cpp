#include "ScopeTimer.hpp"

ScopeTimer::ScopeTimer(std::string p_name) : m_name(p_name),
                                             m_startTime(std::chrono::high_resolution_clock::now()),
                                             m_stream(std::nullopt)
{

}

ScopeTimer::ScopeTimer(std::stringstream& p_stream, std::string p_name) : m_name(p_name), 
                                                                          m_startTime(std::chrono::high_resolution_clock::now()),
                                                                          m_stream(p_stream)
{

}

ScopeTimer::~ScopeTimer()
{
    printResult();
}

void ScopeTimer::printResult()
{
    if (!m_stream) 
    {
        return;
    }
    m_stream->get() << "{\n";
    m_stream->get() << "    \"name\": \"" + m_name + "\",\n";
    m_stream->get() << "    \"subresults\": [\n";
    m_stream->get() << "    ],\n";
    m_stream->get() << "    \"duration\": 0.123\n";
    m_stream->get() << "}\n";
}
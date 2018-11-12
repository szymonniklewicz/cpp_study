#include <sstream>
#include <optional>
#include <functional>
#include <chrono>

class ScopeTimer
{
public:
    ScopeTimer(std::string p_name);
    ScopeTimer(std::stringstream& p_stream, std::string p_name);
    ~ScopeTimer();

private:
    std::string m_name;
    std::chrono::high_resolution_clock::time_point m_startTime;
    std::optional<std::reference_wrapper<std::stringstream>> m_stream;

    void printResult();
};

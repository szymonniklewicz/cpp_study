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
    struct TimerResult
    {
        std::string m_name;
        double m_duration;
        std::vector<TimerResult> m_subresults;
    };

    TimerResult m_result;
    std::chrono::high_resolution_clock::time_point m_startTime;
    std::optional<std::reference_wrapper<std::stringstream>> m_stream;
    
    
    static ScopeTimer* s_currentTop;
    ScopeTimer* m_parent;

    void writeToStream(const TimerResult& p_result, int p_deep = 0, bool p_lastChild = false);
    std::string ws(int p_deep);
};

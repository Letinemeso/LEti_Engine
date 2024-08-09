#include <FPS_Timer.h>

using namespace LEti;


void FPS_Timer::start()
{
    m_start_time_point = std::chrono::steady_clock::now();
}

void FPS_Timer::stop()
{
    m_dt = std::chrono::duration<float, std::ratio<1>>(std::chrono::steady_clock::now() - m_start_time_point).count();
    if(m_max_dt > 0.0f && m_dt > m_max_dt)
        m_dt = m_max_dt;
}

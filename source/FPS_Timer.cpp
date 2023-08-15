#include <FPS_Timer.h>

#include <thread>

using namespace LEti;


void FPS_Timer::update()
{
    m_current_time_point = std::chrono::steady_clock::now();

    m_dt = std::chrono::duration<float, std::ratio<1>>(m_current_time_point - m_prev_time_point).count();
    if(m_max_dt > 0.0f && m_dt > m_max_dt)
        m_dt = m_max_dt;

    m_prev_time_point = m_current_time_point;
}

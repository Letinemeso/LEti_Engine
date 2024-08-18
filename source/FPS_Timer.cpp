#include <FPS_Timer.h>

using namespace LEti;


void FPS_Timer::set_target_fps(unsigned int _value)
{
    m_target_fps = _value;
    m_frame_duration = std::chrono::duration<float>(1.0f / m_target_fps);
}



void FPS_Timer::tick()
{
    std::chrono::high_resolution_clock::time_point current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed_time = current_time - previous_time;
    previous_time = current_time;
    accumulated_time += elapsed_time;

    unsigned int updates_amount = 0;

    while (accumulated_time >= m_frame_duration && updates_amount < 5)
    {
        m_on_tick(m_frame_duration.count());

        accumulated_time -= m_frame_duration;
        ++updates_amount;
    }

    while (accumulated_time >= m_frame_duration)
        accumulated_time -= m_frame_duration;

    std::chrono::duration<float> sleep_time = m_frame_duration - accumulated_time;
    if (sleep_time > std::chrono::duration<float>(0.0f))
        std::this_thread::sleep_for(sleep_time);
}

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

    float dt = elapsed_time.count();
    if(dt > m_frame_duration.count() * 5.0f)
        dt = m_frame_duration.count() * 5.0f;

    m_on_tick(dt);

    std::chrono::duration<float> sleep_time = m_frame_duration - elapsed_time;
    if (sleep_time > std::chrono::duration<float>(0.0f))
        std::this_thread::sleep_for(sleep_time * 0.975f);
}

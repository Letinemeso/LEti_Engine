#include <FPS_Timer.h>

#include <L_Debug/L_Debug.h>

using namespace LEti;


void FPS_Timer::set_target_fps(unsigned int _value)
{
    m_target_fps = _value;
    m_frame_duration = std::chrono::duration<float>(1.0f / m_target_fps);
}



void FPS_Timer::M_update_frames_counter(float _dt)
{
    ++m_frames_counter;
    m_duration_counter += _dt;
    if(m_duration_counter < 1.0f)
        return;

    L_DEBUG_FUNC_NOARG([this]()
    {
        if(m_frames_counter < m_target_fps)
            L_LOG(low_fps_log_level(), "FPS: " + std::to_string(m_frames_counter));
    });

    m_duration_counter = 0.0f;
    m_frames_counter = 0;
}



void FPS_Timer::tick()
{
    std::chrono::high_resolution_clock::time_point current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed_time = current_time - m_previous_time;
    m_previous_time = current_time;

    float dt = elapsed_time.count();
    if(dt > m_frame_duration.count() * 5.0f)
        dt = m_frame_duration.count() * 5.0f;

    m_on_tick(dt);

    M_update_frames_counter(dt);

    std::chrono::duration<float> sleep_time = m_frame_duration - elapsed_time;
    if (sleep_time > std::chrono::duration<float>(0.0f))
        std::this_thread::sleep_for(sleep_time * 0.975f);
}

#pragma once

#include <thread>
#include <chrono>
#include <string>

#include <Stuff/Function_Wrapper.h>


namespace LEti
{

    class FPS_Timer
    {
    private:
        unsigned int m_target_fps = 60;
        std::chrono::duration<float> m_frame_duration{1.0f / m_target_fps};

        std::chrono::high_resolution_clock::time_point m_previous_time = std::chrono::high_resolution_clock::now();

        float m_duration_counter = 0.0f;
        unsigned int m_frames_counter = 0;

        LST::Function<void(float _dt)> m_on_tick;

    public:
        inline static const std::string& low_fps_log_level() { static std::string name = "LOW_FPS"; return name; }

    public:
        inline void set_on_tick(const LST::Function<void(float _dt)>& _on_tick) { m_on_tick = _on_tick; }

        void set_target_fps(unsigned int _value);

    private:
        void M_update_frames_counter(float _dt);

    public:
        void tick();

    };

}

#pragma once

#include <thread>
#include <chrono>

#include <Stuff/Function_Wrapper.h>


namespace LEti
{

    class FPS_Timer
    {
    private:
        unsigned int m_target_fps = 60;
        std::chrono::duration<float> m_frame_duration{1.0f / m_target_fps};

        std::chrono::high_resolution_clock::time_point previous_time = std::chrono::high_resolution_clock::now();

        LST::Function<void(float _dt)> m_on_tick;

    public:
        inline void set_on_tick(const LST::Function<void(float _dt)>& _on_tick) { m_on_tick = _on_tick; }

        void set_target_fps(unsigned int _value);

    public:
        void tick();

    };

}

#pragma once

#include <Stuff/Stopwatch.h>


namespace LEti
{

    class FPS_Timer
    {
    private:
        std::chrono::time_point<std::chrono::steady_clock> m_current_time_point = std::chrono::steady_clock::now();
        std::chrono::time_point<std::chrono::steady_clock> m_prev_time_point = std::chrono::steady_clock::now();

        float m_max_dt = 0.1f;
        float m_dt = 0.0f;

    public:
        inline void set_max_dt(float _max_dt) { m_max_dt = _max_dt; }

    public:
        inline float dt() const { return m_dt; }

    public:
        void update();

    };

}

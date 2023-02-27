#ifndef __TIMER
#define __TIMER

#include "../include/Event_Controller.h"

namespace LEti {

	class Timer
	{
	private:
		float m_alarm_time = 0.0f, m_current_time = 0.0f;
		bool m_active = false;

	public:
		Timer() { }
		~Timer() { }

	public:
		void start(float _alarm_time);
		void reset();

		void update();

        inline bool is_active() const { return m_active; }
        inline float time_left() const { return m_alarm_time - m_current_time; }

	};

}

#endif // __TIMER

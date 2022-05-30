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

		bool is_active() const;

	};

}

#endif // __TIMER

#ifndef __MIDDLE_PHASE_INTERFACE
#define __MIDDLE_PHASE_INTERFACE

#include "Broad_Phase_Interface.h"


namespace LEti
{

	class Middle_Phase_Interface
	{
	private:


	public:
		virtual ~Middle_Phase_Interface();

	public:
		virtual void update(std::list<Broad_Phase_Interface::Colliding_Pair>& _possible_collisions__models, const std::list<Broad_Phase_Interface::Colliding_Point_And_Object>& _possible_collisions__points) const = 0;

	};

}

#endif // __MIDDLE_PHASE_INTERFACE

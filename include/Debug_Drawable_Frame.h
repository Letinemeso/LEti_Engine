#ifndef __DEBUG_DRAWABLE_FRAME
#define __DEBUG_DRAWABLE_FRAME

#include "../include/Object.h"

#include <vector>


namespace LEti {

	class Debug_Drawable_Frame : public Drawable_Object
	{
	private:
		std::vector<glm::vec3> m_points;
		std::vector<unsigned int> m_sequence;
		bool m_changes_were_made = false;

	public:
		Debug_Drawable_Frame& set_point(unsigned int _index, const glm::vec3& _point);
		Debug_Drawable_Frame& set_sequence_element(unsigned int _index, unsigned int _point_index);
		Debug_Drawable_Frame& clear_points();
		Debug_Drawable_Frame& clear_sequence();

	public:
		void draw() const override;
		void update() override;
	};

}

#endif // __DEBUG_DRAWABLE_FRAME

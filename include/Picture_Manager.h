#ifndef PICTURE_MANAGER_H
#define PICTURE_MANAGER_H

#include "Data_Structures/Map.h"

#include "Variable_Base.h"

#include "L_Debug/L_Debug.h"
#include "Picture.h"


namespace LEti
{

	namespace Picture_Manager
	{

		struct Picture_Autoload_Stub : public LV::Variable_Base
		{
		public:
			DECLARE_VARIABLE;

		private:
			unsigned int amount = 0;
			std::string* names = nullptr;
			std::string* paths = nullptr;

        public:
            void on_values_assigned() override;

		public:
			~Picture_Autoload_Stub();

		};

		void add_picture(const std::string& _name, Picture* _picture);
		void remove_picture(const std::string& _name);
		void clear_pictures();

		const Picture* get_picture(const std::string& _name);

	}

}

#endif // PICTURE_MANAGER_H

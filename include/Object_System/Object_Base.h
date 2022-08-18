#ifndef __OBJECT_BASE
#define __OBJECT_BASE


namespace TEST {

	class Object_Base
	{
	public:
		Object_Base() { }
		virtual ~Object_Base() { }

	public:
		virtual void update(float _ratio = 1.0f) = 0;
		virtual void draw() const = 0;

	};

}



#endif // __OBJECT_BASE

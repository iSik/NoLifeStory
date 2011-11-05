////////////////////////////////////////////////////
// This file is part of NoLifeStory.              //
// Please see SuperGlobal.h for more information. //
////////////////////////////////////////////////////

namespace NLS {
	extern sf::Window* window;
	namespace Graphics {
		void Init(bool full);
		void Draw();
		void Unload();
		void setFullScreen();
		extern bool NPOT;
		extern bool Shit;
		extern bool fullScreen;
	}
}

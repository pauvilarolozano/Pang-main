#ifndef _SOUND_PROGRAM_INCLUDE
#define _SOUND_PROGRAM_INCLUDE

#include <irrKlang.h>
using namespace irrklang;

class SoundProgram {

public:
	SoundProgram();
	~SoundProgram();
	static SoundProgram& instance() {
		static SoundProgram SM;
		return SM;
	}

	void init();
	ISoundEngine* getSoundEngine();

private:
	ISoundEngine* engine;
};

#endif // _SOUND_PROGRAM_INCLUDE
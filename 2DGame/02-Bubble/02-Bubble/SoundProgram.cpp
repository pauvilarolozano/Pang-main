#include "SoundProgram.h"

SoundProgram::SoundProgram() {
	engine = NULL;
}

SoundProgram::~SoundProgram() {
	engine->drop();
}

void SoundProgram::init() {
	engine = createIrrKlangDevice();
}

ISoundEngine* SoundProgram::getSoundEngine() {
	return engine;
}
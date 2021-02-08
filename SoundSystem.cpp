#include "SoundSystem.h"

void SoundSystem::play_music(std::string filename) {
    FILE *fp;
    std::string message = "mpg321 " + filename;
    char *cstr = new char[message.size() + 1];
    std::strcpy(cstr, message.c_str());
    fp = popen(cstr, "w");
    pclose(fp);
}
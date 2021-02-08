#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <unistd.h>

class SoundSystem {
    public:
     const std::string filename_bomb = "sources/bomb1.mp3";
     const std::string filename_alarm = "sources/base-siren1.mp3";
     void play_music(std::string filename);
};
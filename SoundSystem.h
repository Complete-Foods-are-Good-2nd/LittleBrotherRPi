#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <unistd.h>

class SoundSystem {
    private:
     FILE *gp;
     const std::string filename_bomb = "bomb1.mp3";
     const std::string filename_alarm = "base-siren1.mp3";

    public:
     void play_bomb();
     void play_alarm();
};
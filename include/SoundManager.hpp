#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#pragma once

#include <Scriptable/State.hpp>

#include <SFML/Audio.hpp>
#include <queue>
#include <map>
#include <vector>


//thread safe
class SoundManager{
    public:
        SoundManager();

        void asyncPlaySound(const std::string& soundName, std::chrono::milliseconds startingOffset = std::chrono::milliseconds(0));

        //TODO: Finish

        static bool loadSound(const std::string& resPath, const std::string& soundName);
        static bool isSoundLoaded(const std::string& soundName);
        static void freeSound(const std::string& soundName);

        ~SoundManager();
    private:
        static std::unordered_map<std::string, sf::SoundBuffer>S_loadedSounds;        
        static std::shared_mutex S_loadedSoundsLock;
        
};

#endif
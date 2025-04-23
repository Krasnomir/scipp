#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#pragma once

#include <Scriptable/State.hpp>

#include <SFML/Audio.hpp>
#include <queue>
#include <map>
#include <mutex>
#include <vector>


class SoundManager{
    public:
        SoundManager() = default;
        
        //sounds cannot yet be freed, TODO: implement S_loadedSounds as a vector of shared_ptrs

        static void asyncPlaySound(const std::string& soundName, std::chrono::milliseconds startingOffset = std::chrono::milliseconds(0));
        static bool loadBuffer(const std::string& resPath, const std::string& soundName);

        static const sf::SoundBuffer* getLoadedBuffer(const std::string& resPath);        

        ~SoundManager() = default;
    private:

        static std::unordered_map<std::string, sf::SoundBuffer>S_loadedSounds;        
        static std::shared_mutex S_loadedSoundsLock;
        
};

#endif
#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#pragma once

#include <Scriptable/State.hpp>

#include <SFML/Audio.hpp>
#include <queue>
#include <map>
#include <vector>


class SoundManager{
    public:
        SoundManager();

        void asyncPlaySound(const std::string& soundName, std::chrono::milliseconds startingOffset = std::chrono::milliseconds(0));

        /*Sound queue, non blocking*/ 
        void play();
        void skip();
        void restart(std::chrono::milliseconds startingOffset = std::chrono::milliseconds(0));
        void pushSound(const std::string& soundName);

        void clearQueue();

        static bool loadSound(const std::string& resPath, const std::string& soundName);
        static bool isSoundLoaded(const std::string& soundName);
        static void freeSound(const std::string& soundName);

        ~SoundManager();
    private:
        void queueHandler();

        enum Flag{
            NONE = 0, RESUME, PAUSE, RESTART, SKIP, TERMINATE
        } M_flag;  

        sf::Time M_offset;

        std::queue<std::string>M_soundQueue;
        std::shared_mutex M_queueLock;

        static std::unordered_map<std::string, sf::SoundBuffer>S_loadedSounds;        
        static std::shared_mutex S_loadedSoundsLock;
        
};

#endif
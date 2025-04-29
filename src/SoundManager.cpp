#include <SoundManager.hpp>
#include <thread>

extern std::unordered_map<std::string, sf::SoundBuffer>SoundManager::S_loadedSounds;
extern std::shared_mutex SoundManager::S_loadedSoundsLock;

bool SoundManager::loadBuffer(const std::string& resPath, const std::string& soundName){
    std::unique_lock writeLock(S_loadedSoundsLock);

    if(S_loadedSounds.contains(soundName)) return false;
    if(!S_loadedSounds[soundName].loadFromFile(Util::getPathToResource(resPath))){
        S_loadedSounds.erase(soundName);
        return false;
    }

    return true;
}

const sf::SoundBuffer* SoundManager::getLoadedBuffer(const std::string& soundName){
    std::shared_lock readLock(S_loadedSoundsLock);

    if(!S_loadedSounds.contains(soundName)) return nullptr;
    return &(S_loadedSounds.at(soundName));
}


void SoundManager::asyncPlaySound(const std::string& soundName, std::chrono::milliseconds startingOffset){
    std::shared_lock readLock(S_loadedSoundsLock);
    
    if(!S_loadedSounds.contains(soundName)) return;

    sf::SoundBuffer buffer(S_loadedSounds[soundName]);

    std::thread soundThread([startingOffset,buffer](){
        sf::Sound sound(buffer);
        sound.setPlayingOffset(sf::milliseconds(startingOffset.count()));
        sound.play();
        std::this_thread::sleep_for(std::chrono::milliseconds(buffer.getDuration().asMilliseconds()));
    });

    soundThread.detach();
}
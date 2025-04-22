#include <SoundManager.hpp>
#include <thread>

SoundManager::SoundManager(){

}

SoundManager::~SoundManager(){

}

extern std::unordered_map<std::string, sf::SoundBuffer>SoundManager::S_loadedSounds;        
extern std::shared_mutex SoundManager::S_loadedSoundsLock;

void SoundManager::freeSound(const std::string& soundName){
    std::unique_lock writeLock(S_loadedSoundsLock);
    if(S_loadedSounds.contains(soundName)) S_loadedSounds.erase(soundName);
}

void SoundManager::asyncPlaySound(const std::string& soundName, std::chrono::milliseconds startingOffset){
    std::shared_lock readLock(S_loadedSoundsLock);
    
    if(!S_loadedSounds.contains(soundName)) return;

    sf::SoundBuffer buffer(S_loadedSounds[soundName]);

    printf("cos\n");

    std::thread soundThread([startingOffset,buffer](){
        sf::Sound sound(buffer);
        sound.setPlayingOffset(sf::milliseconds(startingOffset.count()));
        sound.play();
        std::this_thread::sleep_for(std::chrono::milliseconds(buffer.getDuration().asMilliseconds()));
    });
    soundThread.detach();
}

bool SoundManager::loadSound(const std::string& resPath, const std::string& soundName){
    std::unique_lock writeLock(S_loadedSoundsLock);

    if(S_loadedSounds.contains(soundName)) return false;
    if(S_loadedSounds[soundName].loadFromFile(Util::getPathToResource(resPath))){
        return true;
    }
    S_loadedSounds.erase(soundName);
    return false;
}

bool SoundManager::isSoundLoaded(const std::string& soundName){
    std::shared_lock readLock(S_loadedSoundsLock);

    return S_loadedSounds.contains(soundName);
}
#include <SoundManager.hpp>
#include <thread>

SoundManager::SoundManager(){
    // std::thread(&SoundManager::queueHandler, this).detach();
}

SoundManager::~SoundManager(){
    M_flag = TERMINATE;
    while(M_flag != NONE){}
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

    std::thread soundThread([startingOffset,buffer](){
        sf::Sound sound(buffer);
        sound.setPlayingOffset(sf::milliseconds(startingOffset.count()));
        sound.play();
        std::this_thread::sleep_for(std::chrono::milliseconds(buffer.getDuration().asMilliseconds()));
    });
    soundThread.detach();
}

void SoundManager::play(){
    if(M_flag != NONE) throw std::runtime_error("SoundManager error\n");
    M_flag = Flag::RESUME;
    while(M_flag != Flag::NONE){
        
    }
}

void SoundManager::skip(){
    if(M_flag != NONE) throw std::runtime_error("SoundManager error\n");
    M_flag = Flag::SKIP;
    while(M_flag != Flag::NONE){
        
    }
}

void SoundManager::restart(std::chrono::milliseconds startingOffset){
    M_offset = sf::milliseconds(startingOffset.count());
    if(M_flag != NONE) throw std::runtime_error("SoundManager error\n");
    M_flag = Flag::RESTART;
    while(M_flag != Flag::NONE){

    }
}

void SoundManager::pushSound(const std::string& soundName){
    std::unique_lock writeLock(M_queueLock);
    M_soundQueue.push(soundName);
}

void SoundManager::clearQueue(){
    std::unique_lock writeLock(M_queueLock);
    while(!M_soundQueue.empty()){
        M_soundQueue.pop();
    }
}



void SoundManager::queueHandler(){
    sf::SoundBuffer currentBuffer;
    sf::Sound currentSound;

    while(1){
        switch(M_flag){
            default:
            case NONE:{
                if(currentSound.getPlayingOffset() == currentBuffer.getDuration()){
                    std::shared_lock readLock(M_queueLock);
                    if(isSoundLoaded(M_soundQueue.front())){
                        currentBuffer = S_loadedSounds[M_soundQueue.front()];
                    }
                    M_soundQueue.pop();
                    currentSound.setBuffer(currentBuffer);
                    currentSound.setPlayingOffset(M_offset);
                    currentSound.play();
                }
                break;
            }
            case RESUME:{
                if(currentSound.getStatus() == sf::SoundSource::Paused){
                    currentSound.play();
                }
                else{
                    std::shared_lock readLock(M_queueLock);
                    if(!M_soundQueue.empty()){
                        if(isSoundLoaded(M_soundQueue.front())){
                            currentBuffer = S_loadedSounds[M_soundQueue.front()];
                        }
                        M_soundQueue.pop();
                        currentSound.setBuffer(currentBuffer);
                        currentSound.setPlayingOffset(M_offset);
                        currentSound.play();
                    }
                }
                M_flag = NONE;
                
                break;
            }
            case PAUSE:{
                currentSound.pause();
                M_flag = NONE;
                break;
            }
            case RESTART:{
                currentSound.setPlayingOffset(M_offset);
                currentSound.play();
                M_flag = NONE;
                break;
            }
            case SKIP:{
                std::shared_lock readLock(M_queueLock);
                if(isSoundLoaded(M_soundQueue.front())){
                    currentBuffer = S_loadedSounds[M_soundQueue.front()];
                }
                M_soundQueue.pop();
                currentSound.setBuffer(currentBuffer);
                currentSound.setPlayingOffset(M_offset);
                currentSound.play();
                break;
            };
            case TERMINATE:{
                M_flag = NONE;
                return;
            };
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
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
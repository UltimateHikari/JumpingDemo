#pragma once
#include <memory>
#include <map> 
#include <string>
#include <list>
#include <filesystem>
#include "SDL.h"
#include "SDL_mixer.h"

namespace filesystem = std::filesystem;

template <class T>
struct MusicLoader {};

template <>
struct MusicLoader<Mix_Music> {
	Mix_Music* operator()(const std::string& path) const {
		return Mix_LoadMUS(path.c_str());
	}
};

template <>
struct MusicLoader<Mix_Chunk> {
	Mix_Chunk* operator()(const std::string& path) const {
		return Mix_LoadWAV(path.c_str());
	}
};

template <class T>
struct MusicDeleter {};

template <>
struct MusicDeleter<Mix_Music> {
	void operator()(Mix_Music* ptr) const {
		Mix_FreeMusic(ptr);
	}
};

template <>
struct MusicDeleter<Mix_Chunk> {
	void operator()(Mix_Chunk* ptr) const {
		Mix_FreeChunk(ptr);
	}
};

template <class T>
class SoundManager {
    static SoundManager<T>* p_instance;
	MusicLoader<T> loader;
	using SoundPtr = std::unique_ptr<T, MusicDeleter<T>>;
	std::map<std::string, SoundPtr> sound_list;

	filesystem::path root;

	SoundManager() : loader() {}
	~SoundManager() {}
public:
	static SoundManager<T>* getInstance() {
		if (!p_instance)
			p_instance = new SoundManager<T>;
		return p_instance;
	}

	static void shutdown() {
        delete p_instance;
		p_instance = nullptr;
    }

	void load(const std::string& name, const std::string& path) {
		filesystem::path file_path = filesystem::canonical(root / path);
		T* sound = loader(file_path.string().c_str());
		if (!sound) 
			throw std::exception("Couldn't load music");
		sound_list[name] = SoundPtr(sound, MusicDeleter<T>{});
	}

	
	void setDefaultPath(const std::string &root_) {
    	root = filesystem::canonical(filesystem::absolute(root_));
	}

	T* get(const std::string& name) const {
		auto it = sound_list.find(name);
		if (it == sound_list.end()) 
			throw std::out_of_range("Sound not found");
		return it->second.get();
	}

	void clear() {
		sound_list.clear();
	}
};

template <class T>
SoundManager<T>* SoundManager<T>::p_instance = nullptr;

# pragma once
# include <Siv3D.hpp> // OpenSiv3D v0.4.2

# include "miditone-client/MiditoneClient.hpp"



# define BUTTON_VERSION

# ifdef BUTTON_VERSION
# define WII_VERSION
# endif

// シーンの名前
enum class State {
	Title,

	Game
};

// ゲームデータ
struct GameData {
	// API Client
	api_client::MiditoneClient apiClient;
    // Music List
	Array<api_client::response::music_attr> musics;
    int currentMusicId;
};

// シーン管理クラス
using MyApp = SceneManager<State, GameData>;


# include "Game.hpp"

Game::Game(const InitData& init)
	: IScene(init) {

    {
        const auto result = getData().apiClient.get_button_played_times().all();
        if (!result) {
            Logger << U"[get button ranking of music no." << getData().currentMusicId << U"] Connection Failed";
            connectionFailed = true;
        }

        const auto& body = result.success_value().parsed_body();
        for (const auto& item : body) {
            const auto music = std::find_if(
                getData().musics.cbegin(),
                getData().musics.cend(),
                [&] (const api_client::response::music_attr& music) { return music.id == item.music_id; }
            );
            if (music != getData().musics.cend())
                button_ranking.push_back(played_times_t(item.times.value_or(0), *music));
        }

        button_ranking.sort_by( [] (const played_times_t& a, const played_times_t& b) {
            return a.times > b.times;
        });
    }
	
    {
        const auto result = getData().apiClient.get_board_played_times().all();
        if (!result) {
            Logger << U"[get board ranking of music no." << getData().currentMusicId << U"] Connection Failed";
            connectionFailed = true;
        }

        const auto& body = result.success_value().parsed_body();
        for (const auto& item : body) {
            const auto music = std::find_if(
                getData().musics.cbegin(),
                getData().musics.cend(),
                [&](const api_client::response::music_attr& music) { return music.id == item.music_id; }
            );

            if (music != getData().musics.cend())
                board_ranking.push_back(played_times_t(item.times.value_or(0), *music));
        }

        board_ranking.sort_by( [] (const played_times_t& a, const played_times_t& b) {
            return a.times > b.times;
        });
    }

	connectionFailed = connectionFailed || false;

    sceneTime.start();
}

void Game::update() {
    // シーン切り替え
    if (sceneTime.s() > sceneDisplaySec) {
        if (getData().currentMusicId >= getData().musics.back().id) {
            changeScene(State::Title);
            getData().currentMusicId = 0;
        } else {
            changeScene(State::Game);
            getData().currentMusicId++;
        }
    }
}

void Game::draw() const {
	FontAsset(U"h3")(
		U"{}"_fmt(sceneDisplaySec - sceneTime.s())
    ).draw(0, 0);
    
	const auto& font = FontAsset(U"h4");
	for (size_t i = 0; i < 10; i++) {
		// 順位
        font(i + 1).draw(10, 40 + 40 * i);
        
      	String music_title, music_artist, times;

        if (i < button_ranking.size()) {
            const auto& item = button_ranking[i];
            music_title = Unicode::Widen(item.music.title);
            music_artist = Unicode::Widen(item.music.artist);
            times = U"{:>11}"_fmt(item.times);
        } else {
        	music_title = U"---";
         	music_artist = U"---";
          	times = U"---";
        }
        
        // 名前
        font(music_title).draw(30, 40 + 40 * i);
        // QRコード
        font(music_artist).draw(200, 40 + 40 * i);
        // スコア
        font(times).draw(400, 40 + 40 * i);
    }
}

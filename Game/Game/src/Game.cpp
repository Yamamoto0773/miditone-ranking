
# include "Game.hpp"

Game::Game(const InitData& init)
	: IScene(init) {

    sceneTime.start();
    
# ifdef BUTTON_VERSION
	const auto result = getData().apiClient.get_button_score_ranking(getData().currentMusicId).all();
# else
	const auto result = getData().apiClient.get_board_score_ranking(getData().currentMusicId).all();
# endif
	if (!result) {
		Logger << U"[get ranking of music no." << getData().currentMusicId << U"] Connection Failed";
  		connectionFailed = true;
    }

	const auto& body = result.success_value().parsed_body();
   	ranking.insert(ranking.cend(), body.cbegin(), body.cend());
		
	connectionFailed = false;
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
        
      	String name, qrcode, score;

        if (i < ranking.size()) {
            const auto& item = ranking[i];
            name = Unicode::Widen(item.user.name);
            qrcode = Unicode::Widen(item.user.qrcode);
            score = U"{:>11}"_fmt(item.score.points.value_or(0));
        } else {
        	name = U"---";
         	qrcode = U"---";
          	score = U"---";
        }
        
        // 名前
        font(name).draw(30, 40 + 40 * i);
        // QRコード
        font(qrcode).draw(200, 40 + 40 * i);
        // スコア
        font(score).draw(400, 40 + 40 * i);
    }
}

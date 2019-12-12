
# pragma once
# include "Common.hpp"


// ゲームシーン
class Game : public MyApp::Scene {
private:

	// シーンの表示時間
	const double sceneDisplaySec = 10;

	Stopwatch sceneTime;
	
	Array<api_client::response::musics_score_t> ranking;
	
	bool connectionFailed;

public:

	Game(const InitData& init);

	void update() override;

	void draw() const override;
};

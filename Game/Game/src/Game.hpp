
# pragma once
# include "Common.hpp"


struct played_times_t {
    int times;
    api_client::response::music_attr music;

    played_times_t(int t, api_client::response::music_attr m) : times(t), music(m) {}
};


// ゲームシーン
class Game : public MyApp::Scene {
private:

	// シーンの表示時間
	const double sceneDisplaySec = 10;

	Stopwatch sceneTime;
	
    Array<played_times_t> button_ranking;
    Array<played_times_t> board_ranking;
	
	bool connectionFailed;

public:

	Game(const InitData& init);

	void update() override;

	void draw() const override;
};

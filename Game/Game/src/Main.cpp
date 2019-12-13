
//
// OpenSiv3D v0.4.2 ゲームテンプレート
//

//
// macOS では README.md と同じフォルダに
// include と lib をコピーしてください。
//

# include "Common.hpp"
# include "Title.hpp"
# include "Game.hpp"

void Main()
{
	// （ウィンドウの閉じるボタンを押したときだけ終了する場合はコメントを外す）
	//System::SetTerminationTriggers(UserAction::CloseButtonClicked);

	// タイトルを設定
	Window::SetTitle(U"miditone#");

    Window::Resize(config::windowSize, WindowResizeOption::ResizeSceneSize, false);
    // Window::SetFullscreen(true, config::windowSize, WindowResizeOption::ResizeSceneSize);

	// 背景色を設定
	Scene::SetBackground(ColorF(0.2, 0.8, 0.4));

	// 使用するフォントアセットを登録
	FontAsset::Register(U"h1", 120, U"example/font/AnnyantRoman/AnnyantRoman.ttf");
	FontAsset::Register(U"h2", 60, Typeface::Regular);
    FontAsset::Register(U"h3", 30, Typeface::Regular);
    FontAsset::Register(U"h4", 24, Typeface::Regular);
    FontAsset::Register(U"h5", 18, Typeface::Regular);
    FontAsset::Register(U"h6", 10, Typeface::Regular);

	// 設定読み込み
	const JSONReader settings(U"settings.json");
	if (!settings) {
		Logger << U"Cannot open settings.json";
    }

	const String host = settings[U"client.host"].getString();
	const String port = settings[U"client.port"].getString();
	const String token = settings[U"client.token"].getString();
	const Array<int32> excpetMusicIds = settings[U"except_music_ids"].getArray<int32>();

	// ゲームデータの初期化
	std::shared_ptr<GameData> gameData(new GameData());
	gameData->apiClient.destination(host.narrow(), port.narrow()).token(token.narrow());
	gameData->currentMusicId = 0;

	{
        const auto result = gameData->apiClient.get_musics().all();
        if (!result) {
            Logger << U"[get musics] Connection Failed";
            return ;
        }

        const auto& response = result.success_value();
        if (response.status_code() != 200) {
            Logger << U"[get musics] status code " << response.status_code();
            return ;
        }

        if (response.pagination().total_records == 0) {
        	Logger << U"No musics to show";
	        return ;
        }

        for (const auto& music : response.parsed_body()) {
            if (!excpetMusicIds.includes(music.id)) {
                gameData->musics.push_back(music);
            }
        }
    }

	// シーンと遷移時の色を設定
	MyApp manager(gameData);
	manager
		.add<Title>(State::Title)
		.add<Game>(State::Game)
		.setFadeColor(ColorF(1.0));

	// （ゲームシーンから開始する場合はコメントを外す）
	//manager.init(State::Game);

	while (System::Update())
	{
		if (!manager.update())
		{
			break;
		}
	}
}

//
// = クロスプラットフォーム開発の注意 =
//
// Xcode でソースコードを保存すると UTF-8 形式 (BOM 無し) になります。
// MSVC で作業するときはソースコードを UTF-8 (BOM 付き) に直す必要があります。
//
//
// = アドバイス =
// macOS 10.15 Catalina の Xcode プロジェクトで、
// ビルドしたプログラムを起動するたびにファイルアクセス許可のダイアログが表示される場合、
// プロジェクトのフォルダを User/アプリケーション に移動させることで通常は表示されなくなります。
// 特別なファイルシステム関数の使用や、Web カメラ、マイク使用時のダイアログまでは消せません。
//
//
// = アドバイス =
// Debug ビルドではプログラムの最適化がオフになります。
// 実行速度が遅いと感じた場合は Release ビルドを試しましょう。
// アプリをリリースするときにも、Release ビルドにするのを忘れないように！
//
// 思ったように動作しない場合は「デバッグの開始」でプログラムを実行すると、
// 出力ウィンドウに詳細なログが表示されるので、エラーの原因を見つけやすくなります。
//
//
// = お役立ちリンク =
//
// OpenSiv3D リファレンス
// https://siv3d.github.io/ja-jp/
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/basic/
//
// よくある間違い
// https://siv3d.github.io/ja-jp/articles/mistakes/
//
// 質問などのサポートについて
// https://siv3d.github.io/ja-jp/support/support/
//
// Siv3D Slack (ユーザコミュニティ) への参加
// https://siv3d.github.io/ja-jp/community/community/
//
// 新機能の提案やバグの報告
// https://github.com/Siv3D/OpenSiv3D/issues
//

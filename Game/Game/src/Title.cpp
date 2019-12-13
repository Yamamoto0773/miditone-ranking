
# include "Title.hpp"

Title::Title(const InitData& init)
	: IScene(init)
{

}

void Title::update()
{
	m_startTransition.update(m_startButton.mouseOver());
	m_exitTransition.update(m_exitButton.mouseOver());

	if (m_startButton.mouseOver() || m_exitButton.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	if (m_startButton.leftClicked())
	{
		changeScene(State::Game);
	}

	if (m_exitButton.leftClicked())
	{
		System::Exit();
	}
}

void Title::draw() const
{
	const String titleText = U"miditone#";
	const Vec2 center(Scene::Center().x, 120);
	FontAsset(U"h1")(titleText).drawAt(center.movedBy(4, 6), ColorF(0.0, 0.5));
	FontAsset(U"h1")(titleText).drawAt(center);

	m_startButton.draw(ColorF(1.0, m_startTransition.value())).drawFrame(2);
	m_exitButton.draw(ColorF(1.0, m_exitTransition.value())).drawFrame(2);

	FontAsset(U"h3")(U"はじめる").drawAt(m_startButton.center(), ColorF(0.25));
	FontAsset(U"h3")(U"おわる").drawAt(m_exitButton.center(), ColorF(0.25));

	Rect(0, 500, Scene::Width(), Scene::Height() - 500)
		.draw(Arg::top = ColorF(0.0, 0.0), Arg::bottom = ColorF(0.0, 0.5));

}

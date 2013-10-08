/*
 *  RP_Game.h
 *  Memory
 *
 *  Created by Trevor Powell on 22/07/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef RP_GAME_H
#define RP_GAME_H

class rpFader;

class rpGame : public coreGame, public vsSingleton<rpGame>
{
public:
	enum GameMode
	{
//		Mode_Intro,
//		Mode_TitleScreen,
//		Mode_Tutorial,
		Mode_InGame,
		Mode_Credits,
		MODE_MAX
	};

	enum Music
	{
		//Music_Warmup,
		Music_Main,
		MUSIC_MAX
	};

private:

	typedef coreGame Parent;

	coreGameMode *			m_mode[MODE_MAX];
	vsMusic *				m_music[MUSIC_MAX];
	int						m_currentMusic;

	vsSoundSample *			m_registerSound;

	rpFader *				m_fader;

	bool					m_showedIntro;
	bool					m_enteringFromIntro;

public:
	rpGame();
	virtual			~rpGame();

	virtual void	Init();
	virtual void	Deinit();

	void			SetEnteringFromIntro() { m_enteringFromIntro = true; }
	bool			EnteringFromIntro() { bool result = m_enteringFromIntro;  m_enteringFromIntro = false; return result; }

	void			SetMode( GameMode mode );
	void			PlayMusic( Music type );

	void			FadeIn();
	void			FadeOut();
	bool			IsFading();

	void			PlayRegisterSound();

	void			GoToPreferences();

	virtual void	Update(float timeStep);
};


#endif // RP_GAME_H


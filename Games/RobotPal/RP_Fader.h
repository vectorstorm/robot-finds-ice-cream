/*
 *  RP_Fader.h
 *  RobotPal
 *
 *  Created by Trevor Powell on 27/07/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef RP_FADER_H
#define RP_FADER_H

class rpFader : public vsSprite
{
	int			m_fadeType;

	float		m_alpha;
	float		m_startAlpha;
	float		m_endAlpha;
	float		m_timer;
	float		m_fadeDuration;
	bool		m_fading;

	vsMaterial *m_material;

public:

	rpFader();
	virtual ~rpFader();

	void	FadeTo(float alpha);
	void	FadeOut() { FadeTo(1.f); }
	void	FadeIn() { FadeTo(0.f); }
	bool	IsFading() { return m_fading; }

	virtual void	Update( float timeStep );
	virtual void	DynamicDraw( vsRenderQueue *queue );
};

#endif // RP_FADER_H


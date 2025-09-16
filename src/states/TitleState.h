//
// Created by svjat on 8/15/2025.
//

#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "DefaultState.h"

class TitleState : public DefaultState {
public:
	explicit TitleState(StateManager* state_manager, Application* application);

	void HandleEvents(sf::Event& event) override;
	void HandleInput(float delta_time) override;
	void Update(sf::Time delta_time) override;
	void Draw() override;
	~TitleState() override = default;

private:
	void TitleWindow();

	void HandlePlayButton();
	void HandleOptionsButton();
	void HandleExitButton();
};



#endif //TITLESTATE_H

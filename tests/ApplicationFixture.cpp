//
// Created by svjat on 8/14/2025.
//

#include "ApplicationFixture.h"


ApplicationFixture::ApplicationFixture() {
	application_default_ = new Application(win_width,win_height, fps);
	application_high_ = new Application(win_width * 2,win_height * 2, fps * 2);
};

ApplicationFixture::~ApplicationFixture() {
	delete application_default_;
	delete application_high_;
}
//
// Created by svjat on 8/14/2025.
//

#ifndef APPLICATIONFIXTURE_H
#define APPLICATIONFIXTURE_H

#include <gtest/gtest.h>
#include "../src/application/Application.h"

class ApplicationFixture : public testing::Test {
protected:
	int win_width = 1000;
	int win_height = 800;
	int fps = 60;

	ApplicationFixture();
	~ApplicationFixture() override;

	Application* application_default_;
	Application* application_high_;

};

#endif //APPLICATIONFIXTURE_H

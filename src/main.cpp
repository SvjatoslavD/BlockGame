#include "Application.h"

// ---- code is primarily modeled after code found on learnOpenGL.com and the code posted by Victor Gordan, but modified to be used within an SFML context ----

int main() {
	Application application(1200,1000, 60);

	application.StartApplication();

    return 0;
}
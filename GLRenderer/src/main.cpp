#include <iostream>
#include <Engine.hpp>

int main() {
	Engine app;

	app.initialize();
	app.run();
	app.shutdown();

	return 0;
}
#include <iostream>
#include <Engine.hpp>
#include <filesystem>

int main() {
	try {
		Engine app;

		app.initialize();
		app.run();
		app.shutdown();
	}
	catch (const std::exception& e) {
		std::cerr << "[Fatal Error] Exception caught: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}
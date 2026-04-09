#include "../include/display.hpp"
#include "../include/input.hpp"
#include "../include/wordtracker.hpp"
#include <iostream>

int main() {
	std::cout << "\033[5 q"; // blinking bar cursor
	std::string sentence = "the quick brown fox jumps over the lazy dog";

	WordTracker tracker;
	tracker.setTargetText(sentence);

	KeyboardInput input;
	bool running = true;
	bool typingStarted = false;

	while (running && !tracker.isSentenceComplete()) {
		clearScreen();

		displayWordBasedText(tracker.getTargetWords(), tracker.getTypedWords(),
		                     tracker.getCompletedWords(), tracker.getCurrentWord());

		char key = input.getKey();

		if (!typingStarted && key != 27) {
			std::cout << "\033[6 q"; // steady bar once typing starts
			typingStarted = true;
		}

		if (input.isEscape(key))          running = false;
		else if (input.isBackspace(key))  tracker.handleBackspace();
		else if (key == ' ')              tracker.handleSpace();
		else if (key >= 32 && key <= 126) tracker.handleChar(key);
	}

	clearScreen();
	displayWordAccuracy(tracker.getResults());
	std::cout << "\033[0 q"; // restore default cursor

	std::cin.get();
	return 0;

}

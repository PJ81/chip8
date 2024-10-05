#include "chip8.h"

int openFile(const char* fileName) {
	std::ifstream file;

	file.open(fileName, std::ios::binary);
	if (!file) {
		printf_s("%s", "Error opening the file!\n");
		return 1;
	} 

	Chip8 chip8;

	chip8.loadRom(file);
	file.close();

	InitWindow(640, 320, "Chip 8");
	SetTargetFPS(60);

	while (!WindowShouldClose() && !chip8.terminate) {
		chip8.emulate();
	}

	CloseWindow();

	return 0;
}

int main(int argc, char** argv) {

	srand((unsigned int)time(0));

	if (argc > 1) return openFile(argv[1]);
	else return openFile("D:\\#_DEV_#\\_projects\\chip8-rev4\\chip8\\splash.ch8");
}
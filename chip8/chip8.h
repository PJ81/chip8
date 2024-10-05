#pragma once

#include "stack.h"
#include "keyboard.h"
#include <fstream>

class Chip8 {

	typedef bool (Chip8::* Chip8Func)(unsigned short opCode);

	enum CPUType {
		CHIP_8, 
		S_CHIP,
		XO_CHIP
	};

public:
	Chip8();
	~Chip8();

	void loadRom(std::ifstream& file);
	void emulate();
	bool terminate;

private:

	bool isHalting();
	void blit();
	void createFuncTables();

	bool nul(unsigned short opCode);
	bool x00(unsigned short opCode);
	bool x01(unsigned short opCode);
	bool x02(unsigned short opCode);
	bool x03(unsigned short opCode);
	bool x04(unsigned short opCode);
	bool x05(unsigned short opCode);
	bool x06(unsigned short opCode);
	bool x07(unsigned short opCode);
	bool x08(unsigned short opCode);
	bool x09(unsigned short opCode);
	bool x0A(unsigned short opCode);
	bool x0B(unsigned short opCode);
	bool x0C(unsigned short opCode);
	bool x0D(unsigned short opCode);
	bool x0E(unsigned short opCode);
	bool x0F(unsigned short opCode);
	bool cls(unsigned short opCode);
	bool scu(unsigned short opCode);
	bool scd(unsigned short opCode);
	bool scr(unsigned short opCode);
	bool scl(unsigned short opCode);
	bool exit(unsigned short opCode);
	bool low(unsigned short opCode);
	bool high(unsigned short opCode);
	bool ret(unsigned short opCode);
	bool ld_vx_vy(unsigned short opCode);
	bool or_vx_vy(unsigned short opCode);
	bool and_vx_vy(unsigned short opCode);
	bool xor_vx_vy(unsigned short opCode);
	bool add_vx_vy(unsigned short opCode);
	bool sub_vx_vy(unsigned short opCode);
	bool shr_vx_vy(unsigned short opCode);
	bool subn_vx_vy(unsigned short opCode);
	bool shl_vx_vy(unsigned short opCode);
	bool ld_hf_vx(unsigned short opCode);
	bool ld_dt_vx(unsigned short opCode);
	bool ld_b_vx(unsigned short opCode);
	bool ld_i_vx(unsigned short opCode);
	bool ld_vx_i(unsigned short opCode);
	bool ld_vx_dt(unsigned short opCode);
	bool ld_st_vx(unsigned short opCode);
	bool ld_f_vx(unsigned short opCode);
	bool ld_vx_k(unsigned short opCode);
	bool ld_r_vx(unsigned short opCode);
	bool ld_vx_r(unsigned short opCode);
	bool add_i_vx(unsigned short opCode);
	void clear();
	void updateResolution();
	unsigned char putSprite(int x, int y, int cnt, unsigned short start);
	unsigned char putSprite16(int x, int y, int cnt, unsigned short start);
	unsigned char getX(unsigned short opCode);
	unsigned char getY(unsigned short opCode);

	bool lowRes;
	int ticksPerSec;
	unsigned char* imgBuffer;
	unsigned short I, PC, vX;
	unsigned char memory[4096], V[16], flagsStorage[16],
		delay, sound, wid, hei, state, pixelSize;
	
	CPUType cpuType;
	std::streampos EoF;
	Stack stack;
	Keyboard keyboard;
	Color backClr, foreClr;

	Chip8Func opCodeTbl[16];
	Chip8Func zeroTbl[16];
	Chip8Func eightTbl[16];
	Chip8Func ffTbl[16];
};
#pragma once

#include "chip8.h"

inline bool Chip8::nul(unsigned short opCode) {
	return true;
}

inline bool Chip8::x00(unsigned short opCode) {
    switch ((opCode & 0x00f0) >> 4) {
    case 0xe:
        return (this->*zeroTbl[opCode & 0x000f])(opCode);
    case 0xf:
        return (this->*zeroTbl[(opCode & 0x000f) - 2])(opCode);
    }
    return (this->*zeroTbl[((opCode & 0x00f0) >> 4) - 5])(opCode);
}

inline bool Chip8::x01(unsigned short opCode) {
    PC = (unsigned short)(opCode & 0x0fff);
    return false;
}

inline bool Chip8::x02(unsigned short opCode) {
    stack.push(PC);
    PC = (unsigned short)(opCode & 0x0fff);
    return false;
}

inline bool Chip8::x03(unsigned short opCode) {
    if (V[getX(opCode)] == (opCode & 0x00ff)) PC += 2;
    return true;
}

inline bool Chip8::x04(unsigned short opCode) {
    if (V[getX(opCode)] != (opCode & 0x00ff)) PC += 2;
    return true;
}

inline bool Chip8::x05(unsigned short opCode) {
    if (V[getX(opCode)] == V[getY(opCode)]) PC += 2;
    return true;
}

inline bool Chip8::x06(unsigned short opCode) {
    V[getX(opCode)] = ((unsigned char)(opCode & 0x00ff));
    return true;
}

inline bool Chip8::x07(unsigned short opCode) {
    V[getX(opCode)] += (unsigned char)((opCode & 0x00ff));
    return true;
}

inline bool Chip8::x08(unsigned short opCode) {
    return (this->*eightTbl[opCode & 0x000f])(opCode);
}

inline bool Chip8::x09(unsigned short opCode) {
    if (V[getX(opCode)] != V[getY(opCode)]) PC += 2;
    return true;
}

inline bool Chip8::x0A(unsigned short opCode) {
    I = (unsigned short)(opCode & 0x0fff);
    return true;
}

inline bool Chip8::x0B(unsigned short opCode) {
    unsigned short nnn = (unsigned short)(opCode & 0x0fff);
    PC = (cpuType != S_CHIP) ? (unsigned short)(V[0] + nnn) : (unsigned short)(V[getX(opCode)] + nnn);
    return false;
}

inline bool Chip8::x0C(unsigned short opCode) {
    unsigned char r = (unsigned char)rand() % 256,
        kk = (unsigned char)(opCode & 0x00ff);
    V[getX(opCode)] = (unsigned char)(r & kk);
    return true;
}

inline bool Chip8::x0D(unsigned short opCode) {
    unsigned char first = (unsigned char)((opCode & 0xf000) >> 12), x = getX(opCode),
        y = getY(opCode), last = (unsigned char)(opCode & 0x000f);

    if (last == 0) {
        V[0x0f] = putSprite16(V[x], V[y], 16, I);
        return true;
    }

    V[0x0f] = putSprite(V[x], V[y], last, I);
    return true;
}

inline bool Chip8::x0E(unsigned short opCode) {
    unsigned char x = getX(opCode);
    if ((opCode & 0x000f) == 0xE) {
        if (keyboard.isPressed(V[x])) PC += 2;
    } else {
        if (!keyboard.isPressed(V[x])) PC += 2;
    }
    return true;
}

inline bool Chip8::x0F(unsigned short opCode) {
    if ((opCode & 0x000f) == 5) {
        unsigned char b = (unsigned char)((opCode & 0x00f0) >> 4);
        if (b > 6) b += 4;
        return (this->*ffTbl[b])(opCode);
    }
    return (this->*ffTbl[opCode & 0x000f])(opCode);
}

inline bool Chip8::cls(unsigned short opCode) {
    clear();
    return true;
}

inline bool Chip8::scu(unsigned short opCode) {
    unsigned char cnt = (unsigned char)(opCode & 0x000f);
    int y = cnt, yy, ii;

    for (int i = 0; i < hei; i++) {
        yy = y * wid; ii = i * wid;
        for (int x = 0; x < wid; x++) {
            imgBuffer[x + ii] = (unsigned char)(y >= hei ? 0 : imgBuffer[x + yy]);
        }
        y++;
    }

    return true;
}

inline bool Chip8::scd(unsigned short opCode) {
    unsigned char cnt = (unsigned char)(opCode & 0x000f);
    int y = hei - 1 - cnt, yy, ii;

    for (int i = hei - 1; i > -1; i--) {
        yy = y * wid; ii = i * wid;
        for (int x = 0; x < wid; x++) {
            imgBuffer[x + ii] = (unsigned char)(y < 0 ? 0 : imgBuffer[x + yy]);
        }
        y--;
    }

    return true;
}

inline bool Chip8::scr(unsigned short opCode) {
    int yy, x;
    for (int y = 0; y < hei; y++) {
        x = wid - 1 - 4;// lowRes ? 2 : 4;
        yy = y * wid;
        for (int i = wid - 1; i > -1; i--) {
            imgBuffer[i + yy] = (unsigned char)(x < 0 ? 0 : imgBuffer[x + yy]);
            x--;
        }
    }
    return true;
}

inline bool Chip8::scl(unsigned short opCode) {
    int w = wid, yy, x;
    for (int y = 0; y < hei; y++) {
        x = 4;// lowRes ? 2 : 4;
        yy = y * wid;
        for (int i = 0; i < w; i++) {
            imgBuffer[i + yy] = (unsigned char)(x > w - 5 ? 0 : imgBuffer[x + yy]);
            x++;
        }
    }
    return true;
}

inline bool Chip8::exit(unsigned short opCode) {
    terminate = true;
    return true;
}

inline bool Chip8::low(unsigned short opCode) {
    lowRes = true;
    updateResolution();
    return true;
}

inline bool Chip8::high(unsigned short opCode) {
    lowRes = false;
    updateResolution();
    return true;
}

inline bool Chip8::ret(unsigned short opCode) {
    PC = stack.pop();
    return true;
}

inline bool Chip8::ld_vx_vy(unsigned short opCode) {
    V[getX(opCode)] = V[getY(opCode)];
    return true;
}

inline bool Chip8::or_vx_vy(unsigned short opCode) {
    V[getX(opCode)] |= V[getY(opCode)];
    if (cpuType == CHIP_8) V[0xf] = 0;
    return true;
}

inline bool Chip8::and_vx_vy(unsigned short opCode) {
    V[getX(opCode)] &= V[getY(opCode)];
    if (cpuType == CHIP_8) V[0xf] = 0;
    return true;
}

inline bool Chip8::xor_vx_vy(unsigned short opCode) {
    V[getX(opCode)] ^= V[getY(opCode)];
    if (cpuType == CHIP_8) V[0xf] = 0;
    return true;
}

inline bool Chip8::add_vx_vy(unsigned short opCode) {
    unsigned char x = getX(opCode);
    unsigned short a = (unsigned short)(V[x] + V[getY(opCode)]);
    V[x] = ((unsigned char)(a & 0x00ff));
    V[0x0f] = (unsigned char)(a > 255 ? 1 : 0);
    return true;
}

inline bool Chip8::sub_vx_vy(unsigned short opCode) {
    unsigned char x = getX(opCode);
    short r = (short)(V[x] - V[getY(opCode)]);
    V[x] = ((unsigned char)(r & 0xff));
    V[0x0f] = (unsigned char)(r < 0 ? 0 : 1);
    return true;
}

inline bool Chip8::shr_vx_vy(unsigned short opCode) {
    unsigned char x = getX(opCode);
    if (cpuType != S_CHIP) V[x] = V[getY(opCode)];
    unsigned char r = (unsigned char)(V[x] & 0x01);
    V[x] >>= 1;
    V[0x0f] = (unsigned char)r;
    return true;
}

inline bool Chip8::subn_vx_vy(unsigned short opCode) {
    unsigned char x = getX(opCode);
    short r = (short)(V[getY(opCode)] - V[x]);
    V[x] = ((unsigned char)(r & 0xff));
    V[0x0f] = (unsigned char)(r < 0 ? 0 : 1);
    return true;
}

inline bool Chip8::shl_vx_vy(unsigned short opCode) {
    unsigned char x = getX(opCode);
    if (cpuType != S_CHIP) V[x] = V[getY(opCode)];
    unsigned char r = (unsigned char)(V[x] >> 7);
    V[x] <<= 1;
    V[0x0f] = (unsigned char)r;
    return true;
}

inline bool Chip8::ld_hf_vx(unsigned short opCode) {
    I = (unsigned short)((V[getX(opCode)] * 10) + 16 * 5);
    return true;
}

inline bool Chip8::ld_dt_vx(unsigned short opCode) {
    delay = V[getX(opCode)];
    return true;
}

inline bool Chip8::ld_b_vx(unsigned short opCode) {
    unsigned char r = V[getX(opCode)], i = 0, d = 100;
    while (d > 0) {
        memory[I + i] = (unsigned char)(r / d);
        r %= d;
        i++;
        d /= 10;
    }
    return true;
}

inline bool Chip8::ld_i_vx(unsigned short opCode) {
    unsigned char x = getX(opCode);
    unsigned short l = I;
    for (int i = 0; i <= x; i++)
        memory[l++] = V[i];
    if (cpuType != S_CHIP) I = l;
    return true;
}

inline bool Chip8::ld_vx_i(unsigned short opCode) {
    unsigned char x = getX(opCode);
    unsigned short l = I;
    for (int i = 0; i <= x; i++)
        V[i] = memory[l++];
    if (cpuType != S_CHIP) I = l;
    return true;
}

inline bool Chip8::ld_vx_dt(unsigned short opCode) {
    V[getX(opCode)] = delay;
    return true;
}

inline bool Chip8::ld_st_vx(unsigned short opCode) {
    sound = V[getX(opCode)];
    return true;
}

inline bool Chip8::ld_f_vx(unsigned short opCode) {
    I = (unsigned short)(V[getX(opCode)] * 5);
    return true;
}

inline bool Chip8::ld_vx_k(unsigned short opCode) {
    state = 1;
    vX = getX(opCode);
    return true;
}

inline bool Chip8::ld_r_vx(unsigned short opCode) {
    unsigned char x = getX(opCode);
    for (int i = 0; i <= x; i++)
        flagsStorage[i] = V[i];
    return true;
}

inline bool Chip8::ld_vx_r(unsigned short opCode) {
    unsigned char x = getX(opCode);
    for (int i = 0; i <= x; i++)
        V[i] = flagsStorage[i];
    return true;
}

inline bool Chip8::add_i_vx(unsigned short opCode) {
    I += V[getX(opCode)];
    return true;
}

inline void Chip8::clear() {
    memset(imgBuffer, 0, wid * hei);
}

inline unsigned char Chip8::putSprite(int x, int y, int cnt, unsigned short start) {

    unsigned char b, f = 0;
    int yy, xx;

    if (x >= wid) x %= wid;
    if (y >= hei) y %= hei;

    for (int c = 0; c < cnt; c++) {

        if (cpuType == XO_CHIP) {
            yy = (y + c + hei) % hei;
        }
        else {
            yy = y + c;
            if (yy >= hei) break;
        }

        xx = x;
        b = memory[c + start];

        while (b > 0) {
            if ((b & 0x80) > 0) {
                if (imgBuffer[xx + yy * wid] == 1) f = 1;
                imgBuffer[xx + yy * wid] ^= 1;
            }

            if (cpuType == XO_CHIP) {
                xx = (xx + 1 + wid) % wid;
            }
            else {
                xx++;
                if (xx >= wid) break;
            }

            b <<= 1;
        }
    }

    return f;
}

inline unsigned char Chip8::putSprite16(int x, int y, int cnt, unsigned short start) {

    unsigned char f = 0;
    unsigned short b;
    int yy, xx;

    if (x >= wid) x %= wid;
    if (y >= hei) y %= hei;

    for (int c = 0; c < cnt; c++) {

        if (cpuType == S_CHIP) {
            yy = (y + c + hei) % hei;
        }
        else {
            yy = y + c;
            if (yy >= hei) break;
        }


        xx = x;
        b = (unsigned short)(memory[c + start] << 8 | memory[c + start + 1]);

        while (b > 0) {
            if ((b & 0x8000) > 0) {
                if (imgBuffer[xx + yy * wid] == 1) f = 1;
                imgBuffer[xx + yy * wid] ^= 1;
            }

            if (cpuType == XO_CHIP) {
                xx = (xx + 1 + wid) % wid;
            }
            else {
                xx++;
                if (xx >= wid) break;
            }

            b <<= 1;
        }
        start++;
    }
    return f;
}

inline void Chip8::updateResolution() {

    if (lowRes) {
        pixelSize = 10;
        wid = 64;
        hei = 32;
    }
    else {
        pixelSize = 5;
        wid = 128;
        hei = 64;
    }

    if (imgBuffer) delete imgBuffer;

    imgBuffer = new unsigned char[wid * hei];
    clear();
}

inline unsigned char Chip8::getX(unsigned short opCode) {
    return (unsigned char)((opCode & 0x0f00) >> 8);
}

inline unsigned char Chip8::getY(unsigned short opCode) {
    return (unsigned char)(((opCode & 0x00f0) >> 4) & 0xf);
}
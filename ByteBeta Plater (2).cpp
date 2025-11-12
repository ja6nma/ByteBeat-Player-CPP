#include <Windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <random>
#include <ctime>
#pragma comment(lib, "Winmm.lib")

typedef char (*ByteBeatFunc)(DWORD t);

char bytebeat1(DWORD t) { return static_cast<char>(t * (t & 1233 ? 1 : 0) * (3 + (5 & t >> (t & 2048 ? 12 : 9)))); } //code
char bytebeat2(DWORD t) { return static_cast<char>(t * (39 & t >> 69) * -1); } //code
char bytebeat3(DWORD t) { return static_cast<char>((t & t >> 8) * ((t >> 10 & 3) + 1) + (t >> 4 & t >> 9 & 31)); } //code
char bytebeat4(DWORD t) { return static_cast<char>(t >> t * 3); } //code
char bytebeat5(DWORD t) { return static_cast<char>(t >> 6 | t * 2 | t * (t >> 2) + t >> 6 | t * 2 | t >> (t >> 6)); } //code
char bytebeat6(DWORD t) { return static_cast<char>((t & t >> 8) * ((t >> 10 & 3) + 1) + (t >> 4 & t >> 9 & 31)); } //code
char bytebeat7(DWORD t) { return static_cast<char>((t * (1 + (5 & t >> 10))) | (t >> 3) + 2452); } //code
char bytebeat8(DWORD t) { return static_cast<char>((t >> 1 | t * -1) >> 5 << 1 >> 2 | (t * 4)); } //code
char bytebeat9(DWORD t) { return static_cast<char>((t & t >> 8) * ((t >> 10 & 3) + 1) + (t >> 4 & t >> 9 & 31)); } //code
char bytebeat10(DWORD t) { return static_cast<char>(2 * t ^ 2 * t + (t >> 8) & t >> 13 | t >> 4 - (1 ^ 8 & t >> 20)); } //code


ByteBeatFunc bytebeats[] = {
    bytebeat1,
    bytebeat2,
    bytebeat3,
    bytebeat4,
    bytebeat5,
    bytebeat6,
    bytebeat7,
    bytebeat8,
    bytebeat9,
    bytebeat10
};

const int NUM_BYTEBEATS = sizeof(bytebeats) / sizeof(bytebeats[0]);

int main()
{
    HWAVEOUT hwo = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, NUM_BYTEBEATS - 1);

    const DWORD BUFFER_SIZE = 8000 * 15;
    char* buffer = new char[BUFFER_SIZE];

    DWORD global_t = 0;

    while (true)
    {
        int selected = dis(gen);
        ByteBeatFunc currentBytebeat = bytebeats[selected];

        for (DWORD t = 0; t < BUFFER_SIZE; t++)
        {
            buffer[t] = currentBytebeat(global_t + t);
        }

        WAVEHDR hdr = { buffer, BUFFER_SIZE, 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hwo, &hdr, sizeof(WAVEHDR));
        waveOutWrite(hwo, &hdr, sizeof(WAVEHDR));

        Sleep(10000);

        waveOutUnprepareHeader(hwo, &hdr, sizeof(WAVEHDR));

        global_t += BUFFER_SIZE;
    }

    delete[] buffer;
    waveOutClose(hwo);
    return 0;
}
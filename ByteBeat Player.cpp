#include <Windows.h>
#include <mmsystem.h>
#include <stdio.h>
#pragma comment(lib, "Winmm.lib")

int main()
{
    HWAVEOUT hwo = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8,0 };
    waveOutOpen(&hwo, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    while (true)
    {
        char buffer[32000 * 8];
        for (DWORD t = 0; t < sizeof(buffer); t++)
            buffer[t] = static_cast<char>(t * (4 | t >> 13 & 3) >> (~t >> 11 & 1) & 128); //code

        WAVEHDR hdr = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hwo, &hdr, sizeof(WAVEHDR));
        waveOutWrite(hwo, &hdr, sizeof(WAVEHDR));
        waveOutUnprepareHeader(hwo, &hdr, sizeof(WAVEHDR));
    }

    waveOutClose(hwo);
}
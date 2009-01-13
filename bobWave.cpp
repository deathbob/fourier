#include "newWave.h"

using namespace std;

int main()
{
    //    wave mono1("planisphere_mono.wav");
    //    wave mono1("guitMono.wav");

    //    wave mono1("sine_440.wav");
    //    wave mono1("440_1033.wav");

    wave mono1("Feel_The_Sun_Rise.wav");
    mono1.make_complex();
    mono1.fourier(1024, 1);
    //    mono1.fourier(1024, -1);

}

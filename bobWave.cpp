#include "newWave.h"

using namespace std;

int main()
{
    
    // cur_size must always be a power of 2 or the FFT will fall over.  
    // 1024, 2048, 4096, 8192 16384 32768 65536
    int cur_size = 4096;


    wave mono1("Feel_The_Sun_Rise.wav");
    //    wave mono1("airwar.wav");
    mono1.build_hamming(cur_size);
    mono1.make_complex();
    mono1.fourier(cur_size, 1);
    //    mono1.fourier(cur_size, -1);

    wave mono2 = mono1;

    mono1.rebuild(mono2, cur_size);

    mono1.writeW("feel_to_feel_10.wav");

    return 0;

    // todo:  Make sure copy constructors / etc are working properly
    // figure out why some files have only half dynamic range.  
    /* all had full dynamic range, it was just a quiet file LOL */
    // write out fourierated files so they don't have to be run over and over.  
    /* do this next */
    // still falling over when metadata present.  Figure that out.  
}

#include "newWave.h"

using namespace std;

int main()
{
    
    // cur_size must always be a power of 2 or the FFT will fall over.  
    // 1024, 2048, 4096, 8192 16384 32768 65536
    //    int cur_size = 1024;
    //    int cur_size = 2048;
    //    int cur_size = 4096;
        int cur_size = 8192;
	// anything over 8192 is pretty much too long
    //    int cur_size = 16384;
    //    int cur_size = 32768;

    
    wave mono1("Feel_The_Sun_Rise.wav");
    wave mono3("Wasted.wav");
    mono1 += mono3;

    mono1.build_hamming(cur_size);
    mono1.make_complex();
    mono1.fourier(cur_size, 1, 32);
    //    mono1.fourier(cur_size, -1);
    
    wave mono2 = mono1;

    mono1.rebuild(mono2, cur_size);

    mono1.writeW("feel_to_feel_18.wav");

    return 0;

    // todo:  Make sure copy constructors / etc are working properly
    // figure out why some files have only half dynamic range.  
    /* all had full dynamic range, it was just a quiet file LOL */
    // write out fourierated files so they don't have to be run over and over.  
    /* do this next */
    // still falling over when metadata present.  Figure that out.  

    // run it with multiple values for cur_size and average the result?
    // get it markovating.  

    // build from full overlap but then blend overlaps 
}

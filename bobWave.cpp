#include "newWave.h"

using namespace std;

int main()
{
    // cur_size must always be a power of 2 or the FFT will fall over.  
    // 1024, 2048, 4096, 8192
    int cur_size = 4096;

    //    wave mono1("Wasted.wav");
    wave mono1("airwar.wav");
    cout<<"Wasted.wav loaded, complexing"<<endl;
    mono1.make_complex();
    cout<<"Complex finished"<<endl;
    mono1.fourier(cur_size, 1);
    cout<<"Fourier 1 finished"<<endl;
    //    mono1.fourier(cur_size, -1);


    //    wave mono2("Wasted.wav");
    wave mono2 = mono1;
    //    mono2.make_complex();
    mono2.fourier(cur_size, 1);

    mono1.rebuild(mono2, cur_size);

    mono1.writeW("wasted_to_wasted.wav");

    return 0;

    // todo:  Make sure copy constructors / etc are working properly
    // figure out why some files have only half dynamic range.  
    // write out fourierated files so they don't have to be run over and over.  
    // still falling over when metadata present.  Figure that out.  
}

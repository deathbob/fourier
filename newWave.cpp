// Bob Larrick
// rlarrick@cs.odu.edu

#include "newWave.h"
#include <algorithm>
#include <map>
#include <cassert>
#include <ctime>
#include <complex>

using namespace std;

typedef long long bobLong;

wave::wave(){
    char tempHeader[36] = {
	'R','I','F','F', // always RIFF
	0,0,0,0,	 // size of the entire file (determined later)
	'W','A','V','E', // always WAVE
	'f','m','t',' ', // always "fmt "
	16,0,0,0,        // 16 more bytes, not including these
	1,0,	         // 1 for PCM encoding
	1,0,	         // 1 for mono, 2 for stereo
	68, -84, 0, 0,   // per second, 68, -84,0,0 for 44100
	-120, 88, 1, 0,  // Bytes per Second -120, 88, 1 ,0 for 88200
                         // SampleRate * NumChannels * BitsPerSample/8
	2,0,	         // BlockAlign == NumChannels * BitsPerSample/8
	16,0	         // BitsPerSample, usually 8 or 16
    };
    for(int i = 0; i < 36; i++){
	header.push_back(tempHeader[i]);
    }
    char tempDataHead[8] = { 'd','a','t','a',0,0,0,0 };
    for(int i = 0; i < 8; i++){
	dataHead.push_back(tempDataHead[i]);
    }
    dataLength = *((unsigned int*)(&dataHead[4]));
    totalSize = *((unsigned int *)(&header[4]));
    fmtSize = *((unsigned int *)(&header[16]));
    compressionType = header[20] + header[21];
    channels = header[22] + header[23];
    sampleRate =  *((unsigned int *)(&header[24]));
    bytesPerSec = *((unsigned int *)(&header[28]));
    byteDepth = header[32] + header[33];
    bitDepth = header[34] + header[35];
    char tempInfoBlock[12] = {'i','n','f','o',8,0,0,0,'b','l','a','h'};
    for (int i = 0; i < 12; i++){
	infoBlock.push_back(tempInfoBlock[i]);
    }
};  // End of wave::wave(){};

wave::wave(string fileName){
    ifstream fileIn;
    fileIn.open(fileName.c_str(), ios::binary);
    //get the standard header
    for(int i = 0; i < 36; ++i){
	char temp = fileIn.get();
	header.push_back(temp);
    }
    totalSize = *((unsigned int*)(&header[4]));
    compressionType = header[20] + header[21];
    channels = header[22] + header[23];
    sampleRate = *((unsigned int *)(&header[24]));
    bytesPerSec = *((unsigned int*)(&header[28]));
    byteDepth = header[32] + header[33];
    bitDepth = header[34] + header[35];
    //find datablock, ignore all else
    bool dataBlockFound = false;
    char foo[8];
    fileIn.read(foo, 8);
    while(!dataBlockFound){
	//	dataHead.clear();
	if((foo[0] == 'd')&&
	   (foo[1] == 'a')&&
	   (foo[2] == 't')&&
	   (foo[3] == 'a')){
	    dataBlockFound = true;
	    for(int i = 0; i < 8; i++){
		dataHead.push_back(foo[i]);
	    }
	}
	else{
	    int skip_size = *((int*)(&foo[4]));
	    fileIn.ignore(skip_size);
	}
    }

    dataLength = *((unsigned int*)(&dataHead[4]));
    cout<<"dataLength "<<dataLength<<endl;
    for(unsigned int i = 0; i < dataLength; ++i){
	char temp = fileIn.get();
	buffer.push_back(temp);
    }

    //round up any other data
    char temp;
    while((temp = fileIn.get()) != char_traits<char>::eof()){
	infoBlock.push_back(temp);
    }
    fileIn.close();
}

void wave::make_int(){
    for(int i = 0; i < buffer.size(); i++){    
	short foo =  *((short*)(&buffer[i]));	    
	int_data.push_back(foo);
	i++;
	//	cout<<foo<<endl;
    }
}

void wave::make_complex(){
    cout<<"buffer size "<<buffer.size()<<endl;
    //    complex_data.resize(buffer.size()/2);
    complex_data.clear();
    int i = 0;
    for(i ; i < buffer.size(); i++){
	short foo =  *((short*)(&buffer[i]));
	//	cout<<foo<<endl;
	complex<double> temp(foo,0);
	complex_data.push_back(temp);
	i++;
    }
    cout<<" i "<<i<<endl;
    cout<<"complex_data.size() after make_complex "<<complex_data.size()<<endl;
    //    for(int i = 0; i < complex_data.size(); i++){
    //	cout<<"complex_data["<<i<<"] raw "<<complex_data[i]<<endl;
    //    }
}
// window width is size of samples to look at
// direction specifies if this is the inverse or regular FFT
void wave::fourier(int window_width, int direction){
    complex<double> e(2.71828183,0);
    int N = window_width;
    int d = direction;
    float PI = 3.14159265358979323846;
    float delta_w = -2.0 * PI * d/ float(N);

    // key = frequency, value = offset in originial file of beginning of window
    map<int, vector<int> > freq_bin;
    
    int repetitions = complex_data.size() / N;
    int left_over = complex_data.size() % N;
    cout<<"left over samples "<<left_over<<endl;
    int this_rep = repetitions;
    cout<<"repetitions "<<repetitions<<endl;
    int place_holder = 0;
    for(repetitions; repetitions > 0; repetitions--){
	int r = N / 2;
	vector< complex<double > > window;
	for(int i = place_holder; i < place_holder + N; i++){
	    window.push_back(complex_data[i]);
	}


	for(int i = 1; i < N; ){
	    complex<double> myi(0,1);
	    complex<double> cos_w(cos(i * delta_w),0);
	    complex<double> sin_w(sin(i * delta_w),0);
	    complex<double> w = cos_w + (myi * sin_w);
	    for(int k = 0; k < N; ){
		complex<double> u(1,0);
		for(int m = 0; m < r; m++){
		    complex<double> t = window[k+m] - window[k+m+r];
		    window[k+m] = window[k+m] + window[k+m+r];
		    complex<double> temp = t * u;
		    window[k+m+r] = temp;
		    u = w * u;
		}// end m
		k = k + 2.0 * r;
	    }//end k 
	    i = 2 * i;
	    r = r / 2;
	}// end i
	for(int i = 0; i < N; i++){
	    r = i;
	    int k = 0;
	    for(int m = 1; m < N; ){
		k = 2 * k + (r%2);
		r = r/2;
		m = 2 * m;
	    }// end k
	    if(k > i){
		complex<double> t = window[i];
		window[i] = window[k];
		window[k] = t;
	    }
	}// end i
	if(d < 0){
	    complex<double> complex_N(N, 0);
	    for(int i = 0; i < N; i++){
		window[i] = window[i] / complex_N;
	    }
	}
	int freq_1 = 0; 
	//	int freq_2 = 0;
	//	int freq_3 = 0;
	int max_1 = 0;
	//	int max_2 = 0;
	//	int max_3 = 0;
	for(int i = 0; i < N/2; i++){
	    // uncomment following line to display magnitude of each frequency
	    //	    cout<<"freq "<<44100 * i / N<<" = "<<abs(window[i])<<endl;
	    if(abs(window[i]) > max_1){
		//	      max_3 = max_2;
		//	      max_2 = max_1;
		//	      freq_3 = freq_2;
		//	      freq_2 = freq_1;
		max_1 = abs(window[i]);
		freq_1 = 44100 * i / N;
	    }
	}
	
	cout<<"Frequency "<<freq_1<<" with magnitude "<<max_1
	    <<" is the largest component of window "<<this_rep - repetitions<<endl;
	cout<<"        end of window "<<this_rep - repetitions<<endl;
	// puts the beginning offset of the current window in the bin labelled with the largest frequency magnitude
	freq_bin[freq_1].push_back(place_holder);
	// if something is fucked up, this is it.  move back to right under where the window is filled.  
	place_holder += N;	
    }
    // end repetitions
    // uncomment these next lines if you are running the inverse
    // FFT to see that it does transform the data back to its original state.  
    /*    if(-1 == d){
	for(int i = 0; i < complex_data.size(); i++){
	    cout<<complex_data[i]<<endl;
	}
	}*/
}


void wave::setDataLength(float length){
    dataLength = (int)(length * bytesPerSec);
    *((unsigned int*)(&header[4])) = dataLength + 36 + 12;
    *((unsigned int*)(&dataHead[4])) = dataLength;
};

// .wav must have correct length to play.  
// This checks the size of everything that will be written out 
// and sets .wav length accordingly.
void wave::setDataLength(){
    if(dataLength != buffer.size()){
	cout<<"dataLength was wrong!"<<endl;
	cout<<"expected "<<dataLength<< " got "<<buffer.size()<<endl;
	dataLength = buffer.size(); 
	*((unsigned int*)(&header[4])) = dataLength + 24 + 8 + infoBlock.size();
	*((unsigned int*)(&dataHead[4])) = dataLength + infoBlock.size();
    }
};
	  
void wave::writeW(string fileName){
    if(dataLength != buffer.size()){
	setDataLength();
    }
    ofstream fileOut;
    fileOut.open(fileName.c_str(), ios::out | ios::binary);
    copy(header.begin(), header.end(), ostream_iterator<unsigned char>(fileOut));
    copy(dataHead.begin(), dataHead.end(), ostream_iterator<unsigned char>(fileOut));
    copy(buffer.begin(), buffer.end(), ostream_iterator<unsigned char>(fileOut));
    copy(infoBlock.begin(),infoBlock.end(), ostream_iterator<unsigned char>(fileOut));
    fileOut.close();
};

// for debugging.  
void wave::print(){
    cout<<"dataLength "<<dataLength<<endl;
    cout<<"buffer.size() "<<buffer.size()<<endl;

    cout<<"contents of header \n"<<left;
    for(unsigned int i = 0; i < header.size(); i++)    {
	cout<<setw(7)<<header[i]<<setw(7)<<(int)header[i]<<endl;
    }
    cout<<endl<<"contents of dataHead \n";
    for(unsigned int i = 0; i < dataHead.size(); i++)    {
	cout<<setw(7)<<dataHead[i]<<setw(7)<<(int)dataHead[i]<<endl;
    }
    cout<<endl<<"contents of infoBlock \n";
    for (unsigned int i = 0; i < infoBlock.size(); i++)    {
	cout<<setw(7)<<infoBlock[i]<<setw(7)<<(int)infoBlock[i]<<endl;
    }
}

void wave::operator+=(const wave& right){
    bool chanEq = channels == right.channels;
    bool bytePS = bytesPerSec == right.bytesPerSec;

    if (chanEq && bytePS){
	unsigned long rightSize = right.buffer.size();
	for(unsigned long i = 0; i < rightSize; i++){
	    buffer.push_back(right.buffer[i]);
	}
    }
    else{cout<<"oops"<<endl;}
    setDataLength();
}

wave wave::operator=(const wave& right){
    wave retWave;
    retWave.header = right.header;
    retWave.dataHead = right.dataHead;
    retWave.infoBlock = right.infoBlock;
    retWave.buffer = right.buffer;
    retWave.markov = right.markov;
    
    retWave.dataLength = right.dataLength;
    retWave.totalSize = right.totalSize;
    retWave.fmtSize = right.fmtSize;
    retWave.compressionType = right.compressionType;
    retWave.channels = right.channels;
    retWave.sampleRate = right.sampleRate;
    retWave.bytesPerSec = right.bytesPerSec;
    retWave.byteDepth = right.byteDepth;
    retWave.bitDepth = right.bitDepth;
    return retWave;
}


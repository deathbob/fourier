// Bob Larrick
// rlarrick@cs.odu.edu

#ifndef BOBWAVE_H
#define BOBWAVE_H

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cmath>
#include <map>
//#include "miniwave.h"
//#include "bobGraph.h"
#include <complex>


using namespace std;

class wave
{
 public:
  wave();
  wave(string fileName);
  void setDataLength(float length);
  void setDataLength();
  void writeW(string fileName);
  void print();
  void markovAte();
  void scramble();
  void operator+=(const wave& right);
  wave operator=(const wave& right);
  void make_int();
  void make_complex();
  void fourier(int window_width, int direction);
  void rebuild(wave to_build, int window_size);
  vector<int> this_order;

 private:
  vector< complex<double> > complex_data;
  vector<int> int_data;
  vector<char> header;
  vector<char> dataHead;
  vector<char> infoBlock;
  vector<unsigned char> buffer;
  map<long long, vector<short> > markov;
  // freq_bin: key = frequency, value = offset in originial file of beginning of window
  map<int, vector<int> > freq_bin;


  unsigned long int dataLength;
  unsigned long int totalSize ;
  unsigned int fmtSize;
  unsigned int compressionType;
  unsigned int channels;
  unsigned int sampleRate ;
  unsigned int bytesPerSec;
  unsigned int byteDepth;
  unsigned int bitDepth;
};

#endif

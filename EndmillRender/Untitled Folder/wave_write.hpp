#include <stdio.h>
#include <stdlib.h>
#include <math.h>

template< class FuncLeft, class FuncRight>
class WaveWrite
{

  FILE * m_fileWriteTo;

  double m_lfDuration;
  double m_lfSecondsPerSample;

  int m_nNumSamples;
  int m_nBitsPerSample;
  int m_nBytesPerSample;
  int m_nBytesPerSecond;
  int m_nSamplesPerSecond;

  
  void writeInt(unsigned int nWriteMe) {
    fputc(nWriteMe & 0xFF, m_fileWriteTo);
    nWriteMe >>= 8;
    fputc(nWriteMe & 0xFF, m_fileWriteTo);
    nWriteMe >>= 8;
    fputc(nWriteMe & 0xFF, m_fileWriteTo);
    nWriteMe >>= 8;
    fputc(nWriteMe & 0xFF, m_fileWriteTo);
  }

  void writeShort(unsigned short nWriteMe) { // writes a 16-bit int
    fputc(nWriteMe & 0xFF, m_fileWriteTo);
    nWriteMe >>= 8;
    fputc(nWriteMe & 0xFF, m_fileWriteTo);
  }

  void writeChar(unsigned char cWriteMe) {
    fputc(cWriteMe, m_fileWriteTo);
  }

  void writeRiff() {
    writeChar('R'); writeChar('I');
    writeChar('F'); writeChar('F');

    int nTotalBytes = 4+8+24+m_nNumSamples*m_nBytesPerSample;

    writeInt(nTotalBytes);

    writeChar('W'); writeChar('A');
    writeChar('V'); writeChar('E');
    
  }

  void writeFormat() {
    writeChar('f'); writeChar('m');
    writeChar('t'); writeChar(' ');

    writeInt(0x10);

    writeShort(0x01);

    writeShort(0x02);

    writeInt(m_nSamplesPerSecond);
    writeInt(m_nBytesPerSecond);
    writeShort(m_nBytesPerSample);
    writeShort(m_nBitsPerSample);
  }

  unsigned short doubleFloatToShort(double lfX) {
    return (unsigned short)(32000*lfX);
  }

  void writeData() {

    writeChar('d'); writeChar('a');
    writeChar('t'); writeChar('a');
  
    int nLenDataToFollow = m_nNumSamples*m_nBytesPerSample;
    writeInt(nLenDataToFollow);

    double lfCurrTime = 0.0;
    for(int i = 0; i < m_nNumSamples; ++i) {
      
      double lfCurrLeft = FuncLeft::evalF(lfCurrTime);
      double lfCurrRight = FuncRight::evalF(lfCurrTime);

      unsigned short nWriteMe = doubleFloatToShort(lfCurrLeft);
      writeShort(nWriteMe);
      nWriteMe = doubleFloatToShort(lfCurrRight);
      writeShort(nWriteMe);

      lfCurrTime += m_lfSecondsPerSample;

    }

  }

public:

  WaveWrite(int nSampleRate,
	    double lfDuration) {
   

    m_lfDuration = lfDuration;
    
    m_nSamplesPerSecond = nSampleRate;

    m_nNumSamples = static_cast<int>(nSampleRate*lfDuration+1);
    
    m_nBytesPerSample = 4; // always do 16-bit stereo

    m_nBitsPerSample = 16;

    m_nBytesPerSecond = m_nBytesPerSample*m_nSamplesPerSecond;

    m_lfSecondsPerSample = 1.0/m_nSamplesPerSecond;
  }


  void doWrite(const char * szFileName) {
    m_fileWriteTo = fopen(szFileName, "wb");

    writeRiff(); 
    writeFormat();
    writeData();
    fclose(m_fileWriteTo);
    m_fileWriteTo = NULL;

  }

  
};


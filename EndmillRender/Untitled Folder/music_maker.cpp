#include "wave_write.hpp"
#include "function_templates.hpp"

#define BPM 100

typedef WaveWrite<DoAmp< GenModSin<165, 3, 20> >, 
		  DoAmp< GenModSin<110, 1, 10> > > SIMPLE_TYPE;

typedef WaveWrite<DoAmp< SumSignals< GenModSin<165, 3, 20> , 
				     GenModSin<110, 1, 10>, 2 > >,
		  DoAmp<SumSignals< GenModSin<110, 3, 20> , 
				     GenModSin<165, 1, 10>, 2 > > > FANCY_TYPE;

typedef WaveWrite<DoAmp< SumSignals< GenModSin<130, 3, 20> , 
				     GenModSin<110, 1, 10>, 2 > >,
		  DoAmp<SumSignals< GenModSin<110, 3, 20> , 
				     GenModSin<130, 1, 10>, 2 > > > SPOOKY_TYPE;

typedef SumSignals< 
  SumSignals<DelaySignal<Gaussian< GenSin<110>,60,BPM>, 
			 0*60,BPM>,
	     DelaySignal<Gaussian<GenSin<165>,60,BPM>,
			 1*60*60,BPM>
	     ,1>,
  SumSignals<DelaySignal<Gaussian< GenSin<220>,60,BPM>, 
			 2*60,BPM>,
	     DelaySignal<Gaussian<GenSin<208>,60,BPM>,
			 3*60,BPM>
	     ,1>,
  1> PART_PHRASE;

typedef SumSignals< 
  SumSignals< SumSignals<DelaySignal<Gaussian< GenSin<110>,60,2*BPM>, 
				     0*60,BPM>,
			 DelaySignal<Gaussian<GenSin<165>,60,2*BPM>,
				     1*60,BPM>
			 ,1>,
	      SumSignals<DelaySignal<Gaussian< GenSin<220>,60,2*BPM>, 
				     2*60,BPM>,
			 DelaySignal<Gaussian<GenSin<196>,60,2*BPM>,
				     3*60,BPM>
			 ,1>,
	      1> ,
  SumSignals<DelaySignal<Gaussian< GenSin<147>,60,2*BPM>, 
			 4*60,BPM>,
	     DelaySignal<Gaussian<GenSin<165>,60,2*BPM>,
			 5*60,BPM>
	     ,1>,
  2> PHRASE1;

typedef DelaySignal<
  SumSignals< 
  SumSignals< SumSignals<DelaySignal<Gaussian< GenSin<110>,60,2*BPM>, 
				     0*60,BPM>,
			 DelaySignal<Gaussian<GenSin<165>,60,2*BPM>,
				     1*60,BPM>
			 ,1>,
	      SumSignals<DelaySignal<Gaussian< GenSin<220>,60,2*BPM>, 
				     2*60,BPM>,
			 DelaySignal<Gaussian<GenSin<247>,60,2*BPM>,
				     3*60,BPM>
			 ,1>,
	      1> ,
  SumSignals<DelaySignal<Gaussian< GenSin<196>,60,2*BPM>, 
			 4*60,BPM>,
	     DelaySignal<Gaussian<GenSin<220>,2*60,2*BPM>,
			 5*60,BPM>
	     ,1>,
  2>,6*60,BPM> PHRASE2;


typedef DelaySignal<
  SumSignals< SumSignals<DelaySignal<Gaussian< GenSin<130>,60,2*BPM>, 
				     0*60,BPM>,
			 DelaySignal<Gaussian<GenSin<146>,60,2*BPM>,
				     1*60,BPM>
			 ,1>,
	      SumSignals<DelaySignal<Gaussian< GenSin<165>,60,2*BPM>, 
				     2*60,BPM>,
			 DelaySignal<Gaussian<GenSin<110>,60,2*BPM>,
				     3*60,BPM>
			 ,1>,
  2>,18*60,BPM> PHRASE3;

typedef SumSignals<PHRASE1, 
		   SumSignals<PHRASE2,
			      SumSignals<DelaySignal<PHRASE1,12*60,BPM>,
					 PHRASE3,
					 1>
			      ,1>, 
		   1> LEFT_PHRASE;

typedef DelaySignal<LEFT_PHRASE,1,100> RIGHT_PHRASE;


typedef WaveWrite<DoAmpEven<LEFT_PHRASE>, 
		  DoAmpEven<RIGHT_PHRASE> > EVEN_TYPE;

typedef WaveWrite<DoAmp<LEFT_PHRASE>, 
		  DoAmp<RIGHT_PHRASE> > POWER_TYPE;

typedef WaveWrite<DoAmpGentle<LEFT_PHRASE>, 
		  DoAmpGentle<RIGHT_PHRASE> > GENTLE_TYPE;

typedef WaveWrite<DoAmpFifths<LEFT_PHRASE>, 
		  DoAmpFifths<RIGHT_PHRASE> > FIFTHS_TYPE;

int main(int argc, char **argv) {
  EVEN_TYPE * myWriter = 
    new EVEN_TYPE(44100, 24.0*60.0/BPM);

  POWER_TYPE * powWriter = 
    new POWER_TYPE(44100, 24.0*60.0/BPM);

  GENTLE_TYPE * gentWriter = 
    new GENTLE_TYPE(44100, 24.0*60.0/BPM);

  FIFTHS_TYPE * fifthsWriter = 
    new FIFTHS_TYPE(44100, 24.0*60.0/BPM);

  myWriter->doWrite("even_melody.wav");

  powWriter->doWrite("power_melody.wav");

  gentWriter->doWrite("gentle_melody.wav");

  fifthsWriter->doWrite("fifths_melody.wav");

}


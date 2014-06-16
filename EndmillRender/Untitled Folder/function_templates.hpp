#include<math.h>

template <int nFreq> class GenSin {
public:
  static double evalF(double lfTime) {
    return sin(2*3.141592654*nFreq*lfTime);
  }
};

template <int nFreq, int nModFreqDenom, int nModFreqNum> class GenModSin {
public:
  static double evalF(double lfTime) {
    double lfTmp =
      sin(2*3.141592654*(nModFreqNum*1.0/nModFreqDenom)*lfTime) ;
    return sin(2*3.141592654*nFreq*lfTime)*
      lfTmp*lfTmp;
  }
};

template < class SignalToMod> class DoAmp {
public:
  static double evalF(double lfTime) {
    double x = SignalToMod::evalF(lfTime);
    return  0.5*x*(-1+x*(-3+x*(2+x*4)));
  }
};

template < class SignalToMod> class DoAmpEven {
public:
  static double evalF(double lfTime) {
    double x = SignalToMod::evalF(lfTime);
    return  (1+x*(1+x*x*(8+x*x*(-40.0+x*x*32.0))))/5.0;
  }
};

template < class SignalToMod> class DoAmpGentle {
public:
  static double evalF(double lfTime) {
    double x = SignalToMod::evalF(lfTime);
    return  (x*(3-x*x))*0.5;
  }
};

template < class SignalToMod> class DoAmpFifths {
public:
  static double evalF(double lfTime) {
    double x = SignalToMod::evalF(lfTime);
    return  -0.5+x*x*(2.25+x*(1+x*(-1.5+x*x )) );
  }
};

template <class SignalToMod, int DelayNum, int DelayDenom> class DelaySignal {
public:
  static double evalF(double lfTime) {
    return SignalToMod::evalF(lfTime- DelayNum/(1.0*DelayDenom));
  }
};

template <class Sig1, class Sig2, int Denom> class SumSignals {
public:
  static double evalF(double lfTime) {
    return (Sig1::evalF(lfTime) + Sig2::evalF(lfTime))/(1.0*Denom);
  }
};

template <class Sig1, class Sig2> class MultSignals {
public:
  static double evalF(double lfTime) {
    return (Sig1::evalF(lfTime) * Sig2::evalF(lfTime));
  }
};

template <class Sig, int FREQ, int NUM, int DENOM> class Vibrato {
public:
  static double evalF(double lfTime) {
    return Sig::evalF(lfTime + (NUM * GenSin<FREQ>::evalF(lfTime))/(1.0*DENOM));
  }
};

template <class Sig1, int NumWidth, int DenomWidth> class Gaussian {
public:
  static double evalF(double lfTime) {
    return Sig1::evalF(lfTime) * 
      exp(-(lfTime*lfTime*DenomWidth
	    *DenomWidth) / (NumWidth*NumWidth));
  }
};



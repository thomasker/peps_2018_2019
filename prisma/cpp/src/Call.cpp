#include "Call.h"
#include <math.h>

double Call::GetPrice(PnlMat* paths, double r, int t) {
  return GetPayOff(paths) * exp(-r * (_T - t));
}

double Call::GetPayOff(PnlMat* paths) {
  double St = MGET(paths, 0, paths->n - 1);
  return St < _K ? 0 : St - _K;
}

PnlVectInt* Call::GetDates() { return dates; }

int Call::GetNbForeignCurrency() { return 42; }

Call::Call(double K, int T) {
  _K = K;
  _T = T;
}

Call::~Call() {}
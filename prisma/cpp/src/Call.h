#ifndef CALL_H
#define	CALL_H

#include "pnl/pnl_matrix.h"

class Call {
 public:
  double GetPrice(PnlMat* paths, double r, int t);
  double GetPayOff(PnlMat* paths);
  PnlVectInt* GetDates();
  int GetNbForeignCurrency();
  Call(double K, int T);
  ~Call();

 private:
  double _K;
  int _T;
  PnlVectInt* dates;
};

#endif
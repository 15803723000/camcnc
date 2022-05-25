#pragma once


#include "Preprocess.h"

namespace GCode {
class ShowSharpCorner : public Preprocess {

 public:
  ShowSharpCorner() = delete;
  ShowSharpCorner(cb::SmartPointer<GCode::ToolPath>& toolPath) : Preprocess(toolPath) {}

  void computeSharpCorner(float angle = 46.0);
  void setCornerType();
  void deleteCornerType();

 private:
  std::vector<unsigned> cornerlist;
};
}


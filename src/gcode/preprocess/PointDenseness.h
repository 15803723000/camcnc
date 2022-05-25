#include "Preprocess.h"

namespace GCode {
class PointDenseness : public Preprocess {

public:
  PointDenseness() = delete;
  PointDenseness(cb::SmartPointer<GCode::ToolPath>& toolPath) : Preprocess(toolPath) {homogeniz();}

  void homogeniz();

};

}  // namespace GCode
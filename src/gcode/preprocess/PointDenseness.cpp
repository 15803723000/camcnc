#include "PointDenseness.h"

namespace GCode {

void PointDenseness::homogeniz() {
  std::vector<float> lens;
  for (int i = 0; i <= coordinate.size() - 1; i += 2) {
    float squ = 0.0;
    cb::Vector3F coord = coordinate[i + 1] - coordinate[i];
  }
  

}


}  // namespace GCode
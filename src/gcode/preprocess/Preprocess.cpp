#include "Preprocess.h"

namespace GCode {

Preprocess::Preprocess(cb::SmartPointer<GCode::ToolPath>& toolPath) : path(toolPath) {
  initCoord();
}

void Preprocess::initCoord() {
  if (!path.isNull()) {
    cb::Vector3F lastLine;
    for (auto it = path->begin(); it != path->end(); it++) {
      GCode::Move move = *it;
      const cb::Vector3F& start = move.getStartPt();
      const cb::Vector3F& end = move.getEndPt();

      coordinate.push_back(start);
      coordinate.push_back(end);
    }
  }
}


}  // namespace GCode

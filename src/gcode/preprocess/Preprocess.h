#pragma once


#include <cbang/SmartPointer.h>
#include <cbang/geom/Vector.h>
#include <gcode/ToolPath.h>

#include <vector>


namespace GCode {

class Preprocess {

public:
  Preprocess() = delete;
  Preprocess(cb::SmartPointer<GCode::ToolPath>& toolPath);

private:
  void initCoord();



protected:
  cb::SmartPointer<GCode::ToolPath> path;
  // get all coordinate from GCode
  std::vector<cb::Vector3F> coordinate;

};

}  // namespace GCode
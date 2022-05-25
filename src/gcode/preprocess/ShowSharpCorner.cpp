#include "ShowSharpCorner.h"

namespace GCode {


void ShowSharpCorner::computeSharpCorner(float angle) {
  for (int i = 0; i <= coordinate.size() - 4; i += 2) {
    cb::Vector3F lineA = coordinate[i + 1] - coordinate[i];
    cb::Vector3F lineB = coordinate[i + 2] - coordinate[i + 3];

    float angleOfRadian = lineA.angleBetween(lineB);
    float angleOfDegree = angleOfRadian * 180 / 3.14159265;

    if (angleOfDegree < angle) {
      cornerlist.push_back(i / 2);
      cornerlist.push_back(i / 2 + 1);
    }
  }
}

void ShowSharpCorner::setCornerType() {
  unsigned countPath = 0, countList = 0;
  if (cornerlist.size() > 0) {
    for (auto it = path->begin(); it != path->end(); it++) {
      if (countPath == cornerlist[countList]) {
        GCode::Move move = *it;
        if ((*it).getType() != GCode::MoveType::MOVE_RAPID) {
          (*it).setType(GCode::MoveType::MOVE_CORNER);
        }
        ++countList;
      }
      ++countPath;
    }
  }
}

void ShowSharpCorner::deleteCornerType() {
  unsigned countPath = 0, countList = 0;
  std::cout << "deleteCornerType" << std::endl;
  if (cornerlist.size() > 0) {
    for (auto it = path->begin(); it != path->end(); it++) {
      if (countPath == cornerlist[countList]) {
        if ((*it).getType() == GCode::MoveType::MOVE_CORNER) {
          (*it).setType(GCode::MoveType::MOVE_CUTTING);
        }

        ++countList;
      }
      ++countPath;
    }
  }
}



}
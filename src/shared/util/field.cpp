//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
 \file    field.cpp
 \brief   Field markings management
 \author  Joydeep Biswas, (C) 2013
 */
//========================================================================

#include "field.h"
#include "field_default_constants.h"

FieldLine* FieldLine::FromVarList(VarList* list) {
  vector<VarType*> list_entries = list->getChildren();
  if (list_entries.size() != static_cast<size_t>(7)) {
    // The list should have exactly the expected number of entries.
    return NULL;
  }
  VarString* name = NULL;
  VarStringEnum* type = NULL;
  VarDouble* p1_x = NULL;
  VarDouble* p1_y = NULL;
  VarDouble* p2_x = NULL;
  VarDouble* p2_y = NULL;
  VarDouble* thickness = NULL;
  for (size_t i = 0; i < list_entries.size(); ++i) {
    VarType* entry = list_entries[i];
    if (entry->getName().compare("Name") == 0) {
      name = reinterpret_cast<VarString*>(entry);
    } else if (entry->getName().compare("Type") == 0) {
      type = reinterpret_cast<VarStringEnum*>(entry);
    } else if (entry->getName().compare("P1.x") == 0) {
      p1_x = reinterpret_cast<VarDouble*>(entry);
    } else if (entry->getName().compare("P1.y") == 0) {
      p1_y = reinterpret_cast<VarDouble*>(entry);
    } else if (entry->getName().compare("P2.x") == 0) {
      p2_x = reinterpret_cast<VarDouble*>(entry);
    } else if (entry->getName().compare("P2.y") == 0) {
      p2_y = reinterpret_cast<VarDouble*>(entry);
    } else if (entry->getName().compare("Line thickness") == 0) {
      thickness = reinterpret_cast<VarDouble*>(entry);
    } else {
      // Unexpected entry encountered: fail.
      return NULL;
    }
  }
  return (new FieldLine(name, type, p1_x, p1_y, p2_x, p2_y, thickness, list));
}

FieldLine::FieldLine(
    VarString* name_,
    VarStringEnum* type_,
    VarDouble* p1_x_,
    VarDouble* p1_y_,
    VarDouble* p2_x_,
    VarDouble* p2_y_,
    VarDouble* thickness_,
    VarList* list_): QObject(),
    name(name_), type(type_), p1_x(p1_x_), p1_y(p1_y_), p2_x(p2_x_), p2_y(p2_y_),
    thickness(thickness_), list(list_) {
  connect(name, SIGNAL(wasEdited(VarType*)),
          this, SLOT(Rename()));
}

FieldLine::FieldLine(const FieldLine& other) :
    name(new VarString("Name", other.name->getString())),
    type(new VarStringEnum("Type", other.type->getName())),
    p1_x(new VarDouble("P1.x", other.p1_x->getDouble())),
    p1_y(new VarDouble("P1.y", other.p1_y->getDouble())),
    p2_x(new VarDouble("P2.x", other.p2_x->getDouble())),
    p2_y(new VarDouble("P2.y", other.p2_y->getDouble())),
    thickness(new VarDouble("Line thickness", other.thickness->getDouble())),
    list(new VarList(other.name->getString())) {
  list->addChild(name);
  list->addChild(type);
  list->addChild(p1_x);
  list->addChild(p1_y);
  list->addChild(p2_x);
  list->addChild(p2_y);
  list->addChild(thickness);
  fillTypeEnum();
  connect(name, SIGNAL(wasEdited(VarType*)), this, SLOT(Rename()));
}

void FieldLine::fillTypeEnum() const {
    type->addItem("Undefined");
    type->addItem("TopTouchLine");
    type->addItem("BottomTouchLine");
    type->addItem("LeftGoalLine");
    type->addItem("RightGoalLine");
    type->addItem("HalfwayLine");
    type->addItem("CenterLine");
    type->addItem("LeftPenaltyStretch");
    type->addItem("RightPenaltyStretch");
    type->addItem("LeftFieldLeftPenaltyStretch");
    type->addItem("LeftFieldRightPenaltyStretch");
    type->addItem("RightFieldLeftPenaltyStretch");
    type->addItem("RightFieldRightPenaltyStretch");
}

FieldLine::FieldLine(const string& marking_name) :
    name(new VarString("Name", marking_name)),
    type(new VarStringEnum("Type", "Undefined")),
    p1_x(new VarDouble("P1.x")),
    p1_y(new VarDouble("P1.y")),
    p2_x(new VarDouble("P2.x")),
    p2_y(new VarDouble("P2.y")),
    thickness(new VarDouble("Line thickness", 10)),
    list(new VarList(marking_name)) {
  list->addChild(name);
  list->addChild(type);
  list->addChild(p1_x);
  list->addChild(p1_y);
  list->addChild(p2_x);
  list->addChild(p2_y);
  list->addChild(thickness);
  fillTypeEnum();
  connect(name, SIGNAL(wasEdited(VarType*)), this, SLOT(Rename()));
}

FieldLine::FieldLine(
    const string& marking_name,
    double p1_x_,
    double p1_y_,
    double p2_x_,
    double p2_y_,
    double thickness_) :
    name(new VarString("Name", marking_name)),
    type(new VarStringEnum("Type", marking_name)),
    p1_x(new VarDouble("P1.x", p1_x_)),
    p1_y(new VarDouble("P1.y", p1_y_)),
    p2_x(new VarDouble("P2.x", p2_x_)),
    p2_y(new VarDouble("P2.y", p2_y_)),
    thickness(new VarDouble("Line thickness", thickness_)),
    list(new VarList(marking_name)) {
  list->addChild(name);
  list->addChild(type);
  list->addChild(p1_x);
  list->addChild(p1_y);
  list->addChild(p2_x);
  list->addChild(p2_y);
  list->addChild(thickness);
  fillTypeEnum();
  connect(name, SIGNAL(wasEdited(VarType*)), this, SLOT(Rename()));
}

void FieldLine::Rename() {
  list->setName(name->getString());
}

FieldLine::~FieldLine() {
  list->removeChild(thickness);
  list->removeChild(p2_x);
  list->removeChild(p2_y);
  list->removeChild(p1_x);
  list->removeChild(p1_y);
  list->removeChild(name);
  list->removeChild(type);
  delete name;
  delete type;
  delete p1_x;
  delete p1_y;
  delete p2_x;
  delete p2_y;
  delete thickness;
  delete list;
}

FieldCircularArc* FieldCircularArc::FromVarList(VarList* list) {
  vector<VarType*> list_entries = list->getChildren();
  if (list_entries.size() != static_cast<size_t>(8)) {
      // The list should have exactly the expected number of entries.
    return NULL;
  }
  VarString* name = NULL;
  VarStringEnum* type = NULL;
  VarDouble* center_x = NULL;
  VarDouble* center_y = NULL;
  VarDouble* radius = NULL;
  VarDouble* a1 = NULL;
  VarDouble* a2 = NULL;
  VarDouble* thickness = NULL;
  for (size_t i = 0; i < list_entries.size(); ++i) {
    VarType* entry = list_entries[i];
    if (entry->getName().compare("Name") == 0) {
      name = reinterpret_cast<VarString*>(entry);
    } else if (entry->getName().compare("Type") == 0) {
      type = reinterpret_cast<VarStringEnum*>(entry);
    } else if (entry->getName().compare("Center.x") == 0) {
      center_x = reinterpret_cast<VarDouble*>(entry);
    } else if (entry->getName().compare("Center.y") == 0) {
      center_y = reinterpret_cast<VarDouble*>(entry);
    } else if (entry->getName().compare("Radius") == 0) {
      radius = reinterpret_cast<VarDouble*>(entry);
    } else if (entry->getName().compare("Start angle") == 0) {
      a1 = reinterpret_cast<VarDouble*>(entry);
    } else if (entry->getName().compare("End angle") == 0) {
      a2 = reinterpret_cast<VarDouble*>(entry);
    } else if (entry->getName().compare("Line thickness") == 0) {
      thickness = reinterpret_cast<VarDouble*>(entry);
    } else {
      // Unexpected entry encountered: fail.
      return NULL;
    }
  }
  FieldCircularArc* new_arc = new FieldCircularArc(
      name, type, center_x, center_y, radius, a1, a2, thickness, list);
  return (new_arc);
}

FieldCircularArc::FieldCircularArc(
    VarString* name_, VarStringEnum* type_, VarDouble* center_x_, VarDouble* center_y_,
    VarDouble* radius_, VarDouble* a1_, VarDouble* a2_, VarDouble* thickness_,
    VarList* list_): QObject(),
    name(name_), type(type_), center_x(center_x_), center_y(center_y_), radius(radius_),
    a1(a1_), a2(a2_), thickness(thickness_), list(list_) {
  fillTypeEnum();
  connect(name, SIGNAL(wasEdited(VarType*)), this, SLOT(Rename()));
}

FieldCircularArc::FieldCircularArc(const FieldCircularArc& other) :
    name(new VarString("Name", other.name->getString())),
    type(new VarStringEnum("Type", other.type->getString())),
    center_x(new VarDouble("Center.x", other.center_x->getDouble())),
    center_y(new VarDouble("Center.y", other.center_y->getDouble())),
    radius(new VarDouble("Radius", other.radius->getDouble())),
    a1(new VarDouble("Start angle", other.a1->getDouble())),
    a2(new VarDouble("End angle", other.a2->getDouble())),
    thickness(new VarDouble("Line thickness", other.thickness->getDouble())),
    list(new VarList(other.name->getString())) {
  list->addChild(name);
  list->addChild(type);
  list->addChild(center_x);
  list->addChild(center_y);
  list->addChild(radius);
  list->addChild(a1);
  list->addChild(a2);
  list->addChild(thickness);
  fillTypeEnum();
  connect(name, SIGNAL(wasEdited(VarType*)), this, SLOT(Rename()));
}

FieldCircularArc::FieldCircularArc(
    const string& marking_name,
    double center_x_, double center_y_,
    double radius_, double a1_, double a2_, double thickness_) :
    name(new VarString("Name", marking_name)),
    type(new VarStringEnum("Type", marking_name)),
    center_x(new VarDouble("Center.x", center_x_)),
    center_y(new VarDouble("Center.y", center_y_)),
    radius(new VarDouble("Radius", radius_)),
    a1(new VarDouble("Start angle", a1_)),
    a2(new VarDouble("End angle", a2_)),
    thickness(new VarDouble("Line thickness", thickness_)),
    list(new VarList(marking_name)) {
  list->addChild(name);
  list->addChild(type);
  list->addChild(center_x);
  list->addChild(center_y);
  list->addChild(radius);
  list->addChild(a1);
  list->addChild(a2);
  list->addChild(thickness);
  fillTypeEnum();
  connect(name, SIGNAL(wasEdited(VarType*)), this, SLOT(Rename()));
}

FieldCircularArc::FieldCircularArc(const string& marking_name) :
    name(new VarString("Name", marking_name)),
    type(new VarStringEnum("type", "Undefined")),
    center_x(new VarDouble("Center.x")),
    center_y(new VarDouble("Center.y")),
    radius(new VarDouble("Radius")),
    a1(new VarDouble("Start angle")),
    a2(new VarDouble("End angle")),
    thickness(new VarDouble("Line thickness", 10)),
    list(new VarList(marking_name)) {
  list->addChild(name);
  list->addChild(type);
  list->addChild(center_x);
  list->addChild(center_y);
  list->addChild(radius);
  list->addChild(a1);
  list->addChild(a2);
  list->addChild(thickness);
  fillTypeEnum();
  connect(name, SIGNAL(wasEdited(VarType*)), this, SLOT(Rename()));
}

FieldCircularArc::~FieldCircularArc() {
  list->removeChild(thickness);
  list->removeChild(type);
  list->removeChild(a2);
  list->removeChild(a1);
  list->removeChild(radius);
  list->removeChild(center_y);
  list->removeChild(center_x);
  list->removeChild(name);
  delete name;
  delete type;
  delete center_x;
  delete center_y;
  delete radius;
  delete a1;
  delete a2;
  delete thickness;
  delete list;
}

void FieldCircularArc::fillTypeEnum() const {
  type->addItem("Undefined");
  type->addItem("CenterCircle");
}

void FieldCircularArc::Rename() {
  list->setName(name->getString());
}

RoboCupField::RoboCupField() {
  settings = new VarList("Field Configuration");
  // Using default values based on default test images here, which are slightly different compared to DivA constants
  field_length = new VarDouble(
      "Field Length", 12040);
  field_width = new VarDouble(
      "Field Width", 9020);
  goal_width = new VarDouble(
      "Goal Width", 1200);
  goal_depth = new VarDouble(
      "Goal Depth", 180);
  goal_height = new VarDouble(
      "Goal Height", 155);
  boundary_width = new VarDouble(
      "Boundary Width", 300);
  line_thickness = new VarDouble(
      "Line Thickness", 10);
  penalty_area_depth = new VarDouble(
       "Penalty Area Depth", 1220);
  penalty_area_width = new VarDouble(
       "Penalty Area Width", 2410);
  goal_center_to_penalty_mark = new VarDouble(
      "Goal Line to Penalty Mark", 8000);
  center_circle_radius = new VarDouble(
      "Center Circle Radius", 500);
  ball_radius = new VarDouble(
      "Ball Radius", FieldConstantsRoboCup2018A::kBallRadius);
  max_robot_radius = new VarDouble(
      "Max Robot Radius", FieldConstantsRoboCup2018A::kMaxRobotRadius);
  num_cameras_total = new VarInt(
       "Total Number of Cameras", 2);
  num_cameras_local = new VarInt(
       "Local Number of Cameras", 2);
  center_line = new VarBool(
    "Center Line", true);

  updateShapes = new VarTrigger("Field Lines/Arcs","Update");
  applyDivisionA = new VarTrigger("Division A", "Apply");
  applyDivisionB = new VarTrigger("Division B", "Apply");
  applyDivisionC = new VarTrigger("Division C", "Apply");
  var_num_lines = new VarInt("Number of Line Segments", 0);
  var_num_arcs = new VarInt("Number of Arcs", 0);
  field_lines_list = new VarList("Field Lines");
  field_arcs_list = new VarList("Field Arcs");

  field_models = new RoboCupFieldModels();

  settings->addChild(field_models->settings);
  settings->addChild(applyDivisionA);
  settings->addChild(applyDivisionB);
  settings->addChild(applyDivisionC);
  settings->addChild(field_length);
  settings->addChild(field_width);
  settings->addChild(goal_width);
  settings->addChild(goal_depth);
  settings->addChild(goal_height);
  settings->addChild(boundary_width);
  settings->addChild(line_thickness);
  settings->addChild(penalty_area_depth);
  settings->addChild(penalty_area_width);
  settings->addChild(goal_center_to_penalty_mark);
  settings->addChild(center_circle_radius);
  settings->addChild(ball_radius);
  settings->addChild(max_robot_radius);
  settings->addChild(num_cameras_total);
  settings->addChild(num_cameras_local);
  settings->addChild(center_line);
  settings->addChild(var_num_lines);
  settings->addChild(var_num_arcs);
  settings->addChild(updateShapes);

  settings->addChild(field_lines_list);
  settings->addChild(field_arcs_list);

  connect(updateShapes,SIGNAL(wasEdited(VarType*)),this,SLOT(updateFieldLinesAndArcs()));
  connect(applyDivisionA,SIGNAL(wasEdited(VarType*)),this,SLOT(applyGeometryDivisionA()));
  connect(applyDivisionB,SIGNAL(wasEdited(VarType*)),this,SLOT(applyGeometryDivisionB()));
  connect(applyDivisionC, SIGNAL(wasEdited(VarType*)), this, SLOT(applyGeometryDivisionC()));
  connect(var_num_lines, SIGNAL(wasEdited(VarType*)),
          this, SLOT(ResizeFieldLines()));
  connect(var_num_arcs, SIGNAL(wasEdited(VarType*)),
          this, SLOT(ResizeFieldArcs()));
  connect(field_lines_list, SIGNAL(XMLwasRead(VarType*)),
          this, SLOT(ProcessNewFieldLines()));
  connect(field_arcs_list, SIGNAL(XMLwasRead(VarType*)),
          this, SLOT(ProcessNewFieldArcs()));

  shapeTypeMap["Undefined"] = Undefined;
  shapeTypeMap["CenterCircle"] = CenterCircle;
  shapeTypeMap["TopTouchLine"] = TopTouchLine;
  shapeTypeMap["BottomTouchLine"] = BottomTouchLine;
  shapeTypeMap["LeftGoalLine"] = LeftGoalLine;
  shapeTypeMap["RightGoalLine"] = RightGoalLine;
  shapeTypeMap["HalfwayLine"] = HalfwayLine;
  shapeTypeMap["CenterLine"] = CenterLine;
  shapeTypeMap["LeftPenaltyStretch"] = LeftPenaltyStretch;
  shapeTypeMap["RightPenaltyStretch"] = RightPenaltyStretch;
  shapeTypeMap["LeftFieldLeftPenaltyStretch"] = LeftFieldLeftPenaltyStretch;
  shapeTypeMap["LeftFieldRightPenaltyStretch"] = LeftFieldRightPenaltyStretch;
  shapeTypeMap["RightFieldLeftPenaltyStretch"] = RightFieldLeftPenaltyStretch;
  shapeTypeMap["RightFieldRightPenaltyStretch"] = RightFieldRightPenaltyStretch;

  if (field_lines.empty() && field_arcs.empty()) {
    // Generate default field lines and arcs
    updateFieldLinesAndArcs();
  }

  emit calibrationChanged();
}

RoboCupField::~RoboCupField() {
  delete field_length;
  delete field_width;
  delete goal_width;
  delete goal_depth;
  delete goal_height;
  delete boundary_width;
  delete line_thickness;
  delete penalty_area_depth;
  delete penalty_area_width;
  delete goal_center_to_penalty_mark;
  delete center_circle_radius;
  delete ball_radius;
  delete max_robot_radius;
  delete num_cameras_total;
  delete num_cameras_local;
  delete center_line;
  delete var_num_lines;
  delete var_num_arcs;
  for (size_t i = 0; i < field_lines.size(); ++i) {
    delete field_lines[i];
  }
  for (size_t i = 0; i < field_arcs.size(); ++i) {
    delete field_arcs[i];
  }
  delete updateShapes;
  delete settings;
}

void RoboCupField::applyGeometryDivisionA() {
    field_length->setDouble(FieldConstantsRoboCup2018A::kFieldLength);
    field_width->setDouble(FieldConstantsRoboCup2018A::kFieldWidth);
    goal_width->setDouble(FieldConstantsRoboCup2018A::kGoalWidth);
    goal_depth->setDouble(FieldConstantsRoboCup2018A::kGoalDepth);
    goal_height->setDouble(FieldConstantsRoboCup2018A::kGoalHeight);
    boundary_width->setDouble(FieldConstantsRoboCup2018A::kBoundaryWidth);
    line_thickness->setDouble(FieldConstantsRoboCup2018A::kLineThickness);
    penalty_area_depth->setDouble(FieldConstantsRoboCup2018A::kPenaltyAreaDepth);
    penalty_area_width->setDouble(FieldConstantsRoboCup2018A::kPenaltyAreaWidth);
    goal_center_to_penalty_mark->setDouble(FieldConstantsRoboCup2018A::kGoalLineToPenaltyMark);
    center_circle_radius->setDouble(FieldConstantsRoboCup2018A::kCenterCircleRadius);
    ball_radius->setDouble(FieldConstantsRoboCup2018A::kBallRadius);
    max_robot_radius->setDouble(FieldConstantsRoboCup2018A::kMaxRobotRadius);
    num_cameras_total->setDouble(FieldConstantsRoboCup2018A::kNumCamerasTotal);
    num_cameras_local->setDouble(FieldConstantsRoboCup2018A::kNumCamerasLocal);
}

void RoboCupField::applyGeometryDivisionB() {
    field_length->setDouble(FieldConstantsRoboCup2018B::kFieldLength);
    field_width->setDouble(FieldConstantsRoboCup2018B::kFieldWidth);
    goal_width->setDouble(FieldConstantsRoboCup2018B::kGoalWidth);
    goal_depth->setDouble(FieldConstantsRoboCup2018B::kGoalDepth);
    goal_height->setDouble(FieldConstantsRoboCup2018B::kGoalHeight);
    boundary_width->setDouble(FieldConstantsRoboCup2018B::kBoundaryWidth);
    line_thickness->setDouble(FieldConstantsRoboCup2018B::kLineThickness);
    penalty_area_depth->setDouble(FieldConstantsRoboCup2018B::kPenaltyAreaDepth);
    penalty_area_width->setDouble(FieldConstantsRoboCup2018B::kPenaltyAreaWidth);
    goal_center_to_penalty_mark->setDouble(FieldConstantsRoboCup2018B::kGoalLineToPenaltyMark);
    center_circle_radius->setDouble(FieldConstantsRoboCup2018B::kCenterCircleRadius);
    ball_radius->setDouble(FieldConstantsRoboCup2018B::kBallRadius);
    max_robot_radius->setDouble(FieldConstantsRoboCup2018B::kMaxRobotRadius);
    num_cameras_total->setDouble(FieldConstantsRoboCup2018B::kNumCamerasTotal);
    num_cameras_local->setDouble(FieldConstantsRoboCup2018B::kNumCamerasLocal);
}

void RoboCupField::applyGeometryDivisionC() {
    field_length->setDouble(FieldConstantsSSLEL::kFieldLength);
    field_width->setDouble(FieldConstantsSSLEL::kFieldWidth);
    goal_width->setDouble(FieldConstantsSSLEL::kGoalWidth);
    goal_depth->setDouble(FieldConstantsSSLEL::kGoalDepth);
    goal_height->setDouble(FieldConstantsSSLEL::kGoalHeight);
    boundary_width->setDouble(FieldConstantsSSLEL::kBoundaryWidth);
    line_thickness->setDouble(FieldConstantsSSLEL::kLineThickness);
    penalty_area_depth->setDouble(FieldConstantsSSLEL::kPenaltyAreaDepth);
    penalty_area_width->setDouble(FieldConstantsSSLEL::kPenaltyAreaWidth);
    goal_center_to_penalty_mark->setDouble(FieldConstantsSSLEL::kGoalLineToPenaltyMark);
    center_circle_radius->setDouble(FieldConstantsSSLEL::kCenterCircleRadius);
    ball_radius->setDouble(FieldConstantsSSLEL::kBallRadius);
    max_robot_radius->setDouble(FieldConstantsSSLEL::kMaxRobotRadius);
    num_cameras_total->setDouble(FieldConstantsSSLEL::kNumCamerasTotal);
    num_cameras_local->setDouble(FieldConstantsSSLEL::kNumCamerasLocal);
}

SSL_FieldShapeType RoboCupField::parseShapeType(const VarStringEnum* value) const {
  std::string type = value->getString();
  if(shapeTypeMap.find(type) != shapeTypeMap.end()) {
    return shapeTypeMap.at(type);
  }
  return Undefined;
}

void RoboCupField::toProtoBuffer(SSL_GeometryFieldSize& buffer) const {
  field_markings_mutex.lockForRead();
  buffer.Clear();
  buffer.set_field_length((int) field_length->getDouble());
  buffer.set_field_width((int) field_width->getDouble());
  buffer.set_goal_width((int) goal_width->getDouble());
  buffer.set_goal_depth((int) goal_depth->getDouble());
  buffer.set_boundary_width((int) boundary_width->getDouble());
  buffer.set_penalty_area_depth((int) penalty_area_depth->getDouble());
  buffer.set_penalty_area_width((int) penalty_area_width->getDouble());
  buffer.set_center_circle_radius((int) center_circle_radius->getDouble());
  buffer.set_line_thickness((int) line_thickness->getDouble());
  buffer.set_goal_center_to_penalty_mark((int)goal_center_to_penalty_mark->getDouble());
  buffer.set_goal_height((int) goal_height->getDouble());
  buffer.set_ball_radius((float) ball_radius->getDouble());
  buffer.set_max_robot_radius((float) max_robot_radius->getDouble());

  for (size_t i = 0; i < field_lines.size(); ++i) {
    const FieldLine& line = *(field_lines[i]);
    SSL_FieldLineSegment proto_line;
    proto_line.set_name(line.name->getString());
    proto_line.set_type(parseShapeType(line.type));
    proto_line.mutable_p1()->set_x(line.p1_x->getDouble());
    proto_line.mutable_p1()->set_y(line.p1_y->getDouble());
    proto_line.mutable_p2()->set_x(line.p2_x->getDouble());
    proto_line.mutable_p2()->set_y(line.p2_y->getDouble());
    proto_line.set_thickness(line.thickness->getDouble());
    *(buffer.add_field_lines()) = proto_line;
  }
  for (size_t i = 0; i < field_arcs.size(); ++i) {
    const FieldCircularArc& arc = *(field_arcs[i]);
    SSL_FieldCircularArc proto_arc;
    proto_arc.set_name(arc.name->getString());
    proto_arc.set_type(parseShapeType(arc.type));
    proto_arc.mutable_center()->set_x(arc.center_x->getDouble());
    proto_arc.mutable_center()->set_y(arc.center_y->getDouble());
    proto_arc.set_radius(arc.radius->getDouble());
    proto_arc.set_a1(arc.a1->getDouble());
    proto_arc.set_a2(arc.a2->getDouble());
    proto_arc.set_thickness(arc.thickness->getDouble());
    *(buffer.add_field_arcs()) = proto_arc;
  }
  field_markings_mutex.unlock();
}

void RoboCupField::toProtoBuffer(SSL_GeometryModels& buffer) const {
  field_markings_mutex.lockForRead();
  auto straightTwoPhase = buffer.mutable_straight_two_phase();
  straightTwoPhase->set_acc_roll(field_models->ballModelStraightTwoPhase.accRoll->getDouble());
  straightTwoPhase->set_acc_slide(field_models->ballModelStraightTwoPhase.accSlide->getDouble());
  straightTwoPhase->set_k_switch(field_models->ballModelStraightTwoPhase.kSwitch->getDouble());
  auto chipFixLoss = buffer.mutable_chip_fixed_loss();
  chipFixLoss->set_damping_xy_first_hop(field_models->ballModelChipFixLoss.dampingXyFirstHop->getDouble());
  chipFixLoss->set_damping_xy_other_hops(field_models->ballModelChipFixLoss.dampingXyOtherHops->getDouble());
  chipFixLoss->set_damping_z(field_models->ballModelChipFixLoss.dampingZ->getDouble());
  field_markings_mutex.unlock();
}

void RoboCupField::ProcessNewFieldArcs() {
  field_markings_mutex.lockForWrite();
  vector<VarType*> field_arc_entries = field_arcs_list->getChildren();
  for (size_t i = 0; i < field_arc_entries.size(); ++i) {
    VarList* field_arc_list_ptr =
        reinterpret_cast<VarList*>(field_arc_entries[i]);
    bool found = false;
    for (size_t j = 0; !found && j < field_arcs.size(); ++j) {
      found = (field_arcs[j]->list == field_arc_list_ptr);
    }
    if (!found) {
      // This item is not in field_arcs, try to cast and add it.
      FieldCircularArc* new_arc =
          FieldCircularArc::FromVarList(field_arc_list_ptr);
      if (new_arc != NULL) {
        // Cast succeeded, add it.
        field_arcs.push_back(new_arc);
      } else {
        // Cast failed, drop this entry.
        field_arcs_list->removeChild(field_arc_list_ptr);
      }
    }
  }
  if (static_cast<size_t>(field_arcs_list->getChildrenCount()) != field_arcs.size()) {
    fprintf(stderr, "Bug discovered, please report to the developers: "
            "field_arcs_list->getChildrenCount() != field_arcs.size(), @ "
            "%s:%d\n", __FILE__, __LINE__);
  }
  var_num_arcs->setInt(field_arcs.size());
  field_markings_mutex.unlock();
}

void RoboCupField::ProcessNewFieldLines() {
  field_markings_mutex.lockForWrite();
  vector<VarType*> field_line_entries = field_lines_list->getChildren();
  for (size_t i = 0; i < field_line_entries.size(); ++i) {
    VarList* field_line_list_ptr =
        reinterpret_cast<VarList*>(field_line_entries[i]);
    bool found = false;
    for (size_t j = 0; !found && j < field_lines.size(); ++j) {
      found = (field_lines[j]->list == field_line_list_ptr);
    }
    if (!found) {
      // This item is not in field_lines, try to cast and add it.
      FieldLine* new_line = FieldLine::FromVarList(field_line_list_ptr);
      if (new_line != NULL) {
        // Cast succeeded, add it.
        field_lines.push_back(new_line);
      } else {
        // Cast failed, drop this entry.
        field_lines_list->removeChild(field_line_list_ptr);
      }
    }
  }
  if (static_cast<size_t>(field_lines_list->getChildrenCount()) != field_lines.size()) {
    fprintf(stderr, "Bug discovered, please report to the developers: "
            "field_lines_list->getChildrenCount() != field_lines.size(), @ "
            "%s:%d\n", __FILE__, __LINE__);
  }
  var_num_lines->setInt(field_lines.size());
  field_markings_mutex.unlock();
}

void RoboCupField::ResizeFieldLines() {
  field_markings_mutex.lockForWrite();
  // Resize the field_lines list.
  const size_t new_num_lines = static_cast<size_t>(var_num_lines->getInt());
  const size_t old_num_lines = field_lines.size();
  if (new_num_lines < old_num_lines) {
    // Remove the last few line segments and their associated VarType objects.
    for (size_t i = new_num_lines; i < old_num_lines; ++i) {
      field_lines_list->removeChild(field_lines[i]->list);
      delete field_lines[i];
    }
    field_lines.resize(new_num_lines);
  }
  if (new_num_lines > old_num_lines) {
    for (size_t i = old_num_lines; i < new_num_lines; ++i) {
      const string name = StringPrintf("Line %d", static_cast<int>(i));
      field_lines.push_back(new FieldLine(name));
      field_lines_list->addChild(field_lines[i]->list);
    }
  }
  if (static_cast<size_t>(field_lines_list->getChildrenCount()) != field_lines.size()) {
    fprintf(stderr, "Bug discovered, please report to the developers: "
            "field_lines_list->getChildrenCount() != field_lines.size(), @ "
            "%s:%d\n", __FILE__, __LINE__);
  }
  field_markings_mutex.unlock();
}

void RoboCupField::ResizeFieldArcs() {
  field_markings_mutex.lockForWrite();
  // Resize the field_arcs list.
  const size_t new_num_arcs = static_cast<size_t>(var_num_arcs->getInt());
  const size_t old_num_arcs = field_arcs.size();
  if (new_num_arcs < old_num_arcs) {
    // Remove the last few arc segments and their associated VarType objects.
    for (size_t i = new_num_arcs; i < old_num_arcs; ++i) {
      field_arcs_list->removeChild(field_arcs[i]->list);
      delete field_arcs[i];
    }
    field_arcs.resize(new_num_arcs);
  }
  if (new_num_arcs > old_num_arcs) {
    for (size_t i = old_num_arcs; i < new_num_arcs; ++i) {
      const string name = StringPrintf("Arc %d", static_cast<int>(i));
      field_arcs.push_back(new FieldCircularArc(name));
      field_arcs_list->addChild(field_arcs[i]->list);
    }
  }
  if (static_cast<size_t>(field_arcs_list->getChildrenCount()) != field_arcs.size()) {
    fprintf(stderr, "Bug discovered, please report to the developers: "
            "field_arcs_list->getChildrenCount() != field_arcs.size(), @ "
            "%s:%d\n", __FILE__, __LINE__);
  }
  field_markings_mutex.unlock();
}

void RoboCupField::updateFieldLinesAndArcs() {
    field_markings_mutex.lockForWrite();

    // Delete all old lines.
    vector<VarType*> old_lines = field_lines_list->getChildren();
    for (size_t i = 0; i < old_lines.size(); ++i) {
    field_lines_list->removeChild(old_lines[i]);
    delete field_lines[i];
    }
    field_lines.clear();

    // Load default lines.
    double fieldLengthHalf = field_length->getDouble() / 2.0;
    double fieldWidthHalf = field_width->getDouble() / 2.0;
    double penAreaX = fieldLengthHalf - penalty_area_depth->getDouble();
    double penAreaY = penalty_area_width->getDouble() / 2.0;
    field_lines.push_back(new FieldLine("TopTouchLine", -fieldLengthHalf, fieldWidthHalf, fieldLengthHalf, fieldWidthHalf, line_thickness->getDouble()));
    field_lines.push_back(new FieldLine("BottomTouchLine", -fieldLengthHalf, -fieldWidthHalf, fieldLengthHalf, -fieldWidthHalf, line_thickness->getDouble()));
    field_lines.push_back(new FieldLine("LeftGoalLine", -fieldLengthHalf, -fieldWidthHalf, -fieldLengthHalf, fieldWidthHalf, line_thickness->getDouble()));
    field_lines.push_back(new FieldLine("RightGoalLine", fieldLengthHalf, -fieldWidthHalf, fieldLengthHalf, fieldWidthHalf, line_thickness->getDouble()));
    field_lines.push_back(new FieldLine("HalfwayLine", 0, -fieldWidthHalf, 0, fieldWidthHalf, line_thickness->getDouble()));
    field_lines.push_back(new FieldLine("LeftPenaltyStretch", -penAreaX, -penAreaY, -penAreaX, penAreaY, line_thickness->getDouble()));
    field_lines.push_back(new FieldLine("RightPenaltyStretch", penAreaX, -penAreaY, penAreaX, penAreaY, line_thickness->getDouble()));
    field_lines.push_back(new FieldLine("LeftFieldLeftPenaltyStretch", -fieldLengthHalf, -penAreaY, -penAreaX, -penAreaY, line_thickness->getDouble()));
    field_lines.push_back(new FieldLine("LeftFieldRightPenaltyStretch", -fieldLengthHalf, penAreaY, -penAreaX, penAreaY, line_thickness->getDouble()));
    field_lines.push_back(new FieldLine("RightFieldRightPenaltyStretch", fieldLengthHalf, -penAreaY, penAreaX, -penAreaY, line_thickness->getDouble()));
    field_lines.push_back(new FieldLine("RightFieldLeftPenaltyStretch", fieldLengthHalf, penAreaY, penAreaX, penAreaY, line_thickness->getDouble()));

    if (center_line->getBool()) {
      field_lines.push_back(
        new FieldLine("CenterLine", -fieldLengthHalf, 0, fieldLengthHalf, 0, line_thickness->getDouble()));
    }

    var_num_lines->setInt(field_lines.size());
    for (size_t i = 0; i < field_lines.size(); ++i) {
        field_lines_list->addChild(field_lines[i]->list);
    }

    // Delete all old arcs.
    vector<VarType*> old_arcs = field_arcs_list->getChildren();
    for (size_t i = 0; i < old_arcs.size(); ++i) {
      field_arcs_list->removeChild(old_arcs[i]);
      delete field_arcs[i];
    }
    field_arcs.clear();

    // Load default arcs.
    field_arcs.push_back(new FieldCircularArc("CenterCircle", 0, 0, center_circle_radius->getDouble(), 0, 2.0 * M_PI, 10));

    var_num_arcs->setInt(field_arcs.size());
    for (size_t i = 0; i < field_arcs.size(); ++i) {
      field_arcs_list->addChild(field_arcs[i]->list);
    }
    field_markings_mutex.unlock();
}

BallModelStraightTwoPhase::BallModelStraightTwoPhase() {
  accSlide = new VarDouble("Sliding acceleration [m/s^2]", -3.0, -20, 0);
  accRoll = new VarDouble("Rolling acceleration [m/s^2]", -0.4, -20, 0);
  kSwitch = new VarDouble("Switch factor", 0.7, 0, 1);
  settings = new VarList("Straight Two-Phase");
  settings->addChild(accSlide);
  settings->addChild(accRoll);
  settings->addChild(kSwitch);
}

BallModelStraightTwoPhase::~BallModelStraightTwoPhase() {
  delete accSlide;
  delete accRoll;
  delete kSwitch;
  delete settings;
}

BallModelChipFixLoss::BallModelChipFixLoss() {
  dampingXyFirstHop = new VarDouble("Damping (xy) on first hop", 0.6, 0, 1);
  dampingXyOtherHops = new VarDouble("Damping (xy) on other hops", 0.95, 0, 1);
  dampingZ = new VarDouble("Damping (z)", 0.7, 0, 1);
  settings = new VarList("Chip fix loss");
  settings->addChild(dampingXyFirstHop);
  settings->addChild(dampingXyOtherHops);
  settings->addChild(dampingZ);
}

BallModelChipFixLoss::~BallModelChipFixLoss() {
  delete dampingXyFirstHop;
  delete dampingXyOtherHops;
  delete dampingZ;
  delete settings;
}

RoboCupFieldModels::RoboCupFieldModels() {
  settings = new VarList("Models");
  settings->addChild(ballModelStraightTwoPhase.settings);
  settings->addChild(ballModelChipFixLoss.settings);
}

RoboCupFieldModels::~RoboCupFieldModels() {
  delete settings;
}

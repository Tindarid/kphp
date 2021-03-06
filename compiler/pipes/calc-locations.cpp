// Compiler for PHP (aka KPHP)
// Copyright (c) 2020 LLC «V Kontakte»
// Distributed under the GPL v3 License, see LICENSE.notice.txt

#include "compiler/pipes/calc-locations.h"

#include "compiler/data/class-data.h"

void CalcLocationsPass::on_start() {
  if (current_function->type == FunctionData::func_class_holder) {
    current_function->class_id->members.for_each([](ClassMemberConstant &constant) {
      stage::set_line(constant.value->location.line);
      constant.value.set_location(stage::get_location());
    });
  }
}

VertexPtr CalcLocationsPass::on_enter_vertex(VertexPtr v) {
  stage::set_line(v->location.line);
  v->location = stage::get_location();
  return v;
}

// RUN: %check_clang_tidy -std=c23-or-later %s readability-implicit-bool-conversion %t \
// RUN: -config='{CheckOptions: \
// RUN: {readability-implicit-bool-conversion.AllowIntegerConditions: true, \
// RUN: readability-implicit-bool-conversion.AllowPointerConditions: true}}'

void functionTakingBool(bool);

void allowedConditions(int integer, int *pointer, float floating) {
  if (integer) {}
  if (!integer) {}
  if (pointer) {}
  if (!pointer) {}
  while (integer && pointer) {}
  for (; pointer;) {}
  do {
  } while (integer);
  int value = pointer ? 1 : 2;
  int value2 = integer ? 1 : 2;

  // Float conditions are not covered by the options.
  if (floating) {}
  // CHECK-MESSAGES: :[[@LINE-1]]:7: warning: implicit conversion 'float' -> 'bool' [readability-implicit-bool-conversion]
  // CHECK-FIXES: if (floating != 0.0f) {}
}

void notAllowedOutsideConditions(int integer, int *pointer) {
  functionTakingBool(integer);
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: implicit conversion 'int' -> 'bool'
  // CHECK-FIXES: functionTakingBool(integer != 0);

  functionTakingBool(pointer);
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: implicit conversion 'int *' -> 'bool'
  // CHECK-FIXES: functionTakingBool(pointer != nullptr);

  bool fromLogical = pointer || integer;
  // CHECK-MESSAGES: :[[@LINE-1]]:22: warning: implicit conversion 'int *' -> 'bool'
  // CHECK-MESSAGES: :[[@LINE-2]]:33: warning: implicit conversion 'int' -> 'bool'
  // CHECK-FIXES: bool fromLogical = (pointer != nullptr) || (integer != 0);
}

// RUN: %clang_cc1 -templight-dump %s 2>&1 | FileCheck %s

template <class T>
struct foo {};

// CHECK-LABEL: {{^---$}}
// CHECK: {{^name:[ ]+'foo<int>'$}}
// CHECK: {{^kind:[ ]+TemplateInstantiation$}}
// CHECK: {{^event:[ ]+Begin$}}
// CHECK: {{^poi:[ ]+'.*templight-memoization.cpp:}}[[@LINE+6]]{{:10'$}}
// CHECK-LABEL: {{^---$}}
// CHECK: {{^name:[ ]+'foo<int>'$}}
// CHECK: {{^kind:[ ]+TemplateInstantiation$}}
// CHECK: {{^event:[ ]+End$}}
// CHECK: {{^poi:[ ]+'.*templight-memoization.cpp:}}[[@LINE+1]]{{:10'$}}
foo<int> x;

// CHECK-LABEL: {{^---$}}
// CHECK-LABEL: {{^---$}}

// CHECK-LABEL: {{^---$}}
// CHECK: {{^name:[ ]+'foo<int>'$}}
// CHECK: {{^kind:[ ]+Memoization$}}
// CHECK: {{^event:[ ]+Begin$}}
// CHECK: {{^poi:[ ]+'.*templight-memoization.cpp:}}[[@LINE+6]]{{:10'$}}
// CHECK-LABEL: {{^---$}}
// CHECK: {{^name:[ ]+'foo<int>'$}}
// CHECK: {{^kind:[ ]+Memoization$}}
// CHECK: {{^event:[ ]+End$}}
// CHECK: {{^poi:[ ]+'.*templight-memoization.cpp:}}[[@LINE+1]]{{:10'$}}
foo<int> y;


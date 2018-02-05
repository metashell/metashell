// RUN: %clang_cc1 -templight-dump %s 2>&1 | FileCheck %s
template<class T>
class A {};

template <template <class Inner> class Outer>
class B {};

// CHECK-LABEL: {{^---$}}
// CHECK: {{^name:[ ]+'B::Outer'$}}
// CHECK: {{^kind:[ ]+PriorTemplateArgumentSubstitution$}}
// CHECK: {{^event:[ ]+Begin$}}
// CHECK: {{^orig:[ ]+'.*templight-prior-template-arg.cpp:5:40'}}
// CHECK: {{^poi:[ ]+'.*templight-prior-template-arg.cpp:}}[[@LINE+50]]{{:1'$}}
// CHECK-LABEL: {{^---$}}
// CHECK: {{^name:[ ]+'B::Outer'$}}
// CHECK: {{^kind:[ ]+PriorTemplateArgumentSubstitution$}}
// CHECK: {{^event:[ ]+End$}}
// CHECK: {{^orig:[ ]+'.*templight-prior-template-arg.cpp:5:40'}}
// CHECK: {{^poi:[ ]+'.*templight-prior-template-arg.cpp:}}[[@LINE+45]]{{:1'$}}
//
// CHECK-LABEL: {{^---$}}
// CHECK: {{^name:[ ]+'B<A>'$}}
// CHECK: {{^kind:[ ]+TemplateInstantiation$}}
// CHECK: {{^event:[ ]+Begin$}}
// CHECK: {{^orig:[ ]+'.*templight-prior-template-arg.cpp:6:7'}}
// CHECK: {{^poi:[ ]+'.*templight-prior-template-arg.cpp:}}[[@LINE+39]]{{:6'$}}
// CHECK-LABEL: {{^---$}}
// CHECK: {{^name:[ ]+'B<A>'$}}
// CHECK: {{^kind:[ ]+TemplateInstantiation$}}
// CHECK: {{^event:[ ]+End$}}
// CHECK: {{^orig:[ ]+'.*templight-prior-template-arg.cpp:6:7'}}
// CHECK: {{^poi:[ ]+'.*templight-prior-template-arg.cpp:}}[[@LINE+34]]{{:6'$}}
//
// CHECK-LABEL: {{^---$}}
// CHECK: {{^name:[ ]+'B<A>'$}}
// CHECK: {{^kind:[ ]+TemplateInstantiation$}}
// CHECK: {{^event:[ ]+Begin$}}
// CHECK: {{^orig:[ ]+'.*templight-prior-template-arg.cpp:6:7'}}
// CHECK: {{^poi:[ ]+'.*templight-prior-template-arg.cpp:}}[[@LINE+28]]{{:6'$}}
// CHECK-LABEL: {{^---$}}
// CHECK: {{^name:[ ]+'B<A>'$}}
// CHECK: {{^kind:[ ]+TemplateInstantiation$}}
// CHECK: {{^event:[ ]+End$}}
// CHECK: {{^orig:[ ]+'.*templight-prior-template-arg.cpp:6:7'}}
// CHECK: {{^poi:[ ]+'.*templight-prior-template-arg.cpp:}}[[@LINE+23]]{{:6'$}}
//
// CHECK-LABEL: {{^---$}}
// CHECK: {{^name:[ ]+'B<A>'$}}
// CHECK: {{^kind:[ ]+Memoization$}}
// CHECK: {{^event:[ ]+Begin$}}
// CHECK: {{^orig:[ ]+'.*templight-prior-template-arg.cpp:6:7'}}
// CHECK: {{^poi:[ ]+'.*templight-prior-template-arg.cpp:}}[[@LINE+17]]{{:6'$}}
// CHECK-LABEL: {{^---$}}
// CHECK: {{^name:[ ]+'B<A>'$}}
// CHECK: {{^kind:[ ]+Memoization$}}
// CHECK: {{^event:[ ]+End$}}
// CHECK: {{^orig:[ ]+'.*templight-prior-template-arg.cpp:6:7'}}
// CHECK: {{^poi:[ ]+'.*templight-prior-template-arg.cpp:}}[[@LINE+12]]{{:6'$}}
//
// CHECK-LABEL: {{^---$}}
// CHECK: {{^name:[ ]+'B<A>'$}}
// CHECK: {{^kind:[ ]+Memoization$}}
// CHECK: {{^event:[ ]+Begin$}}
// CHECK: {{^orig:[ ]+'.*templight-prior-template-arg.cpp:6:7'}}
// CHECK: {{^poi:[ ]+'.*templight-prior-template-arg.cpp:}}[[@LINE+6]]{{:6'$}}
// CHECK-LABEL: {{^---$}}
// CHECK: {{^name:[ ]+'B<A>'$}}
// CHECK: {{^kind:[ ]+Memoization$}}
// CHECK: {{^event:[ ]+End$}}
// CHECK: {{^orig:[ ]+'.*templight-prior-template-arg.cpp:6:7'}}
// CHECK: {{^poi:[ ]+'.*templight-prior-template-arg.cpp:}}[[@LINE+1]]{{:6'$}}
B<A> b;

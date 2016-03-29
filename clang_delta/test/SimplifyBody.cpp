//RUN: %clangdelta --transformation=simplify-body --counter=1 %s 2>&1 | FileCheck %s
//RUN: %clangdelta --transformation=simplify-body --counter=1 --to-counter=4 %s 2>&1 | FileCheck  -check-prefix=CHECK-MULTI %s
//RUN: %clangdelta --query-instances=simplify-body %s 2>&1 | FileCheck -check-prefix=CHECK-QI %s

void f() {
  int i = 0, j;
  for (int f = i; i < 10; ++i)
    ++j;
}

struct S {
  int field;
  S* g() {
    if (field == 1)
      return new S();
    else if (field == 0)
      return 0;
    return 0;
  }
  // Since the simplify-body heuristics order the decls by size of reduction
  // this is our best candidate and thus it gets removed first.
  // CHECK: S* g() {return 0;}
  // CHECK-MULTI: S* g() {return 0;}
  S& forward_decl_ref();
  S& forward_decl();
  S& h() {
    if (field == 1)
      return forward_decl_ref();
    return forward_decl();
  }
  // CHECK-MULTI: S& h() {static S s; return s;}
  S i() {
    if (field == 1)
      return forward_decl();
    return forward_decl_ref();
  }
  // CHECK-MULTI: S i() {return S();}
};

// This is broken source code but it might happen if CREDUCE_INCLUDE_PATH is not
// properly set.
class FwdDeclS;
FwdDeclS& should_ignore() {
  S s;
  return reinterpret_cast<FwdDeclS&>(s);
}

FwdDeclS& FwdDeclsFactory();

FwdDeclS should_ignore1() { return FwdDeclsFactory(); }
// End broken code.

//CHECK-QI: Available transformation instances: 4

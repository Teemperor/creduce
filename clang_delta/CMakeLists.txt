# Copyright (c) 2012, 2013, 2014, 2015, 2016 The University of Utah
# All rights reserved.
#
# This file is distributed under the University of Illinois Open Source
# License.  See the file COPYING for details.

cmake_minimum_required(VERSION 2.8.12)
project(clang_delta)

# Find LLVM
find_package(LLVM REQUIRED CONFIG NO_CMAKE_BUILDS_PATH)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Check LLVM version
if (${LLVM_PACKAGE_VERSION} VERSION_LESS "3.8")
  message(FATAL_ERROR "LLVM version must be >= 3.8")
endif()
set(LLVM_VERSION ${LLVM_VERSION_MAJOR}${LLVM_VERSION_MINOR})

# Add flags for LLVM
add_definitions(${LLVM_DEFINITIONS})
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS HAVE_CONFIG_H
  $<$<CONFIG:Release>:NDEBUG> $<$<CONFIG:Debug>:ENABLE_TRANS_ASSERT>)
include_directories(${LLVM_INCLUDE_DIRS})

# Add Clang's include paths if this is a debug build and they are not under
# the same path as LLVM's. FIXME: Find a better way to do this.
if (EXISTS ${LLVM_BUILD_BINARY_DIR}/tools/clang/include)
  include_directories(${LLVM_BUILD_BINARY_DIR}/tools/clang/include)
endif()
if (EXISTS ${LLVM_BUILD_MAIN_SRC_DIR}/tools/clang/include)
  include_directories(${LLVM_BUILD_MAIN_SRC_DIR}/tools/clang/include)
endif()
include_directories(${CMAKE_BINARY_DIR})
link_directories(${LLVM_LIBRARY_DIRS})

llvm_map_components_to_libnames(LLVM_LIBS
  irreader
  mcparser
  option
  profiledata
)

set(CLANG_LIBS
  clangRewrite
  clangFrontend
  clangParse
  clangSema
  clangSerialization
  clangAnalysis
  clangEdit
  clangLex
  clangAST
  clangDriver
  clangBasic
)

set(TRANSFORMATION_HEADERS
  AggregateToScalar.h
  BinOpSimplification.h
  CallExprToValue.h
  ClassTemplateToClass.h
  CombineGlobalVarDecl.h
  CombineLocalVarDecl.h
  CommonParameterRewriteVisitor.h
  CommonRenameClassRewriteVisitor.h
  CommonStatementVisitor.h
  CommonTemplateArgumentVisitor.h
  CopyPropagation.h
  EmptyStructToInt.h
  InstantiateTemplateParam.h
  InstantiateTemplateTypeParamToInt.h
  LiftAssignmentExpr.h
  LocalToGlobal.h
  MoveFunctionBody.h
  MoveGlobalVar.h
  ParamToGlobal.h
  ParamToLocal.h
  ReduceArrayDim.h
  ReduceArraySize.h
  ReduceClassTemplateParameter.h
  ReducePointerLevel.h
  ReducePointerPairs.h
  RemoveAddrTaken.h
  RemoveArray.h
  RemoveBaseClass.h
  RemoveCtorInitializer.h
  RemoveDecl.h
  RemoveEnumMemberValue.h
  RemoveNamespace.h
  RemoveNestedFunction.h
  RemovePointer.h
  RemoveTrivialBaseTemplate.h
  RemoveUnresolvedBase.h
  RemoveUnusedEnumMember.h
  RemoveUnusedFunction.h
  RemoveUnusedOuterClass.h
  RemoveUnusedStructField.h
  RemoveUnusedVar.h
  RenameCXXMethod.h
  RenameClass.h
  RenameFun.h
  RenameParam.h
  RenameVar.h
  ReplaceArrayAccessWithIndex.h
  ReplaceArrayIndexVar.h
  ReplaceCallExpr.h
  ReplaceClassWithBaseTemplateSpec.h
  ReplaceDependentName.h
  ReplaceDependentTypedef.h
  ReplaceDerivedClass.h
  ReplaceFunctionDefWithDecl.h
  ReplaceOneLevelTypedefType.h
  ReplaceSimpleTypedef.h
  ReplaceUndefinedFunction.h
  ReturnVoid.h
  SimpleInliner.h
  SimplifyBody.h
  SimplifyCallExpr.h
  SimplifyCommaExpr.h
  SimplifyDependentTypedef.h
  SimplifyIf.h
  SimplifyNestedClass.h
  SimplifyRecursiveTemplateInstantiation.h
  SimplifyStruct.h
  SimplifyStructUnionDecl.h
  TemplateArgToInt.h
  TemplateNonTypeArgToInt.h
  UnifyFunctionDecl.h
  UnionToStruct.h
)

set(TRANSFORMATION_SOURCES
  AggregateToScalar.cpp
  BinOpSimplification.cpp
  CallExprToValue.cpp
  ClassTemplateToClass.cpp
  CombineGlobalVarDecl.cpp
  CombineLocalVarDecl.cpp
  CopyPropagation.cpp
  EmptyStructToInt.cpp
  InstantiateTemplateParam.cpp
  InstantiateTemplateTypeParamToInt.cpp
  LiftAssignmentExpr.cpp
  LocalToGlobal.cpp
  MoveFunctionBody.cpp
  MoveGlobalVar.cpp
  ParamToGlobal.cpp
  ParamToLocal.cpp
  ReduceArrayDim.cpp
  ReduceArraySize.cpp
  ReduceClassTemplateParameter.cpp
  ReducePointerLevel.cpp
  ReducePointerPairs.cpp
  RemoveAddrTaken.cpp
  RemoveArray.cpp
  RemoveBaseClass.cpp
  RemoveCtorInitializer.cpp
  RemoveEnumMemberValue.cpp
  RemoveNamespace.cpp
  RemoveNestedFunction.cpp
  RemovePointer.cpp
  RemoveTrivialBaseTemplate.cpp
  RemoveUnresolvedBase.cpp
  RemoveDecl.cpp
  RemoveUnusedEnumMember.cpp
  RemoveUnusedFunction.cpp
  RemoveUnusedOuterClass.cpp
  RemoveUnusedStructField.cpp
  RemoveUnusedVar.cpp
  RenameCXXMethod.cpp
  RenameClass.cpp
  RenameFun.cpp
  RenameParam.cpp
  RenameVar.cpp
  ReplaceArrayAccessWithIndex.cpp
  ReplaceArrayIndexVar.cpp
  ReplaceCallExpr.cpp
  ReplaceClassWithBaseTemplateSpec.cpp
  ReplaceDependentName.cpp
  ReplaceDependentTypedef.cpp
  ReplaceDerivedClass.cpp
  ReplaceFunctionDefWithDecl.cpp
  ReplaceOneLevelTypedefType.cpp
  ReplaceSimpleTypedef.cpp
  ReplaceUndefinedFunction.cpp
  ReturnVoid.cpp
  SimpleInliner.cpp
  SimplifyBody.cpp
  SimplifyCallExpr.cpp
  SimplifyCommaExpr.cpp
  SimplifyDependentTypedef.cpp
  SimplifyIf.cpp
  SimplifyNestedClass.cpp
  SimplifyRecursiveTemplateInstantiation.cpp
  SimplifyStruct.cpp
  SimplifyStructUnionDecl.cpp
  TemplateArgToInt.cpp
  TemplateNonTypeArgToInt.cpp
  UnifyFunctionDecl.cpp
  UnionToStruct.cpp
)

add_executable(clang_delta
  ${CMAKE_BINARY_DIR}/config.h
  ${TRANSFORMATION_HEADERS}
  ${TRANSFORMATION_SOURCES}
  RewriteUtils.h
  Transformation.h
  TransformationManager.h
  ClangDelta.cpp
  RewriteUtils.cpp
  Transformation.cpp
  TransformationManager.cpp
)

target_link_libraries(clang_delta ${CLANG_LIBS} ${LLVM_LIBS})

install(TARGETS clang_delta
  RUNTIME DESTINATION "bin"
)

add_subdirectory(test)

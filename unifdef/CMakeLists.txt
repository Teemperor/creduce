# Copyright (c) 2012, 2013, 2014, 2015 The University of Utah
# All rights reserved.
#
# This file is distributed under the University of Illinois Open Source
# License.  See the file COPYING for details.

cmake_minimum_required(VERSION 2.8.12)
project(unifdef)

add_executable(unifdef
  unifdef.c
  unifdef.h
  version.h
)

install(TARGETS unifdef
  RUNTIME DESTINATION "bin"
)


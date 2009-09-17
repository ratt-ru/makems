#  $Id: CMakeLists.txt 13883 2009-08-31 13:27:00Z diepen $
#
#  Copyright (C) 2008-2009
#  ASTRON (Netherlands Foundation for Research in Astronomy)
#  P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

include(LofarCTest)

lofar_add_test(testLogger testLogger.cc)
lofar_add_test(tAllocator tAllocator.cc)
lofar_add_test(tDataConvert tDataConvert.cc)
lofar_add_test(tTypeNames tTypeNames.cc)
lofar_add_test(tFileLocator tFileLocator.cc)
lofar_add_test(tStringUtil tStringUtil.cc)
lofar_add_test(tStreamUtil tStreamUtil.cc)
lofar_add_test(tHexdump tHexdump.cc)
lofar_add_test(testSocket testSocket.cc)
lofar_add_test(tComplex tComplex.cc)
lofar_add_test(tProcess tProcess.cc)
lofar_add_test(tRunOnNode tRunOnNode.cc)
lofar_add_test(tTimer tTimer.cc)
lofar_add_test(tNumeric tNumeric.cc)
lofar_add_test(tSingleton tSingleton.cc)
lofar_add_test(tObjectFactory tObjectFactory.cc Shapes.cc Coordinates.cc)
lofar_add_test(tSystemUtil tSystemUtil.cc)
lofar_add_test(tSaxpy tSaxpy.cc)
lofar_add_test(tPrettyUnits tPrettyUnits.cc)
lofar_add_test(tParameterValue tParameterValue.cc)
lofar_add_test(tParameterSet tParameterSet.cc)
lofar_add_test(tParameterSetPerf tParameterSetPerf.cc)
lofar_add_test(tgetparsetvalue DEPENDS getparsetvalue)
lofar_add_test(tBacktrace tBacktrace.cc)
lofar_add_test(tReadLine tReadLine.cc)

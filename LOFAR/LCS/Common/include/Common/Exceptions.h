//#  Exceptions.h: Declaration and definition of SINFONI specific exceptions.
//#
//#  Copyright (C) 2002-2006
//#  ASTRON (Netherlands Foundation for Research in Astronomy)
//#  P.O.Box 2, 7990 AA Dwingeloo, The Netherlands, seg@astron.nl
//#
//#  This program is free software; you can redistribute it and/or modify
//#  it under the terms of the GNU General Public License as published by
//#  the Free Software Foundation; either version 2 of the License, or
//#  (at your option) any later version.
//#
//#  This program is distributed in the hope that it will be useful,
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//#  GNU General Public License for more details.
//#
//#  You should have received a copy of the GNU General Public License
//#  along with this program; if not, write to the Free Software
//#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//#
//#  $Id: Exceptions.h 12371 2008-12-23 13:18:31Z loose $

#ifndef LOFAR_COMMON_EXCEPTIONS_H
#define LOFAR_COMMON_EXCEPTIONS_H

//# Includes
#include <Common/Exception.h>

namespace LOFAR
{
  //
  // This exception will be thrown when an assertion fails.
  //
  EXCEPTION_CLASS(AssertError,Exception);

  //
  // This exception will be thrown when an I/O error occurs.
  //
  EXCEPTION_CLASS(IOException, Exception);

  //
  // This exception will be thrown when a math error occurs.
  //
  EXCEPTION_CLASS(MathException, Exception);

  //
  // This exception will be thrown when a method is called that has not been
  // implemented (yet).
  //
  EXCEPTION_CLASS(NotImplemented, Exception);

  //
  // This exception will be thrown when an error occurs in the ParameterSet
  // classes.
  //
  EXCEPTION_CLASS(APSException, Exception);

} // namespace ASTRON

#endif

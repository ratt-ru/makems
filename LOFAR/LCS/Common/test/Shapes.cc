//#  Shapes.cc: Implementation of Shapes used by ObjectFactory test program.
//#
//#  Copyright (C) 2006
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
//#  $Id: Shapes.cc 9677 2006-11-22 21:18:08Z loose $

//# Always #include <lofar_config.h> first!
#include <lofar_config.h>

//# This code can only be compiled if Boost is available.
#ifdef HAVE_BOOST

//# Includes
#include "Shapes.h"
#include <Common/lofar_iostream.h>

namespace LOFAR
{
  // Use an anonymous namespace. This ensures that variables declared inside
  // it get there own private storage area and are only visible in this
  // compilation unit.
  namespace
  {
    // Forces the classes to be registered during static initialization.
    // We don't care about the resturn value; maybe we should ;-)
    bool dummy = 
      ShapeFactory::instance().registerClass<Rectangle> ("Rectangle") 
      && ShapeFactory::instance().registerClass<Square> ("Square")
      && ShapeFactory::instance().registerClass<Ellipse>("Ellipse")
      && ShapeFactory::instance().registerClass<Circle> ("Circle");
  }

  void Rectangle::print(ostream& os) const
  {
    os << "Rectangle";
  }

  void Square::print(ostream& os) const
  {
    os << "Square";
  }

  void Ellipse::print(ostream& os) const
  {
    os << "Ellipse";
  }

  void Circle::print(ostream& os) const
  {
    os << "Circle";
  }

  ostream& operator<<(ostream& os, const Shape& s)
  {
    s.print(os);
    return os;
  }

}

#endif

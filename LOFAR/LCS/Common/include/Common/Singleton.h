//#  Singleton.h: Implementation of a Meyers singleton class.
//#
//#  Copyright (C) 2002-2004
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
//#  $Id: Singleton.h 9677 2006-11-22 21:18:08Z loose $

#ifndef LOFAR_COMMON_SINGLETON_H
#define LOFAR_COMMON_SINGLETON_H

// \file
// Implementation of a Meyers singleton class.

//# Includes

namespace LOFAR
{
  // \addtogroup Common
  // @{

  // Singleton implements the so-called Meyers singleton (see Item 26 in
  // <em>More Effective C++</em>, by Scott Meyers).
  //
  // \attention The Meyers singleton is \e not thread-safe. So, you can only
  // safely use this Singleton class <em>as long as</em> there is only one
  // thread running. Note that, in general, the static initialization and
  // destruction phases (i.e. before main() has started and after main() has
  // finished), are single threaded.
  //
  // \attention The order of destruction of static objects is
  // undetermined. This Singleton class therefore suffers from the so-called
  // "Dead Reference Problem"
  template<typename T>
  class Singleton
  {
  public:
    // Return a reference to the object \c T. The object \c T is created when
    // instance() is called for the first time.
    static T& instance();

  private:
    // @{
    // Do not allow construction, destruction, copy construction, and
    // assignment by a third party.
    Singleton();
    Singleton(const Singleton<T>&);
    Singleton<T>& operator=(const Singleton<T>&);
    ~Singleton();
    // @}
  };

  // @}

  template<typename T>
  T& Singleton<T>::instance()
  {
    static T obj;
    return obj;
  }

} // namespace LOFAR

#endif

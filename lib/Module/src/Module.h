/*
  Copyright (C) 2019 Axel Ruder

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MODULE_H_
#define MODULE_H_

#include <stdio.h>


class Module {

    public:
        virtual const char *getName();
        virtual void setup();
        virtual void afterSetup();
        virtual void loop();
        virtual void every10Milliseconds();
        virtual void every50Milliseconds();
        virtual void every100Milliseconds();
        virtual void everySecond();

};

#endif

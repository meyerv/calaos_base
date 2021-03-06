/******************************************************************************
**  Copyright (c) 2007-2014, Calaos. All Rights Reserved.
**
**  This file is part of Calaos Home.
**
**  Calaos Home is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 3 of the License, or
**  (at your option) any later version.
**
**  Calaos Home is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with Foobar; if not, write to the Free Software
**  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
******************************************************************************/
#include "ZibaseAnalogIn.h"
#include "Zibase.h"

using namespace Calaos;

ZibaseAnalogIn::ZibaseAnalogIn(Params &p):
                InputAnalog(p),
                port(0)
{
        host = get_param("host");
        Utils::from_string(get_param("port"), port);

        Zibase::Instance(host, port).sig_newframe.connect(sigc::mem_fun(*this, &ZibaseAnalogIn::valueUpdated));

        Utils::logger("input") << Priority::DEBUG << "ZibaseAnalogIn::ZibaseAnalogIn(" << get_param("id") << "): Ok" << log4cpp::eol;
}

ZibaseAnalogIn::~ZibaseAnalogIn()
{
        Utils::logger("input") << Priority::DEBUG << "ZibaseAnalogIn::~ZibaseAnalogIn(): Ok" << log4cpp::eol;
}

void ZibaseAnalogIn::valueUpdated(ZibaseInfoSensor *sensor)
{
/*
        TODO: get the new value from the sensor object
        the sensor obj can be for any sensor, we must also check if
        it's for us
 
        if (new_value != value)
        {
                value = new_value;
                emitChange();
        }
*/
}

void ZibaseAnalogIn::readValue()
{
        //don't do anything here, as we can't query the zibase sensor for a value,
        //the zibase will send us new values
}


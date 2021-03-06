/******************************************************************************
**  Copyright (c) 2007-2008, Calaos. All Rights Reserved.
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
#include <WebOutputLightRGB.h>
#include <OutputLightRGB.h>

using namespace Calaos;

WebOutputLightRGB::WebOutputLightRGB(Params &_p):
                OutputLightRGB(_p)
{
        Calaos::StartReadRules::Instance().addIO();
        Calaos::StartReadRules::Instance().addIO();
        Calaos::StartReadRules::Instance().addIO();

        Utils::logger("output") << Priority::DEBUG << "WebOutputLightRGB::WebOutputLightRGB(" << get_param("id") << "): Ok" << log4cpp::eol;
}

WebOutputLightRGB::~WebOutputLightRGB()
{
        Utils::logger("output") << Priority::DEBUG << "WebOutputLightRGB::~WebOutputLightRGB(): Ok" << log4cpp::eol;
}


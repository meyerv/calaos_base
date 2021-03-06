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
#include <WOVolet.h>
#include <IPC.h>

using namespace Calaos;

WOVolet::WOVolet(Params &p):
                OutputShutter(p),
                port(502)
{
        host = get_param("host");
        if (get_params().Exists("port"))
                Utils::from_string(get_param("port"), port);

        Utils::logger("output") << Priority::DEBUG << "WOVolet::WOVolet(" << get_param("id") << "): Ok" << log4cpp::eol;
}

WOVolet::~WOVolet()
{
        Utils::logger("output") << Priority::DEBUG << "WOVolet::~WOVolet(): Ok" << log4cpp::eol;
}

void WOVolet::readConfig()
{
        host = get_param("host");
        if (get_params().Exists("port"))
                Utils::from_string(get_param("port"), port);
        Utils::from_string(get_param("var_up"), up_address);
        Utils::from_string(get_param("var_down"), down_address);

        //handle knx and 841/849
        if (get_param("knx") == "true")
        {
                up_address += WAGO_KNX_START_ADDRESS;
                down_address += WAGO_KNX_START_ADDRESS;
        }
        if (get_param("wago_841") == "true" && get_param("knx") != "true")
        {
                up_address += WAGO_841_START_ADDRESS;
                down_address += WAGO_841_START_ADDRESS;
        }
}

void WOVolet::setOutputUp(bool enable)
{
        readConfig();
        WagoMap::Instance(host, port).write_single_bit((UWord)up_address, enable, sigc::mem_fun(*this, &WOVolet::WagoWriteCallback));
}

void WOVolet::setOutputDown(bool enable)
{
        readConfig();
        WagoMap::Instance(host, port).write_single_bit((UWord)down_address, enable, sigc::mem_fun(*this, &WOVolet::WagoWriteCallback));
}

void WOVolet::WagoWriteCallback(bool status, UWord address, bool value)
{
        if (!status)
        {
                Utils::logger("output") << Priority::ERROR << "WOVolet(" << get_param("id") << "): Failed to write value" << log4cpp::eol;
                return;
        }
}

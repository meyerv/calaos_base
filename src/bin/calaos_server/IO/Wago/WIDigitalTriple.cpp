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
#include <WIDigitalTriple.h>

using namespace Calaos;

WIDigitalTriple::WIDigitalTriple(Params &p):
                InputSwitchTriple(p),
                port(502)
{
        host = get_param("host");
        Utils::from_string(get_param("var"), address);
        if (get_params().Exists("port"))
                Utils::from_string(get_param("port"), port);

        iter = Utils::signal_wago.connect( sigc::mem_fun(this, &WIDigitalTriple::ReceiveFromWago) );
        Utils::logger("input") << Priority::DEBUG << "WIDigitalTriple::WIDigitalTriple(" << get_param("id") << "): Ok" << log4cpp::eol;
}

WIDigitalTriple::~WIDigitalTriple()
{
        iter->disconnect();
        Utils::logger("input") << Priority::DEBUG << "WIDigitalTriple::~WIDigitalTriple(): Ok" << log4cpp::eol;
}

void WIDigitalTriple::ReceiveFromWago(std::string ip, int addr, bool val, std::string intype)
{
        if (ip == host && addr == address)
        {
                if ((intype == "std" && get_param("knx") != "true") ||
                    (intype == "knx" && get_param("knx") == "true"))
                {
                        Utils::logger("input") << Priority::INFO << "WIDigitalTriple::ReceiveFromWago(): Got "
                                               << Utils::to_string(val) << " on " << intype << " input " << addr
                                               << log4cpp::eol;

                        udp_value = val;
                        hasChanged();
                }
        }
}

void WIDigitalTriple::WagoReadCallback(bool status, UWord addr, int nb, vector<bool> &values)
{
        if (!status)
        {
                Utils::logger("input") << Priority::ERROR << "WIDigitalTriple(" << get_param("id") << "): Failed to read value" << log4cpp::eol;
                return;
        }
}

bool WIDigitalTriple::readValue()
{
        host = get_param("host");
        Utils::from_string(get_param("var"), address);
        if (get_params().Exists("port"))
                Utils::from_string(get_param("port"), port);

        if (get_param("knx") != "true")
        {
                //Force to reconnect in case of disconnection
                //WagoMap::Instance(host, port).read_bits((UWord)address, 1, sigc::mem_fun(*this, &WIDigitalTriple::WagoReadCallback));
        }

        return udp_value;
}


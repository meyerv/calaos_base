/******************************************************************************
**  Copyright (c) 2006-2012, Calaos. All Rights Reserved.
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
**  along with Calaos; if not, write to the Free Software
**  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
******************************************************************************/
#ifndef AVRECEIVER_H
#define AVRECEIVER_H

#include <Calaos.h>
#include <EcoreTimer.h>
#include <Ecore.h>
#include <Ecore_Con.h>
#include <Input.h>
#include <Output.h>

namespace Calaos
{

typedef map<int ,string> AVRList;

class AVReceiver
{
        friend class AVRManager;

        protected:
                int ref_count;

                Params params;
                AVRList source_names;

                Ecore_Con_Server *econ;
                EcoreTimer *timer_con;

                bool isConnected;
                string recv_buffer;
                string host;
                int port;

                int volume_main, volume_zone2, volume_zone3;
                bool power_main, power_zone2, power_zone3;
                string display_text;
                int source_main, source_zone2, source_zone3;

                string command_suffix;

                Ecore_Event_Handler *ehandler_add;
                Ecore_Event_Handler *ehandler_del;
                Ecore_Event_Handler *ehandler_data;

                enum { AVR_CON_CHAR = 0, AVR_CON_BYTES };
                int connection_type;

                void timerConnReconnect();
                virtual void processMessage(string msg);
                virtual void processMessage(vector<char> msg);

                void sendRequest(string request);
                void sendRequest(vector<char> request);

                virtual void connectionEstablished() {}

                void dataGet(string data);

        public:
                AVReceiver(Params &p, int default_port, int connection_type = AVR_CON_CHAR);
                virtual ~AVReceiver();

                /* AVR specific functions */
                enum {
                        AVR_UNKNOWN,

                        /* AVR Zones */
                        AVR_ZONE_MAIN, AVR_ZONE_2, AVR_ZONE_3,

                        /* AVR Input sources */
                        //WARNING: After first release, don't move these value it will mess the stored rule's actions
                        //Instead, add new value to the end.
                        AVR_INPUT_BD, AVR_INPUT_DVD, AVR_INPUT_TVSAT, AVR_INPUT_DVRBDR,
                        AVR_INPUT_VIDEO_1, AVR_INPUT_VIDEO_2, AVR_INPUT_VIDEO_3, AVR_INPUT_VIDEO_4,
                        AVR_INPUT_VIDEO_5, AVR_INPUT_VIDEO_6, AVR_INPUT_VIDEO_7, AVR_INPUT_VIDEO_8,
                        AVR_INPUT_NETRADIO, AVR_INPUT_IPOD,
                        AVR_INPUT_HDMI_1, AVR_INPUT_HDMI_2, AVR_INPUT_HDMI_3, AVR_INPUT_HDMI_4,
                        AVR_INPUT_HDMI_5, AVR_INPUT_HDMI_6, AVR_INPUT_HDMI_7, AVR_INPUT_HDMI_8,
                        AVR_INPUT_CD, AVR_INPUT_CDRTAPE, AVR_INPUT_TUNER, AVR_INPUT_PHONO, AVR_INPUT_MULTIIN,
                        AVR_INPUT_APORT, AVR_INPUT_SIRIUS, AVR_INPUT_TV, AVR_INPUT_SAT, AVR_INPUT_GAME_1,
                        AVR_INPUT_GAME_2, AVR_INPUT_AUX, AVR_INPUT_DOCK, AVR_INPUT_HDRADIO,
                        AVR_INPUT_RHAPSODY, AVR_INPUT_NAPSTER, AVR_INPUT_PANDORA, AVR_INPUT_LASTFM,
                        AVR_INPUT_FLICKR, AVR_INPUT_FAV, AVR_INPUT_SERVER, AVR_INPUT_NETWORK,
                        AVR_INPUT_USB, AVR_INPUT_USB2, AVR_INPUT_USB3, AVR_INPUT_USB4,
                        AVR_INPUT_USB5, AVR_INPUT_USB6, AVR_INPUT_USB7, AVR_INPUT_USB8,
                        AVR_INPUT_VCR, AVR_INPUT_AUX1, AVR_INPUT_AUX2, AVR_INPUT_AUX3, AVR_INPUT_AUX4,
                        AVR_INPUT_AUX5, AVR_INPUT_AUX6, AVR_INPUT_AUX7, AVR_INPUT_AUX8,
                        AVR_INPUT_BLUETOOTH, AVR_INPUT_IPODUSB, AVR_INPUT_PC, AVR_INPUT_UAW 
                     };

                virtual void Power(bool on, int zone = 1) {}
                virtual bool getPower(int zone = 1);
                virtual void setVolume(int volume, int zone = 1) {}
                virtual int getVolume(int zone = 1);

                virtual AVRList getSources() { return source_names; }
                virtual void selectInputSource(int source, int zone = 1) {}
                virtual int getInputSource(int zone = 1);

                //return true if AVR can send his display status text
                virtual bool hasDisplay() { return false; }
                virtual string getDisplayText() { return display_text; }

                virtual void sendCustomCommand(string command) { sendRequest(command); }

                sigc::signal<void, string, string> state_changed_1; //zone 1
                sigc::signal<void, string, string> state_changed_2; //zone 2
                sigc::signal<void, string, string> state_changed_3; //zone 3

                /* This is private for C callbacks */
                void addConnection(Ecore_Con_Server *srv);
                void delConnection(Ecore_Con_Server *srv);
                void dataGet(Ecore_Con_Server *srv, void *data, int size);
};

//Input/Output for A/V Receiver devices
class IOAVReceiver: public Input, public Output, public sigc::trackable
{
        private:
                AVReceiver *receiver;
                int zone;

                void statusChanged(string param, string value);

        public:
                IOAVReceiver(Params &p);
                virtual ~IOAVReceiver();

                /* Input/Output functions */
                virtual DATA_TYPE get_type() { return TSTRING; }

                virtual void set_param(std::string opt, std::string val)
                        { Input::set_param(opt, val); }
                virtual std::string get_param(std::string opt)
                        { return Input::get_param(opt); }
                virtual Params &get_params()
                        { return Input::get_params(); }

                virtual map<string, string> query_param(string key);

                //Input
                virtual string get_value_string();
                virtual map<string, string> get_all_values_string();
                virtual void force_input_string(string v);

                //Output
                virtual bool set_value(string val);

                virtual bool LoadFromXml(TiXmlElement *node)
                        { return false; }
                virtual bool SaveToXml(TiXmlElement *node);
};

}

#endif // AVRECEIVER_H

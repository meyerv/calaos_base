/******************************************************************************
**  Copyright (c) 2006-2011, Calaos. All Rights Reserved.
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
#ifndef IOSCENARIOHOMEVIEW_H
#define IOSCENARIOHOMEVIEW_H

#include <Utils.h>
#include <IOView.h>

using namespace Utils;

class IOScenarioHomeView: public IOView
{
        private:
                void clickScenario(void *data, Evas_Object *edje_object, string emission, string source);

                string state;

                void clickFlashButton_cb();

        public:
                IOScenarioHomeView(Evas *evas, Evas_Object *parent, IOBase *io);
                virtual ~IOScenarioHomeView();

                virtual void initView();
                virtual void updateView();
};

#endif // IOSCENARIOHOMEVIEW_H

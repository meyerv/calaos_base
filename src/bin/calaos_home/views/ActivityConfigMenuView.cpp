/******************************************************************************
**  Copyright (c) 2006-2013, Calaos. All Rights Reserved.
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
#include "ActivityConfigMenuView.h"
#include "GengridItemConfig.h"

ActivityConfigMenuView::ActivityConfigMenuView(Evas *_e, Evas_Object *_parent):
        ActivityView(_e, _parent, "calaos/config/menu")
{

    setPartText("tab1.text", _("Resume"));
    setPartText("tab1.text.detail", _("Resume of: <light_blue>My Home</light_blue><br><small>Informations about your Calaos System</small"));
    setPartText("tab2.text", _("Our Partners"));
    setPartText("tab2.text.detail", _("<light_blue>Calaos</light_blue> partners<br><small>List of Calaos partners</small>"));
    setPartText("tab3.text", _("About"));
    setPartText("tab3.text.detail", _("About : <light_blue>Calaos products</light_blue><br><small>Touchscreen solutions.</small>"));

    grid = elm_gengrid_add(_parent);

    elm_gengrid_select_mode_set(grid, ELM_OBJECT_SELECT_MODE_ALWAYS);

    elm_object_style_set(grid, "calaos");
    evas_object_show(grid);

    elm_gengrid_group_item_size_set(grid, 200, 120);

    naviframe = elm_naviframe_add(parent);
    evas_object_show(naviframe);
    Swallow(naviframe, "naviframe.swallow");

    GengridItemConfig *item;

    item = new GengridItemConfig(evas, grid, _("Date and clock"), "clock");
    item->Append(grid);
    item->item_selected.connect([=](void *data)
    {
        cout << "click on item clock!" << endl;
        menu_item_clicked.emit("clock");
    });

    item = new GengridItemConfig(evas, grid, _("Password"), "security");
    item->Append(grid);
    item->item_selected.connect([=](void *data)
    {
        cout << "click on item security!" << endl;
        menu_item_clicked.emit("security");
    });

    item = new GengridItemConfig(evas, grid, _("Energy saving"), "veille");
    item->Append(grid);
    item->item_selected.connect([=](void *data)
    {
        cout << "click on item veille!" << endl;
        menu_item_clicked.emit("screensaver");
    });

    elm_naviframe_item_push(naviframe, NULL, NULL, NULL, grid, "calaos");

}

ActivityConfigMenuView::~ActivityConfigMenuView()
{
    evas_object_del(grid);
}

void ActivityConfigMenuView::resetView()
{
}


/*
 * Copyright (C) 2014 Endless Mobile
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
 * 02111-1307, USA.
 *
 * Written by:
 *     Jasper St. Pierre <jstpierre@mecheye.net>
 */

#pragma once

#include <node.h>
#include <girepository.h>
#include <glib-object.h>

namespace GNodeJS {

v8::Handle<v8::Function> MakeClass(v8::Isolate *isolate, GIBaseInfo *info);

v8::Handle<v8::Value> WrapperFromGObject(v8::Isolate *isolate, GObject *object);
GObject * GObjectFromWrapper(v8::Handle<v8::Value> value);

};

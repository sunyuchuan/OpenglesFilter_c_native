/*
* Created by sunyc on 2018-8-21.
* Abstraction and packaging about filters factory.
*/

#ifndef XM_FILTER_FACTORY_H
#define XM_FILTER_FACTORY_H
#include "ijksdl/ijksdl_vout.h"
#include "filter/xm_filter.h"
#include "time_filter/time_filter.h"
#include "base_filter/base_filter.h"

XM_Filter *XM_Filter_Factory_createBaseFilters(SDL_VoutOverlay *overlay);
XM_Filter *XM_Filter_Factory_createTimeFilters(SDL_VoutOverlay *overlay);
#endif

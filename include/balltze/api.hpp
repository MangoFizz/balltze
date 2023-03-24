// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API_HPP
#define BALLTZE_API_HPP

#ifdef BALLTZE_EXPORTS
#define BALLTZE_API __declspec(dllexport)
#else
#define BALLTZE_API __declspec(dllimport)
#define BALLTZE_PLUGIN_API extern "C" __declspec(dllexport)
#endif

#endif


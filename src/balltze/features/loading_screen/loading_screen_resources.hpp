// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE__FEATURES__LOADING_SCREEN__LOADING_SCREEN_RESOURCES_HPP
#define BALLTZE__FEATURES__LOADING_SCREEN__LOADING_SCREEN_RESOURCES_HPP

#include <windows.h>
#include <d3d9.h>

namespace Balltze::Features {
    HRESULT load_loading_screen_background_texture(IDirect3DDevice9 *device, IDirect3DTexture9 **texture);
    HRESULT load_loading_screen_shader(IDirect3DDevice9 *device, IDirect3DPixelShader9 **shader);
}

#endif

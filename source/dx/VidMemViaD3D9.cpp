//----------------------------------------------------------------------------
// File: VidMemViaD3D9.cpp
//
// This method queries D3D9 for the amount of available texture memory. On 
// Windows Vista, this number is typically the dedicated video memory plus 
// the shared system memory minus the amount of memory in use by textures 
// and render targets. 
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License (MIT).
//-----------------------------------------------------------------------------

#define INITGUID
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <d3d9.h>
#include "plugin.h"


//-----------------------------------------------------------------------------
// Defines, and constants
//-----------------------------------------------------------------------------
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=nullptr; } }
#endif

HRESULT GetVideoMemoryViaD3D9(HMONITOR hMonitor, UINT* pdwAvailableTextureMem) {
    *pdwAvailableTextureMem = 0;

    if (IDirect3DDevice9* pd3dDevice = (IDirect3DDevice9*)GetD3DDevice()) {
        *pdwAvailableTextureMem = pd3dDevice->GetAvailableTextureMem();
        return S_OK;
    }

    return E_FAIL;
}


HRESULT GetHMonitorFromD3D9Device(IDirect3DDevice9* pd3dDevice, HMONITOR hMonitor) {
    D3DDEVICE_CREATION_PARAMETERS cp;
    hMonitor = nullptr;
    bool bFound = false;
    if (SUCCEEDED(pd3dDevice->GetCreationParameters(&cp))) {
        IDirect3D9* pD3D = nullptr;
        if (SUCCEEDED(pd3dDevice->GetDirect3D(&pD3D))) {
            hMonitor = pD3D->GetAdapterMonitor(cp.AdapterOrdinal);
            bFound = true;
        }
        pD3D->Release();
    }

    if (bFound)
        return S_OK;
    else
        return E_FAIL;
}
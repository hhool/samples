/******************************************************************************\
Copyright (c) 2005-2016, Intel Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

This sample was distributed or derived from the Intel's Media Samples package.
The original version of this sample may be obtained from https://software.intel.com/en-us/intel-media-server-studio
or https://software.intel.com/en-us/media-client-solutions-support.
\**********************************************************************************/

#ifndef __SAMPLE_VPP_UTILS_H
#define __SAMPLE_VPP_UTILS_H

/* ************************************************************************* */

#include "sample_defs.h"

#if D3D_SURFACES_SUPPORT
#pragma warning(disable : 4201)
#include <d3d9.h>
#include <dxva2api.h>
#include <windows.h>
#endif

#if ! defined(_WIN32) && ! defined(_WIN64)
struct BITMAPFILEHEADER
{
  unsigned short  bfType;
  unsigned int bfSize;
  unsigned short  bfReserved1;
  unsigned short  bfReserved2;
  unsigned int bfOffBits;
};
struct BITMAPINFOHEADER
{
  unsigned int biSize;
  unsigned int  biWidth;
  unsigned int  biHeight;
  unsigned short  biPlanes;
  unsigned short  biBitCount;
  unsigned int biCompression;
  unsigned int biSizeImage;
  unsigned int  biXPelsPerMeter;
  unsigned int  biYPelsPerMeter;
  unsigned int biClrUsed;
  unsigned int biClrImportant;
};

#define BI_RGB   0L
#endif

#include <stdio.h>

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cctype>

#include "mfxvideo.h"
#include "mfxvideo++.h"
#include "sample_utils.h"
#include "base_allocator.h"
#include "mfxcamera.h"

//#define CONVERT_TO_LSB
#define SHIFT_OUT_TO_LSB

//#define CAM_SAMPLE_PERF

/* ************************************************************************* */
#if 0
#define camera_printf printf
#define camera_msdk_printf msdk_printf
#define camera_fflush fflush
#else
#define camera_printf
#define camera_msdk_printf
#define camera_fflush
#endif

#define VPP_IN       (0)
#define VPP_OUT      (1)

#define MFX_MAX_32U   ( 0xFFFFFFFF )

#define NOT_INIT_VALUE (0xFFF7)


template<>struct mfx_ext_buffer_id<mfxExtCamGammaCorrection>{
    enum {id = MFX_EXTBUF_CAM_GAMMA_CORRECTION};
};

template<>struct mfx_ext_buffer_id<mfxExtCamBlackLevelCorrection>{
    enum {id = MFX_EXTBUF_CAM_BLACK_LEVEL_CORRECTION};
};

template<>struct mfx_ext_buffer_id<mfxExtCamWhiteBalance>{
    enum {id = MFX_EXTBUF_CAM_WHITE_BALANCE};
};

template<>struct mfx_ext_buffer_id<mfxExtCamColorCorrection3x3>{
    enum {id = MFX_EXTBUF_CAM_COLOR_CORRECTION_3X3};
};

template<>struct mfx_ext_buffer_id<mfxExtCamVignetteCorrection>{
    enum {id = MFX_EXTBUF_CAM_VIGNETTE_CORRECTION};
};
// we introduce new macros without error message (returned status only)
// it allows to remove final error message due to EOF
#define IOSTREAM_CHECK_NOT_EQUAL(P, X, ERR)          {if ((X) != (P)) {return ERR;}}
#define IOSTREAM_CHECK_NOT_EQUAL_SAFE(P, X, ERR,CLEANUP)          {if ((X) != (P)) {CLEANUP;return ERR;}}

#define CAM_SAMPLE_ASYNC_DEPTH 4
#define CAM_SAMPLE_NUM_BMP_FILES 20

enum AccelType {
    D3D9   = 0x01,
    D3D11  = 0x02
};

enum MemoryType {
    SYSTEM   = 0x01,
    VIDEO    = 0x02
};

enum MemType {
    SYSTEM_MEMORY = 0x00,
    D3D9_MEMORY   = 0x01,
    D3D11_MEMORY  = 0x02,
    UNDEFINED_MEMORY = 0x1000
};


typedef struct _ownFrameInfo
{
  mfxU32  nWidth;
  mfxU32  nHeight;
  // ROI
  mfxU32  CropX;
  mfxU32  CropY;
  mfxU32  CropW;
  mfxU32  CropH;

  mfxU32 FourCC;
  mfxF64 dFrameRate;

} sOwnFrameInfo;


typedef struct _resetParams
{
    msdk_char  strSrcFile[MSDK_MAX_FILENAME_LEN];
    msdk_char  strDstFile[MSDK_MAX_FILENAME_LEN];
    mfxU32 width;
    mfxU32 height;
    mfxU16 cropW;
    mfxU16 cropH;
    mfxU16 cropX;
    mfxU16 cropY;
    mfxU32 bayerType;

    bool   bDenoise;
    mfxU16 denoiseThreshold;

    bool   bHP;
    mfxU16 hp_diff;
    mfxU16 hp_num;

    bool   bBlackLevel;
    mfxU16 black_level_B;
    mfxU16 black_level_G0;
    mfxU16 black_level_G1;
    mfxU16 black_level_R;

    bool   bWhiteBalance;
    mfxF64 white_balance_B;
    mfxF64 white_balance_G0;
    mfxF64 white_balance_G1;
    mfxF64 white_balance_R;

    bool   bCCM;
    mfxF64 CCM[3][3];

    bool   bVignette;
    msdk_char  strVignetteMaskFile[MSDK_MAX_FILENAME_LEN];
    _resetParams()
    {
        bHP           = false;
        bBlackLevel   = false;
        bWhiteBalance = false;
        bCCM          = false;
        bDenoise      = false;
        bVignette     = false;
        cropX = cropY = cropW = cropH = 0;
    }
} sResetParams;

struct sInputParams
{

    sOwnFrameInfo frameInfo[2];  // [0] - in, [1] - out

    mfxU32  bayerType;
    bool    bOutput; // if renderer is enabled, possibly no need in output file
    mfxI32  maxNumBmpFiles;
    AccelType   accelType;
    MemoryType  memTypeIn;
    MemoryType  memTypeOut;
    mfxU32  CameraPluginVersion;
    bool    bRendering; // true if d3d rendering is in use
    mfxI32  asyncDepth;
    mfxU32  bitDepth;
    mfxI32  alphaValue;

    mfxU32  nWallCell;
    mfxU32  nWallW;//number of windows located in each row
    mfxU32  nWallH;//number of windows located in each column
    mfxU32  nWallMonitor;//monitor id, 0,1,.. etc
    mfxU32  nWallFPS;//rendering limited by certain fps
    bool    bWallNoTitle;//whether to show title for each window with fps value
    mfxU32  nWallTimeout; //timeout for -wall option

    bool   bHP;
    mfxU16 hp_diff;
    mfxU16 hp_num;

    bool   bGamma;
    bool   b3DLUTGamma;
    bool   bExternalGammaLUT;
    mfxU16 gamma_point[64];
    mfxU16 gamma_corrected[64];
    mfxF64 gamma_value;
    mfxU16 gamma_mode;

    bool   bBlackLevel;
    mfxU16 black_level_B;
    mfxU16 black_level_G0;
    mfxU16 black_level_G1;
    mfxU16 black_level_R;

    bool   bWhiteBalance;
    mfxF64 white_balance_B;
    mfxF64 white_balance_G0;
    mfxF64 white_balance_G1;
    mfxF64 white_balance_R;

    bool   bLens;
    mfxF32 lens_aR;
    mfxF32 lens_bR;
    mfxF32 lens_cR;
    mfxF32 lens_dR;
    mfxF32 lens_aG;
    mfxF32 lens_bG;
    mfxF32 lens_cG;
    mfxF32 lens_dG;
    mfxF32 lens_aB;
    mfxF32 lens_bB;
    mfxF32 lens_cB;
    mfxF32 lens_dB;

    bool   bCCM;
    mfxF64 CCM[3][3];

    bool   bVignette;
    msdk_char  strVignetteMaskFile[MSDK_MAX_FILENAME_LEN];
    bool bBayerDenoise;
    mfxU16 denoiseThreshold;

    mfxU32 nFramesToProceed;

    msdk_char  strSrcFile[MSDK_MAX_FILENAME_LEN];
    msdk_char  strDstFile[MSDK_MAX_FILENAME_LEN];
    msdk_char  strPluginPath[MSDK_MAX_FILENAME_LEN];

    std::vector<sResetParams> resetParams;
    mfxU32 resetInterval;

    bool bDoPadding;

    bool b3DLUT;

    sInputParams()
    {
        MSDK_ZERO_MEMORY(*this);
        CameraPluginVersion = 1;
        bayerType     = MFX_CAM_BAYER_RGGB;
        frameInfo[VPP_IN].nWidth = 4096;
        frameInfo[VPP_IN].nHeight = 2160;
        frameInfo[VPP_IN].CropH = frameInfo[VPP_IN].CropW = frameInfo[VPP_OUT].CropH = frameInfo[VPP_OUT].CropW = NOT_INIT_VALUE;
        frameInfo[VPP_IN].dFrameRate = frameInfo[VPP_OUT].dFrameRate = 24.0;
        frameInfo[VPP_OUT].FourCC = MFX_FOURCC_RGB4;
        asyncDepth = CAM_SAMPLE_ASYNC_DEPTH;
        maxNumBmpFiles = CAM_SAMPLE_NUM_BMP_FILES;
        memTypeIn  = SYSTEM;
        memTypeOut = VIDEO;
        accelType  = D3D9;
        bitDepth = 10;
        bGamma = true;
        b3DLUTGamma   = false;
        bHP           = false;
        bBlackLevel   = false;
        bWhiteBalance = false;
        bCCM          = false;
        bBayerDenoise = false;
        bVignette     = false;
        bLens         = false;
        b3DLUT        = false;

        alphaValue = -1;
        resetInterval = 7;
        bExternalGammaLUT = false;
    }
};

struct sMemoryAllocator
{
  MFXFrameAllocator*     pMfxAllocator;
  mfxAllocatorParams*    pAllocatorParams;
  mfxFrameSurface1**     pSurfaces;
  mfxFrameAllocResponse* response;
};

class CRawVideoReader
{
public :

  CRawVideoReader();
  virtual ~CRawVideoReader();

  virtual void    Close();
  virtual mfxStatus  Init(sInputParams *pParams);
  virtual mfxStatus  LoadNextFrame(mfxFrameData* pData, mfxFrameInfo* pInfo, mfxU32 bayerType);
  virtual void  SetStartFileNumber(mfxI32);

protected:
  mfxStatus  LoadNextFrameSingle    (mfxFrameData* pData, mfxFrameInfo* pInfo, mfxU32 bayerType);
  mfxStatus  LoadNextFrameSequential(mfxFrameData* pData, mfxFrameInfo* pInfo, mfxU32 bayerType);

  FILE*       m_fSrc;
  msdk_char m_FileNameBase[MSDK_MAX_FILENAME_LEN];
  mfxU32   m_FileNum;
#ifdef CONVERT_TO_LSB
  mfxU16   *m_pPaddingBuffer;
  mfxU32    m_paddingBufSize;
#endif
  bool     m_DoPadding;
  bool     m_bSingleFileMode;
  mfxU32   m_Width;
  mfxU32   m_Height;

};

class CBmpWriter
{
public :

    CBmpWriter();
    //~CBmpWriter() {};

    mfxStatus Init(const msdk_char *strFileNameBase, mfxU32 width, mfxU32 height, mfxI32 maxNumFilesToCreate = -1);
    mfxStatus  WriteFrame(mfxFrameData* pData, const msdk_char *fileExt, mfxFrameInfo* pInfo);

protected:
    msdk_char    m_FileNameBase[MSDK_MAX_FILENAME_LEN];
    mfxI32       m_FileNum;
    mfxI32       m_maxNumFilesToCreate;

    BITMAPFILEHEADER m_bfh;
    BITMAPINFOHEADER m_bih;

};


class CRawVideoWriter
{
public :

    CRawVideoWriter() {m_FileNum = 0;}; //m_pShiftBuffer = 0;};
  //~CRawVideoWriter();

  mfxStatus  Init(sInputParams *pParams);
  mfxStatus  WriteFrame(mfxFrameData* pData, const msdk_char *fileExt, mfxFrameInfo* pInfo);

protected:
    msdk_char    m_FileNameBase[MSDK_MAX_FILENAME_LEN];
    mfxI32       m_FileNum;
    mfxI32       m_maxNumFilesToCreate;
    //mfxU16       *m_pShiftBuffer;
    //mfxU32       m_shiftBufSize;
};


/* ******************************************************************* */
/*                        service functions                            */
/* ******************************************************************* */

mfxStatus GetFreeSurface(mfxFrameSurface1* pSurfacesPool, mfxU16 nPoolSize, mfxFrameSurface1** ppSurface);

mfxStatus InitSurfaces(sMemoryAllocator* pAllocator, mfxFrameAllocRequest* pRequest, mfxFrameInfo* pInfo, bool bUsedAsExternalAllocator);

void ReleaseSurface(mfxFrameSurface1* pSurface);

msdk_char* FourCC2Str( mfxU32 FourCC );

#endif /* __SAMPLE_VPP_UTILS_H */

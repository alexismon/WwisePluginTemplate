/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2023 Audiokinetic Inc.
*******************************************************************************/

#include "WwisePluginTemplateFX.h"
#include "../WwisePluginTemplateConfig.h"

#include <AK/AkWwiseSDKVersion.h>

AK::IAkPlugin* CreateWwisePluginTemplateFX(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, WwisePluginTemplateFX());
}

AK::IAkPluginParam* CreateWwisePluginTemplateFXParams(AK::IAkPluginMemAlloc* in_pAllocator)
{
    return AK_PLUGIN_NEW(in_pAllocator, WwisePluginTemplateFXParams());
}

AK_IMPLEMENT_PLUGIN_FACTORY(WwisePluginTemplateFX, AkPluginTypeEffect, WwisePluginTemplateConfig::CompanyID, WwisePluginTemplateConfig::PluginID)

WwisePluginTemplateFX::WwisePluginTemplateFX()
    : m_pParams(nullptr)
    , m_pAllocator(nullptr)
    , m_pContext(nullptr)
{
}

WwisePluginTemplateFX::~WwisePluginTemplateFX()
{
}

AKRESULT WwisePluginTemplateFX::Init(AK::IAkPluginMemAlloc* in_pAllocator, AK::IAkEffectPluginContext* in_pContext, AK::IAkPluginParam* in_pParams, AkAudioFormat& in_rFormat)
{
    m_pParams = (WwisePluginTemplateFXParams*)in_pParams;
    m_pAllocator = in_pAllocator;
    m_pContext = in_pContext;

    return AK_Success;
}

AKRESULT WwisePluginTemplateFX::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    AK_PLUGIN_DELETE(in_pAllocator, this);
    return AK_Success;
}

AKRESULT WwisePluginTemplateFX::Reset()
{
    return AK_Success;
}

AKRESULT WwisePluginTemplateFX::GetPluginInfo(AkPluginInfo& out_rPluginInfo)
{
    out_rPluginInfo.eType = AkPluginTypeEffect;
    out_rPluginInfo.bIsInPlace = true;
	out_rPluginInfo.bCanProcessObjects = false;
    out_rPluginInfo.uBuildVersion = AK_WWISESDK_VERSION_COMBINED;
    return AK_Success;
}

#define AK_LINTODB( __lin__ ) (log10f(__lin__) * 20.f)

void WwisePluginTemplateFX::Execute(AkAudioBuffer* io_pBuffer)
{
    const AkUInt32 uNumChannels = io_pBuffer->NumChannels();

    // Monitor Data
    AkReal32 rmsBefore = 0.f;
    AkReal32 rmsAfter = 0.f;

    AkUInt16 uFramesProcessed;
    for (AkUInt32 i = 0; i < uNumChannels; ++i)
    {
        AkReal32* AK_RESTRICT pBuf = (AkReal32* AK_RESTRICT)io_pBuffer->GetChannel(i);

        uFramesProcessed = 0;
        while (uFramesProcessed < io_pBuffer->uValidFrames)
        {
            // Execute DSP in-place here
        #ifndef AK_OPTIMIZED
            if (m_pContext->CanPostMonitorData())
                rmsBefore += powf(pBuf[uFramesProcessed], 2);
        #endif

            pBuf[uFramesProcessed] = pBuf [uFramesProcessed] * AK_DBTOLIN(m_pParams->RTPC.fPlaceholder);

        #ifndef AK_OPTIMIZED
            if (m_pContext->CanPostMonitorData())
                rmsAfter += powf(pBuf[uFramesProcessed], 2);
        #endif
        
            ++uFramesProcessed;
        }
    }

#ifndef AK_OPTIMIZED
    if (m_pContext->CanPostMonitorData())
    {
        // RMS = Root of the Mean of the Squares
        //       sqrt( (1/n) * sum_0-n( (x_1)^2, ..., (x_n)^2 ) )

        rmsBefore /= (uNumChannels * io_pBuffer->uValidFrames);
        rmsBefore = sqrtf(rmsBefore);

        rmsAfter /= (uNumChannels * io_pBuffer->uValidFrames);
        rmsAfter = sqrtf(rmsAfter);

        AkReal32 monitorData[2] = { AK_LINTODB(rmsBefore), AK_LINTODB(rmsAfter) };
        m_pContext->PostMonitorData((void*)monitorData, sizeof(monitorData));
    }
#endif
}

AKRESULT WwisePluginTemplateFX::TimeSkip(AkUInt32 in_uFrames)
{
    return AK_DataReady;
}

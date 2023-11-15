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

#pragma once

#include "../WwisePluginTemplatePlugin.h"
#include "../resource.h"
#include <shellapi.h>
#include <string>

#include "../../JuceLibraryCode/JuceHeader.h"

class MainComponent;

class WwisePluginTemplatePluginGUI final
	: public AK::Wwise::Plugin::GUIWindows,
      public AK::Wwise::Plugin::Notifications::Monitor
{
public:
	WwisePluginTemplatePluginGUI();
    ~WwisePluginTemplatePluginGUI();

    //=== Frontend Methods ===

    /// Retrieve the handle holding resources for the module containing the plug-in instance
    HINSTANCE GetResourceHandle() const override;

    /// Retrieve the dialog ID and its table mapping control ID to properties
    bool GetDialog(
        AK::Wwise::Plugin::eDialog in_eDialog,				///< The dialog type
        UINT & out_uiDialogID,			///< The returned resource ID of the dialog
        AK::Wwise::Plugin::PopulateTableItem *& out_pTable	///< The returned table of property-control bindings (can be NULL)
    ) const override;

    /// Window message handler
    bool WindowProc(
        AK::Wwise::Plugin::eDialog in_eDialog,		///< The dialog type
        HWND in_hWnd,			///< The window handle of the dialog
        UINT in_message,		///< The incoming message. This is a standard Windows message ID (ex. WM_PAINT).
        WPARAM in_wParam,		///< The WPARAM of the message (see MSDN)
        LPARAM in_lParam,		///< The LPARAM of the message (see MSDN)
        LRESULT & out_lResult 	///< The returned value if the message has been processed (it is only considered if the method also returns True)
    ) override;

    /// Function called when user clicked the help [?] button
    bool Help(
        HWND in_hWnd,					///< The handle of the dialog
        AK::Wwise::Plugin::eDialog in_eDialog,				///< The dialog type
        const char * in_szLanguageCode		///< The language code in ISO639-1
    ) const override;

    /// Function called when monitor data is posted for our plug-in type
    void NotifyMonitorData(
        AkTimeMs in_iTimeStamp,
        const AK::Wwise::Plugin::MonitorData* in_pMonitorDataArray,
        unsigned int in_uMonitorDataArraySize,
        bool in_bIsRealtime
    ) override;

private:
    HWND m_hwnd;

    std::unique_ptr<MainComponent> mainComponent;
};

//==============================================================================
class MainComponent : public juce::Component
{
public:
    MainComponent();

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
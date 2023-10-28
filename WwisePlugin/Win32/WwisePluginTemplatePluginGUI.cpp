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

#include "WwisePluginTemplatePluginGUI.h"

WwisePluginTemplatePluginGUI::WwisePluginTemplatePluginGUI()
{
}

// Acquire the module instance from the Microsoft linker
extern "C" IMAGE_DOS_HEADER __ImageBase;

HINSTANCE WwisePluginTemplatePluginGUI::GetResourceHandle() const
{
    return ((HINSTANCE)&__ImageBase);

    // OR using MFC:
    //AFX_MANAGE_STATE( AfxGetStaticModuleState() );
    //return AfxGetStaticModuleState()->m_hCurrentResourceHandle;
}

// These macros generate a table named "WoaGainProperties" to pass to GetDialog
// See https://www.audiokinetic.com/library/edge/?source=SDK&id=wwiseplugin_dialog_guide.html#wwiseplugin_dialog_guide_poptable
//
// The preprocessor turns the code below into:
// AK::Wwise::PopulateTableItem WoaGainProperties = {
//    {IDC_GAIN_SLIDER, L"Dummy"},
//    {0, NULL}
// };

constexpr auto propertyKey = u8"Placeholder";
 
AK_WWISE_PLUGIN_GUI_WINDOWS_BEGIN_POPULATE_TABLE(PropertyTable)
    AK_WWISE_PLUGIN_GUI_WINDOWS_POP_ITEM(
      IDC_FADER,    /* < ID of the Win32 control in resource.h and WoaGain.rc */
      propertyKey   /* < Property Name in WoaGain.xml */
      )       

AK_WWISE_PLUGIN_GUI_WINDOWS_END_POPULATE_TABLE()

// Return true = Custom GUI
// Return false = Generated GUI
bool WwisePluginTemplatePluginGUI::GetDialog(
    AK::Wwise::Plugin::eDialog in_eDialog,				///< The dialog type
    UINT & out_uiDialogID,			///< The returned resource ID of the dialog
    AK::Wwise::Plugin::PopulateTableItem *& out_pTable	///< The returned table of property-control bindings (can be NULL)
) const
{
    // Which dialog type is being requested?
    switch (in_eDialog)
    {
        // Plug-in Settings: Available to all plug-ins
        case AK::Wwise::Plugin::SettingsDialog:
        {
            out_uiDialogID = IDD_PLUGIN_PAGE;
            out_pTable = PropertyTable;
            return true;
        }
        // Contents Editor: Only available to source plug-ins
        case AK::Wwise::Plugin::ContentsEditorDialog:
        default:
        {
            return false;
        }
    }
}

// Window message handler

/*

bool VoluminousPlugin::WindowProc (AK::Wwise::Plugin::eDialog dialog, HWND nativeHandle, UINT message, WPARAM wparam, LPARAM lparam, LRESULT& result)
{
    switch (message)
    {
    case WM_INITDIALOG:
    {
        container.reset(new AudioProcessorEditorContainer(*this));
        container->setOpaque (true);
        container->setVisible (true);
        container->addToDesktop (0, nativeHandle);

        const auto* editor = dynamic_cast<VoluminousAudioProcessorEditor*> (container->getEditor());

        proxy.reset(new AudioProcessorPropertySetProxy(&m_propertySet, { editor->masterDial }));
        addListener (proxy.get());
        proxy->audioProcessorAttached (this);
    }
    break;
    case WM_DESTROY:
    {
        removeListener (proxy.get());
        proxy = nullptr;

        container->removeFromDesktop();
        container = nullptr;
    }
    break;
    }

    result = 0;
    return false;
}


*/
// See https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms633573(v=vs.85)
// Standard window function allowing the user to intercept whatever message is of interest when implementing UI behavior.
bool WwisePluginTemplatePluginGUI::WindowProc(
    AK::Wwise::Plugin::eDialog in_eDialog,
    HWND in_hWnd,
    UINT in_message,
    WPARAM in_wParam,
    LPARAM in_lParam,
    LRESULT& out_lResult)
{
    // Effects only support the dialog type "SettingsDialog"
    if (in_eDialog != AK::Wwise::Plugin::SettingsDialog)
        return false;

    bool messageWasHandled = true;

    switch (in_message)
    {
        // Sent to the dialog box procedure immediately before a dialog box is displayed.
        // Dialog box procedures typically use this message to initialize controls and carry out any other
        // initialization tasks that affect the appearance of the dialog box.
        case WM_INITDIALOG:
        {
            m_hwnd = in_hWnd;

            // return TRUE to direct the system to set the keyboard focus to the control specified by wParam
            // return FALSE to prevent the system from setting the default keyboard focus
            out_lResult = FALSE;
            break;
        }
        // Sent when a window is being destroyed.
        // It is sent to the window procedure of the window being destroyed after the window is removed from the screen.
        case WM_DESTROY:
        {
            m_hwnd = NULL;

            // If an application processes this message, it should return zero.
            out_lResult = 0;
            break;
        }
        default:
        {
            messageWasHandled = false;
            break;
        }
    }

    return messageWasHandled;
}

// Function called when user clicked the help [?] button
// See https://www.audiokinetic.com/library/edge/?source=SDK&id=plugin_dll.html#wwiseplugin_help
bool WwisePluginTemplatePluginGUI::Help(
    HWND in_hWnd,					///< The handle of the dialog
    AK::Wwise::Plugin::eDialog in_eDialog,				///< The dialog type
    const char *in_szLanguageCode		///< The language code in ISO639-1
) const
{
    if (in_eDialog == AK::Wwise::Plugin::SettingsDialog && strlen(in_szLanguageCode) == 2)
    {
        wchar_t url[128] = { 0 };
        wsprintf(url, L"https://www.audiokinetic.com/%s/library/edge/?source=SDK&id=plugin_dll.html", in_szLanguageCode);
        // Let Windows open the URL through the default application
        ::ShellExecute(0, 0, url, 0, 0 , SW_SHOW);
        return true;
    }
    return false;
}

void WwisePluginTemplatePluginGUI::NotifyMonitorData(
    AkTimeMs in_iTimeStamp,
    const AK::Wwise::Plugin::MonitorData* in_pMonitorDataArray,
    unsigned int in_uMonitorDataArraySize,
    bool in_bIsRealtime)
{
    // Below are pedantic validation for demonstration purposes

    if (m_hwnd != NULL &&          // The dialog exists
        in_pMonitorDataArray != nullptr &&     // The data payload is not null
        in_uMonitorDataArraySize >= 1       // Received data from at least 1 instance
        // && in_bNeedsByteSwap == false // Only handle little-endian to little-endian
    ) {
        // TODO: This handles only a single instance!
        //       We _must_ handle all instances (in_uDataSize = number of instances)

        if (in_pMonitorDataArray->pData != nullptr &&               // The monitor data is not null
            in_pMonitorDataArray->uDataSize == sizeof(AkReal32) * 2 // 
        ) {
            AkReal32* serializedData = (AkReal32*)in_pMonitorDataArray->pData;

            HWND inputLvlLabel = ::GetDlgItem(m_hwnd, IDC_INPUT_LVL);
            ::SetWindowTextW(inputLvlLabel, std::to_wstring(serializedData[0]).c_str());

            HWND outputLvlLabel = ::GetDlgItem(m_hwnd, IDC_OUTPUT_LVL);
            ::SetWindowTextW(outputLvlLabel, std::to_wstring(serializedData[1]).c_str());
        }
    }
}

ADD_AUDIOPLUGIN_CLASS_TO_CONTAINER(
    WwisePluginTemplate,            // Name of the plug-in container for this shared library
    WwisePluginTemplatePluginGUI,   // Authoring plug-in class to add to the plug-in container
    WwisePluginTemplateFX           // Corresponding Sound Engine plug-in class
);

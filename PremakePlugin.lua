--[[----------------------------------------------------------------------------
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
------------------------------------------------------------------------------]]

if not _AK_PREMAKE then
    error('You must use the custom Premake5 scripts by adding the following parameter: --scripts="Scripts\\Premake"', 1)
end

local Plugin = {}
Plugin.name = "WwisePluginTemplate"
Plugin.factoryheader = "../SoundEnginePlugin/WwisePluginTemplateFXFactory.h"
Plugin.sdk = {}
Plugin.sdk.static = {}
Plugin.sdk.shared = {}
Plugin.authoring = {}

-- SDK STATIC PLUGIN SECTION
Plugin.sdk.static.includedirs = -- https://github.com/premake/premake-core/wiki/includedirs
{
    "../JuceLibraryCode",
    "../JUCE/modules",
}
Plugin.sdk.static.files = -- https://github.com/premake/premake-core/wiki/files
{
    "../JuceLibraryCode/include_juce_core.cpp",
    "../JuceLibraryCode/include_juce_core.h",
    "../JuceLibraryCode/include_juce_audio_basics.cpp",
    "../JuceLibraryCode/include_juce_audio_basics.h",
    "../JuceLibraryCode/include_juce_audio_devices.cpp",
    "../JuceLibraryCode/include_juce_audio_devices.h",
    "../JuceLibraryCode/include_juce_audio_formats.cpp",
    "../JuceLibraryCode/include_juce_audio_formats.h",
    "../JuceLibraryCode/include_juce_audio_processors.cpp",
    "../JuceLibraryCode/include_juce_audio_processors.h",
    "../JuceLibraryCode/include_juce_audio_utils.cpp",
    "../JuceLibraryCode/include_juce_audio_utils.h",

    "**.cpp",
    "**.h",
    "**.hpp",
    "**.c",
}
Plugin.sdk.static.excludes = -- https://github.com/premake/premake-core/wiki/removefiles
{
    "WwisePluginTemplateFXShared.cpp"
}
Plugin.sdk.static.links = -- https://github.com/premake/premake-core/wiki/links
{
}
Plugin.sdk.static.libsuffix = "FX"
Plugin.sdk.static.libdirs = -- https://github.com/premake/premake-core/wiki/libdirs
{
}
Plugin.sdk.static.defines = -- https://github.com/premake/premake-core/wiki/defines
{
    "JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED"
}
Plugin.sdk.static.custom = function()
    buildoptions { "/GR", "/EHsc" }
    cppdialect "C++17"
end

-- SDK SHARED PLUGIN SECTION
Plugin.sdk.shared.includedirs =
{
}
Plugin.sdk.shared.files =
{
    "WwisePluginTemplateFXShared.cpp",
    "WwisePluginTemplateFXFactory.h",
}
Plugin.sdk.shared.excludes =
{
}
Plugin.sdk.shared.links =
{
}
Plugin.sdk.shared.libdirs =
{
}
Plugin.sdk.shared.defines =
{
}

-- AUTHORING PLUGIN SECTION
Plugin.authoring.includedirs =
{
    "../JuceLibraryCode",
    "../JUCE/modules",
}
Plugin.authoring.files =
{ 
    "../JuceLibraryCode/include_juce_core.cpp",
    "../JuceLibraryCode/include_juce_core.h",
    "../JuceLibraryCode/include_juce_data_structures.cpp",
    "../JuceLibraryCode/include_juce_data_structure.h",
    "../JuceLibraryCode/include_juce_events.cpp",
    "../JuceLibraryCode/include_juce_events.h",
    "../JuceLibraryCode/include_juce_graphics.cpp",
    "../JuceLibraryCode/include_juce_graphics.h",
    "../JuceLibraryCode/include_juce_gui_basics.cpp",
    "../JuceLibraryCode/include_juce_gui_basics.h",
    "../JuceLibraryCode/include_juce_gui_extra.cpp",
    "../JuceLibraryCode/include_juce_gui_extra.h",

    "**.cpp",
    "**.h",
    "**.hpp",
    "**.c",
    
    "WwisePluginTemplate.def",
    "WwisePluginTemplate.xml",
    "**.rc",
}
Plugin.authoring.excludes =
{
}
Plugin.authoring.links =
{
}
Plugin.authoring.libdirs =
{
}
Plugin.authoring.defines =
{
    "JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED"
}
Plugin.authoring.custom = function()
    buildoptions { "/bigobj" }
end

return Plugin

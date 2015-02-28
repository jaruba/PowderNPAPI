#/**********************************************************\ 
#
# Auto-Generated Plugin Configuration file
# for Powder NPAPI Plugin
#
#\**********************************************************/

set(PLUGIN_NAME "PowderNPAPI")
set(PLUGIN_PREFIX "PNP")
set(COMPANY_NAME "jaruba")

# ActiveX constants:
set(FBTYPELIB_NAME PowderNPAPILib)
set(FBTYPELIB_DESC "PowderNPAPI 0.0.1 Type Library")
set(IFBControl_DESC "PowderNPAPI Control Interface")
set(FBControl_DESC "PowderNPAPI Control Class")
set(IFBComJavascriptObject_DESC "PowderNPAPI IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "PowderNPAPI ComJavascriptObject Class")
set(IFBComEventSource_DESC "PowderNPAPI IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID ea6d99e8-7459-503e-b481-cdb55d599a87)
set(IFBControl_GUID 7e4d3868-1120-592d-b0d7-9dce9d298c02)
set(FBControl_GUID fba89332-a606-516e-9df3-2597e33a1aee)
set(IFBComJavascriptObject_GUID 21841ab2-3c1f-5892-949d-0abce29cfeea)
set(FBComJavascriptObject_GUID cc9df344-52e7-5888-b0e0-66a106f3dd65)
set(IFBComEventSource_GUID 83e28956-6d78-5caf-a4e6-dd6aed34791a)
if ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID b5447e0b-2e07-5ccc-9939-004965de2626)
else ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID ca4edc1d-eec4-5664-b560-dda9c796b9e1)
endif ( FB_PLATFORM_ARCH_32 )

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "jaruba.PowderNPAPI")
set(MOZILLA_PLUGINID "none.com/PowderNPAPI")

# strings
set(FBSTRING_CompanyName "Jaruba")
set(FBSTRING_PluginDescription "An NPAPI Plugin that interfaces Peerflix for the browser")
set(FBSTRING_PLUGIN_VERSION "0.0.1")
set(FBSTRING_LegalCopyright "Copyright 2015 Jaruba")
set(FBSTRING_PluginFileName "np${PLUGIN_NAME}.dll")
set(FBSTRING_ProductName "Powder NPAPI Plugin")
set(FBSTRING_FileExtents "")
if ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "Powder NPAPI Plugin")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "Powder NPAPI Plugin_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )
set(FBSTRING_MIMEType "application/x-powdernpapi")

# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 0)
set(FBMAC_USE_COCOA 0)
set(FBMAC_USE_COREGRAPHICS 0)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)

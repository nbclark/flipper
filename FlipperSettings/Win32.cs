using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Text;

namespace FlipperSettings
{
    internal class Win32
    {
        [DllImport("coredll.dll", EntryPoint = "LoadLibraryW", SetLastError = true)]
        internal static extern IntPtr LoadLibrary(string lpszLib);

        [DllImport("coredll.dll", EntryPoint = "GetProcAddressW", SetLastError = true)]
        internal static extern IntPtr GetProcAddress(IntPtr hModule, string procName);

        [DllImport("coredll.dll", EntryPoint = "FreeLibraryW", SetLastError = true)]
        internal static extern bool FreeLibrary(IntPtr hModule);

        [DllImport(@"\program files\mobilesrc\flipper\flippersetup.dll", EntryPoint = "GetPluginNames", SetLastError = true)]
        internal static extern int GetPluginNames(IntPtr hModule, [MarshalAs(UnmanagedType.LPArray)]FlipperPlugin[] pPlugins, ulong nMaxCount);
        
        [StructLayout(LayoutKind.Sequential, Size=92),Serializable]
        internal struct FlipperPlugin
        {
            public int pluginId;
            public IntPtr pData;
            [MarshalAsAttribute(UnmanagedType.ByValTStr, SizeConst = 30)]
	        public string wzName;
	        public ulong dwColor;
            public IntPtr pfnDrawThumbnail;
            public IntPtr pfnNodeSelected;
            public IntPtr pfnCloseOnSelection;
            public IntPtr pfnBuildNodeList;
            public IntPtr pfnDestroyNodeList;
        };
    }
}

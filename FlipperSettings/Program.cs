using System;

using System.Collections.Generic;
using System.Windows.Forms;

namespace FlipperSettings
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [MTAThread]
        static void Main()
        {
            string[] s = System.IO.Directory.GetFiles(@"\program files\mobilesrc\flipper");
            bool b = System.IO.File.Exists(@"\program files\mobilesrc\flipper\flippersetup.dll");
            IntPtr lib = Win32.LoadLibrary(@"\program files\mobilesrc\flipper\plugins\flipperplugins.dll");
            Win32.FlipperPlugin[] plugins = new Win32.FlipperPlugin[5];
            int count = Win32.GetPluginNames(lib, plugins, 5);
            Win32.FreeLibrary(lib);
            Application.Run(new FlipperSettings());
        }
    }
}
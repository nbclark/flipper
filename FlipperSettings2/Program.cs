using System;
using System.IO;
using System.Reflection;
using System.Collections.Generic;
using System.Windows.Forms;

namespace MobileSRC.GScroll
{
    static class Program
    {
        public static Documentation Documentation;
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [MTAThread]
        static void Main()
        {
            string workingDir = Path.GetDirectoryName(Assembly.GetExecutingAssembly().ManifestModule.FullyQualifiedName);
            System.Xml.Serialization.XmlSerializer xs = new System.Xml.Serialization.XmlSerializer(typeof(Documentation));
            using (StringReader fs = new StringReader(Properties.Resources.documentation))
            {
                Program.Documentation = (Documentation)xs.Deserialize(fs);
            }
            Cursor.Current = Cursors.WaitCursor;
            GScrollSettings settings = new GScrollSettings();
            if (!GScrollRegistry.CheckRegistration())
            {
                using (Register register = new Register())
                {
                    register.ShowDialog();
                }
            }
            Cursor.Current = Cursors.WaitCursor;
            Application.Run(settings);
        }
    }
}
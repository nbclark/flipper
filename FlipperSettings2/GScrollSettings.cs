using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Xml.Serialization;
using Microsoft.Win32;

namespace MobileSRC.GScroll
{
    public partial class GScrollSettings : Form
    {
        public GScrollSettings()
        {
            InitializeComponent();
            UpdateRegistration();
        }

        private static Bitmap GetRegionImage(char region)
        {
            switch (region)
            {
                case '0':
                    {
                        return Properties.Resources.home;
                    }
                case '1':
                    {
                        return Properties.Resources.call;
                    }
                case '2':
                    {
                        return Properties.Resources.back;
                    }
                case '3':
                    {
                        return Properties.Resources.end;
                    }
                case '4':
                    {
                        return Properties.Resources.wheel;
                    }
            }
            return new Bitmap(32, 32);
        }

        public static Bitmap BuildSequenceImage(string sequence, int maxLength, Color backColor)
        {
            return BuildSequenceImage(sequence, maxLength, backColor, 32, 32);
        }

        public static Bitmap BuildSequenceImage(string sequence, int maxLength, Color backColor, int width, int height)
        {
            Bitmap bmp = new Bitmap(width * maxLength, height);

            using (Graphics g = Graphics.FromImage(bmp))
            {
                g.Clear(backColor);
                int start = 0;
                foreach (char c in sequence)
                {
                    using (Bitmap icon = GetRegionImage(c))
                    {
                        g.DrawImage(icon, new Rectangle(start, 0, width, height), new Rectangle(0, 0, icon.Width, icon.Height), GraphicsUnit.Pixel);
                    }
                    start += width;
                }
            }

            return bmp;
        }

        private void GScrollSettings_Closed(object sender, EventArgs e)
        {
            if (this.DialogResult != DialogResult.Cancel)
            {
                this._generalSettings.Save();
                this._launchSettings.Save();
                this._perProcessSettings.Save();
                this._appSwitchSettings.Save();
            }
        }

        private void menuItem1_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            Close();
        }

        private void menuItem2_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            Close();
        }

        private void menuItem3_Click(object sender, EventArgs e)
        {
            using (Register register = new Register())
            {
                register.ShowDialog();
            }
        }

        private void UpdateRegistration()
        {
            _registerMenuItem.Enabled = !GScrollRegistry.CheckRegistration();

            if (_registerMenuItem.Enabled)
            {
                _registerMenuItem.Text = "Register";
            }
            else
            {
                _registerMenuItem.Text = "Registered - Thank You";
            }
        }

        private void _mobileSrcLinkLabel_Click(object sender, EventArgs e)
        {
            // shell execute mobilesrc.com
            LaunchUrl("http://www.mobilesrc.com/");
        }

        private void LaunchUrl(string url)
        {
            System.Diagnostics.Process p = new System.Diagnostics.Process();
            p.StartInfo = new System.Diagnostics.ProcessStartInfo(url, "");
            p.StartInfo.UseShellExecute = true;
            p.Start();
        }

        private void _checkForUpdatesMenuItem_Click(object sender, EventArgs e)
        {
            Cursor.Current = Cursors.WaitCursor;
            try
            {
                VersionXml newData;
                VersionXml oldData;

                XmlSerializer serializer = new XmlSerializer(typeof(VersionXml));
                string currentPath = Path.Combine(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "AppToDate"), "gscroll2.xml");
                using (StreamReader reader = new StreamReader(File.OpenRead(currentPath)))
                {
                    oldData = (VersionXml)serializer.Deserialize(reader);
                }

                System.Net.WebRequest webRequest = System.Net.WebRequest.Create(oldData.VersionUrl);
                using (StreamReader reader = new StreamReader(webRequest.GetResponse().GetResponseStream()))
                {
                    newData = (VersionXml)serializer.Deserialize(reader);
                }

                if (newData.Version > oldData.Version)
                {
                    if (DialogResult.Yes == MessageBox.Show(string.Format("Version {0:0.00} is now available. Would you like to update now?", newData.Version), "GScroll Update Available", MessageBoxButtons.YesNo, MessageBoxIcon.Asterisk, MessageBoxDefaultButton.Button1))
                    {
                        LaunchUrl(newData.CabUrl);
                        Close();
                    }
                }
                else
                {
                    MessageBox.Show("GScroll is up to date!", "Up To Date", MessageBoxButtons.OK, MessageBoxIcon.Asterisk, MessageBoxDefaultButton.Button1);
                }
            }
            catch
            {
                if (DialogResult.Yes == MessageBox.Show("There was an error checking for updates. Would you like to visit the mobilesrc.com webpage?", "Error Checking for Updates", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1))
                {
                    LaunchUrl("http://www.mobilesrc.com/");
                }
            }
            Cursor.Current = Cursors.Default;
        }
    }
}
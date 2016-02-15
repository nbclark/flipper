using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;
using Microsoft.Win32;

namespace MobileSRC.GScroll
{
    public partial class SelectProcess : Form
    {
        public SelectProcess()
        {
            InitializeComponent();
        }

        public string ProcessName
        {
            get
            {
                return _processNameTextBox.Text;
            }
        }

        public DialogResult ShowDialog(string processName)
        {
            _processNameTextBox.Text = processName;
            return base.ShowDialog();
        }

        private void menuItem1_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
        }

        private void menuItem2_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
        }

        private void _processListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (_processListView.SelectedIndices.Count > 0)
            {
                string linkPath = _processListView.Items[_processListView.SelectedIndices[0]].Tag as string;

                StringBuilder processPathName = new StringBuilder(255);
                if (ApplicationListView.GetProcessName(linkPath, processPathName, processPathName.Capacity))
                {
                    string processName = processPathName.ToString().Trim().Replace("/", "").Replace("\"", "");

                    while (processName.StartsWith(":"))
                    {
                        try
                        {
                            processName = processName.Split(' ')[0];
                            using (RegistryKey key = Registry.LocalMachine.OpenSubKey(String.Format(@"Software\Microsoft\Shell\Rai\{0}", processName)))
                            {
                                processName = Convert.ToString(key.GetValue("1", string.Empty)).Trim().Replace("/", "");
                            }
                        }
                        catch
                        {
                            break;
                        }
                    }

                    if (processName.Length > 0)
                    {
                        try
                        {
                            processName = Path.GetFileName(processName).Split(',', ' ')[0];
                        }
                        catch
                        {
                            processName = string.Empty;
                        }
                        _processNameTextBox.Text = processName;
                    }
                }
            }
        }

        private void EditLaunch_Closing(object sender, CancelEventArgs e)
        {
            if (this.DialogResult != DialogResult.Cancel)
            {
                if (string.IsNullOrEmpty(_processNameTextBox.Text))
                {
                    e.Cancel = true;
                }
            }
        }
    }
}
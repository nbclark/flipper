using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Microsoft.Win32;

namespace MobileSRC.GScroll
{
    public partial class PerProcessSettings : UserControl
    {
        public PerProcessSettings()
        {
            InitializeComponent();
            InitializeSettings();
        }

        private void InitializeSettings()
        {
            this._perProcessListView.Items.Clear();
            using (RegistryKey rootKey = Registry.CurrentUser.CreateSubKey(Path.Combine(GScrollRegistry.GScrollKey, GScrollRegistry.PerProcessKey)))
            {
                foreach (string processName in rootKey.GetSubKeyNames())
                {
                    ListViewItem item = new ListViewItem(processName);
                    item.Tag = null;

                    this._perProcessListView.Items.Add(item);
                }
            }
        }

        public void Save()
        {
            List<string> reservedProcs = new List<string>();
            foreach (ListViewItem item in _perProcessListView.Items)
            {
                reservedProcs.Add(item.Text);
                if (item.Tag is GeneralSettings)
                {
                    ((GeneralSettings)item.Tag).Save();
                    item.Tag = null;
                }
            }
            using (RegistryKey rootKey = Registry.CurrentUser.CreateSubKey(Path.Combine(GScrollRegistry.GScrollKey, GScrollRegistry.PerProcessKey)))
            {
                foreach (string processName in rootKey.GetSubKeyNames())
                {
                    if (!reservedProcs.Contains(processName))
                    {
                        rootKey.DeleteSubKeyTree(processName);
                    }
                }
            }
        }

        private void _processContextMenu_Popup(object sender, EventArgs e)
        {
            _editProcessMenuItem.Enabled = _removeProcessMenuItem.Enabled = (_perProcessListView.SelectedIndices.Count > 0);
        }

        private void _customizeProcessMenuItem_Click(object sender, EventArgs e)
        {
            using (SelectProcess selectProc = new SelectProcess())
            {
                if (DialogResult.OK == selectProc.ShowDialog())
                {
                    foreach (ListViewItem item in _perProcessListView.Items)
                    {
                        if (string.Equals(item.Text, selectProc.ProcessName, StringComparison.InvariantCultureIgnoreCase))
                        {
                            MessageBox.Show("Sepcified process already exists.", "Process Exists", MessageBoxButtons.OK, MessageBoxIcon.Asterisk, MessageBoxDefaultButton.Button1);
                            return;
                        }
                    }

                    GeneralSettings customSettings = new GeneralSettings();
                    customSettings.Load(string.Format(@"{0}\{1}\{2}", GScrollRegistry.GScrollKey, GScrollRegistry.PerProcessKey, selectProc.ProcessName), true);

                    using (GeneralSettingsHost host = new GeneralSettingsHost())
                    {
                        if (DialogResult.OK == host.ShowDialog(string.Format("Settings for {0}", selectProc.ProcessName), customSettings))
                        {
                            // add the list item here
                            ListViewItem item = new ListViewItem(selectProc.ProcessName);
                            item.Tag = customSettings;

                            _perProcessListView.Items.Add(item);
                        }
                        else
                        {
                            customSettings.Dispose();
                        }
                    }
                }
            }
        }

        private void _removeProcessMenuItem_Click(object sender, EventArgs e)
        {
            if (_perProcessListView.SelectedIndices.Count > 0)
            {
                _perProcessListView.Items.RemoveAt(_perProcessListView.SelectedIndices[0]);
            }
        }

        private void _editProcessMenuItem_Click(object sender, EventArgs e)
        {
            if (_perProcessListView.SelectedIndices.Count > 0)
            {
                ListViewItem item = _perProcessListView.Items[_perProcessListView.SelectedIndices[0]];
                // show a dialog here to create

                GeneralSettings customSettings = null;

                if (null != item.Tag)
                {
                    customSettings = item.Tag as GeneralSettings;
                }
                else
                {
                    customSettings = new GeneralSettings();
                    customSettings.Load(string.Format(@"{0}\{1}\{2}", GScrollRegistry.GScrollKey, GScrollRegistry.PerProcessKey, item.Text), false);
                    item.Tag = customSettings;
                }

                using (GeneralSettingsHost host = new GeneralSettingsHost())
                {
                    host.ShowDialog(string.Format("Settings for {0}", item.Text), customSettings);
                }
            }
        }
    }
}

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
    public partial class AppSwitchSettings : UserControl
    {
        public AppSwitchSettings()
        {
            InitializeComponent();
            InitializeSettings();
        }

        public void InitializeSettings()
        {
            using (RegistryKey rootKey = Registry.CurrentUser.CreateSubKey(string.Format(@"{0}\{1}", GScrollRegistry.GScrollKey, GScrollRegistry.AppSwitchKey)))
            {
                _enableShrinkAnimationCheckBox.Checked = (Convert.ToUInt32(rootKey.GetValue(GScrollRegistry.AppSwitch_EnableShrinkAnimation, 0)) != 0);
                _enableScrollAnimationCheckBox.Checked = (Convert.ToUInt32(rootKey.GetValue(GScrollRegistry.AppSwitch_EnableScrollAnimation, 0)) != 0);
                _enableThumbnailsCheckBox.Checked = (Convert.ToUInt32(rootKey.GetValue(GScrollRegistry.AppSwitch_EnableThumbnails, 0)) != 0);
                _autoCloseDelayUpDown.Value = Convert.ToUInt32(rootKey.GetValue(GScrollRegistry.AppSwitch_AutoCloseDelay, 0));
                _enableAutoCloseCheckBox.Checked = (_autoCloseDelayUpDown.Value > 0);
            }
            using (RegistryKey rootKey = Registry.CurrentUser.CreateSubKey(string.Format(@"{0}\{1}\{2}", GScrollRegistry.GScrollKey, GScrollRegistry.AppSwitchKey, GScrollRegistry.AppSwitch_ProcessKey)))
            {
                foreach (string reservedProcValue in rootKey.GetSubKeyNames())
                {
                    AppSwitchProcessSettings settings = new AppSwitchProcessSettings();
                    settings.Load(string.Format(@"{0}\{1}\{2}\{3}", GScrollRegistry.GScrollKey, GScrollRegistry.AppSwitchKey, GScrollRegistry.AppSwitch_ProcessKey, reservedProcValue), false);

                    ListViewItem item = new ListViewItem(new string[] { reservedProcValue, settings.Ignore.ToString(), settings.CanClose.ToString(), settings.CanMinimize.ToString() });
                    item.Tag = settings;

                    this._reservedProcessListView.Items.Add(item);
                }
            }
        }

        public void Save()
        {
            using (RegistryKey rootKey = Registry.CurrentUser.CreateSubKey(string.Format(@"{0}\{1}", GScrollRegistry.GScrollKey, GScrollRegistry.AppSwitchKey)))
            {
                rootKey.SetValue(GScrollRegistry.AppSwitch_EnableShrinkAnimation, _enableShrinkAnimationCheckBox.Checked, RegistryValueKind.DWord);
                rootKey.SetValue(GScrollRegistry.AppSwitch_EnableScrollAnimation, _enableScrollAnimationCheckBox.Checked, RegistryValueKind.DWord);
                rootKey.SetValue(GScrollRegistry.AppSwitch_EnableThumbnails, _enableThumbnailsCheckBox.Checked, RegistryValueKind.DWord);
                rootKey.SetValue(GScrollRegistry.AppSwitch_AutoCloseDelay, _autoCloseDelayUpDown.Value, RegistryValueKind.DWord);
            }
            List<string> reservedProcs = new List<string>();
            foreach (ListViewItem item in _reservedProcessListView.Items)
            {
                reservedProcs.Add(item.Text);
                if (item.Tag is AppSwitchProcessSettings)
                {
                    ((AppSwitchProcessSettings)item.Tag).Save();
                    item.Tag = null;
                }
            }
            using (RegistryKey rootKey = Registry.CurrentUser.CreateSubKey(string.Format(@"{0}\{1}\{2}", GScrollRegistry.GScrollKey, GScrollRegistry.AppSwitchKey, GScrollRegistry.AppSwitch_ProcessKey)))
            {
                foreach (string processName in rootKey.GetSubKeyNames())
                {
                    if (!reservedProcs.Contains(processName))
                    {
                        try
                        {
                            rootKey.DeleteSubKey(processName);
                        }
                        catch
                        {
                        }
                    }
                }
            }
        }

        private void _customizeProcessMenuItem_Click(object sender, EventArgs e)
        {
            using (SelectProcess selectProc = new SelectProcess())
            {
                if (DialogResult.OK == selectProc.ShowDialog())
                {
                    foreach (ListViewItem item in _reservedProcessListView.Items)
                    {
                        if (string.Equals(item.Text, selectProc.ProcessName, StringComparison.InvariantCultureIgnoreCase))
                        {
                            MessageBox.Show("Sepcified process already exists.", "Process Exists", MessageBoxButtons.OK, MessageBoxIcon.Asterisk, MessageBoxDefaultButton.Button1);
                            return;
                        }
                    }
                    AppSwitchProcessSettings settings = new AppSwitchProcessSettings();
                    settings.Load(string.Format(@"{0}\{1}\{2}\{3}", GScrollRegistry.GScrollKey, GScrollRegistry.AppSwitchKey, GScrollRegistry.AppSwitch_ProcessKey, selectProc.ProcessName), true);

                    using (GeneralSettingsHost host = new GeneralSettingsHost())
                    {
                        if (DialogResult.OK == host.ShowDialog(string.Format("Settings for {0}", selectProc.ProcessName), settings))
                        {
                            ListViewItem newItem = new ListViewItem(new string[] { selectProc.ProcessName, settings.Ignore.ToString(), settings.CanClose.ToString(), settings.CanMinimize.ToString() });
                            newItem.Tag = settings;

                            _reservedProcessListView.Items.Add(newItem);
                        }
                        else
                        {
                            settings.Dispose();
                        }
                    }
                }
            }
        }

        private void _removeProcessMenuItem_Click(object sender, EventArgs e)
        {
            if (_reservedProcessListView.SelectedIndices.Count > 0)
            {
                _reservedProcessListView.Items.RemoveAt(_reservedProcessListView.SelectedIndices[0]);
            }
        }

        private void _editProcessMenuItem_Click(object sender, EventArgs e)
        {
            if (_reservedProcessListView.SelectedIndices.Count > 0)
            {
                ListViewItem item = _reservedProcessListView.Items[_reservedProcessListView.SelectedIndices[0]];
                AppSwitchProcessSettings settings = item.Tag as AppSwitchProcessSettings;

                using (GeneralSettingsHost host = new GeneralSettingsHost())
                {
                    host.ShowDialog(string.Format("Settings for {0}", item.Text), settings);
                    item.SubItems[1].Text = settings.Ignore.ToString();
                    item.SubItems[2].Text = settings.CanClose.ToString();
                    item.SubItems[3].Text = settings.CanMinimize.ToString();
                }
            }
        }

        private void _processContextMenu_Popup(object sender, EventArgs e)
        {
            _editProcessMenuItem.Enabled = _removeProcessMenuItem.Enabled = (_reservedProcessListView.SelectedIndices.Count > 0);
        }

        private void _enableAutoCloseCheckBox_CheckStateChanged(object sender, EventArgs e)
        {
            _autoCloseDelayUpDown.Enabled = _autoCloseDescription.Enabled = _enableAutoCloseCheckBox.Checked;
        }
    }
}

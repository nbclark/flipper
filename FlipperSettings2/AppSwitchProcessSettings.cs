using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;

namespace MobileSRC.GScroll
{
    public partial class AppSwitchProcessSettings : UserControl
    {
        private string _rootKey = string.Empty;
        public AppSwitchProcessSettings()
        {
            InitializeComponent();
        }

        public void Load(string rootKey, bool createdNew)
        {
            _rootKey = rootKey;

            if (!createdNew)
            {
                InitializeSettings();
            }
        }

        public bool Ignore
        {
            get { return _ignoreCheckBox.Checked; }
        }

        public bool CanClose
        {
            get { return _terminateCheckBox.Checked; }
        }

        public bool CanMinimize
        {
            get { return _minimizeCheckBox.Checked; }
        }

        private void InitializeSettings()
        {
            _ignoreCheckBox.Checked = (GScrollRegistry.ReadFromRegistry(_rootKey, GScrollRegistry.AppSwitch_Process_Ignore, 0) != 0);
            _terminateCheckBox.Checked = (GScrollRegistry.ReadFromRegistry(_rootKey, GScrollRegistry.AppSwitch_Process_CanClose, 0) == 0);
            _minimizeCheckBox.Checked = (GScrollRegistry.ReadFromRegistry(_rootKey, GScrollRegistry.AppSwitch_Process_CanMinimize, 0) == 0);
        }

        public void Save()
        {
            using (RegistryKey rootKey = Registry.CurrentUser.CreateSubKey(_rootKey))
            {
                rootKey.SetValue(GScrollRegistry.AppSwitch_Process_Ignore, _ignoreCheckBox.Checked, RegistryValueKind.DWord);
                rootKey.SetValue(GScrollRegistry.AppSwitch_Process_CanClose, !_terminateCheckBox.Checked, RegistryValueKind.DWord);
                rootKey.SetValue(GScrollRegistry.AppSwitch_Process_CanMinimize, !_minimizeCheckBox.Checked, RegistryValueKind.DWord);
            }
        }
    }
}

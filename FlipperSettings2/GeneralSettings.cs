using System;
using System.Reflection;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using Microsoft.Win32;

namespace MobileSRC.GScroll
{
    public partial class GeneralSettings : UserControl
    {
        protected class TitleAttribute : Attribute
        {
            public string Title;
            public TitleAttribute(string title)
            {
                Title = title;
            }
        }
        protected class DescriptionAttribute : Attribute
        {
            public string Description;
            public DescriptionAttribute(string description)
            {
                Description = description;
            }
        }

        private class TapAndTiltAttribute : Attribute
        {
        }

        protected enum ScrollMode
        {
            [TapAndTilt()]
            [TitleAttribute("Disabled")]
            [DescriptionAttribute("Swiping in this mode will have no effect.")]
            Disabled = 0,
            [TapAndTilt()]
            [TitleAttribute("Scroll")]
            [DescriptionAttribute("Scroll applications with scroll bars.")]
            ScrollOnly = 1,
            [TapAndTilt()]
            [TitleAttribute("DPad")]
            [DescriptionAttribute("Simulate DPad keypresses (up/down/left/right).")]
            DPadOnly = 2,
            [TapAndTilt()]
            [TitleAttribute("Smart Scroll")]
            [DescriptionAttribute("Scroll applications with scroll bars.  Simulate DPad in applications without scroll bars.")]
            SmartScroll = 3,
            [TitleAttribute("Left:Scroll, Right:DPad")]
            [DescriptionAttribute("Left panel scrolls applications. Right panel simulates DPad.")]
            LeftScrollRightDir = 4,
            [TitleAttribute("Left:DPad, Right:Scroll")]
            [DescriptionAttribute("Left panel simulates DPad. Right panel scrolls applications.")]
            LeftDirRightScroll = 5
        }

        protected class EnumValue<T>
        {
            private TitleAttribute _title;
            private DescriptionAttribute _description;
            private T _value;

            public EnumValue(FieldInfo field)
            {
                _value = (T)field.GetValue(null);
                _title = (TitleAttribute)field.GetCustomAttributes(typeof(TitleAttribute), false)[0];
                _description = (DescriptionAttribute)field.GetCustomAttributes(typeof(DescriptionAttribute), false)[0];
            }

            public override string ToString()
            {
                return _title.Title;
            }

            public string Description
            {
                get { return _description.Description; }
            }

            public T Value
            {
                get { return _value; }
            }
        }

        private string _rootKey = GScrollRegistry.GScrollKey;
        public GeneralSettings()
        {
            InitializeComponent();
            _panelSwipeComboBox.DisplayMember = "Name";
            _panelSwipeComboBox.ValueMember = "Value";
            _tapTiltComoBox.DisplayMember = "Name";
            _tapTiltComoBox.ValueMember = "Value";

            List<object> objs = new List<object>();
            foreach (FieldInfo field in typeof(ScrollMode).GetFields(BindingFlags.Public | BindingFlags.Static))
            {
                objs.Add(new EnumValue<ScrollMode>(field));
            }

            _panelSwipeComboBox.DataSource = objs;

            List<object> objs2 = new List<object>();
            foreach (FieldInfo field in typeof(ScrollMode).GetFields(BindingFlags.Public | BindingFlags.Static))
            {
                if (field.GetCustomAttributes(typeof(TapAndTiltAttribute), false).Length > 0)
                {
                    objs2.Add(new EnumValue<ScrollMode>(field));
                }
            }
            _tapTiltComoBox.DataSource = objs2;

            InitializeSettings();
        }

        public void Load(string rootKey, bool createdNew)
        {
            _rootKey = rootKey;

            if (!createdNew)
            {
                InitializeSettings();
            }
        }

        private void InitializeSettings()
        {
            if (_rootKey == GScrollRegistry.GScrollKey)
            {
                _isReservedCheckBox.Text = "Disable GScroll";
            }
            else
            {
                _isReservedCheckBox.Text = "Disable GScroll in Process";
            }
            _panelSwipeComboBox.SelectedValue = (ScrollMode)GScrollRegistry.ReadFromRegistry(_rootKey, GScrollRegistry.SwipeMode, 0);
            _tapTiltComoBox.SelectedValue = (ScrollMode)GScrollRegistry.ReadFromRegistry(_rootKey, GScrollRegistry.TiltMode, 0);
            _isReservedCheckBox.Checked = (GScrollRegistry.ReadFromRegistry(_rootKey, GScrollRegistry.Disabled, 0) != 0);
            _enableScrollWheelCheckBox.Checked = (GScrollRegistry.ReadFromRegistry(_rootKey, GScrollRegistry.EnableScrollWheel, 0) != 0);
            _enableGameModeCheckBox.Checked = (GScrollRegistry.ReadFromRegistry(_rootKey, GScrollRegistry.EnableGameMode, 0) != 0);
            _enablePrecisionModeCheckBox.Checked = (GScrollRegistry.ReadFromRegistry(_rootKey, GScrollRegistry.EnablePrecisionMode, 0) != 0);
            _smoothScrollCheckBox.Checked = (GScrollRegistry.ReadFromRegistry(_rootKey, GScrollRegistry.EnableSmoothScroll, 0) != 0);
            _gameModeRepeatUpDown.Value = GScrollRegistry.ReadFromRegistry(_rootKey, GScrollRegistry.GameModeKeyRepeat, 500);

            _precisionNavigateUpDown.Value = GScrollRegistry.ReadFromRegistry(_rootKey, GScrollRegistry.PrecisionModeNavigateSize, 20);
            _precisionScrollUpDown.Value = GScrollRegistry.ReadFromRegistry(_rootKey, GScrollRegistry.PrecisionModeScrollSize, 10);
        }

        public void Save()
        {
            using (RegistryKey rootKey = Registry.CurrentUser.CreateSubKey(_rootKey))
            {
                rootKey.SetValue(GScrollRegistry.SwipeMode, _panelSwipeComboBox.SelectedValue, RegistryValueKind.DWord);
                rootKey.SetValue(GScrollRegistry.TiltMode, _tapTiltComoBox.SelectedValue, RegistryValueKind.DWord);

                rootKey.SetValue(GScrollRegistry.Disabled, _isReservedCheckBox.Checked, RegistryValueKind.DWord);
                rootKey.SetValue(GScrollRegistry.EnableScrollWheel, _enableScrollWheelCheckBox.Checked, RegistryValueKind.DWord);
                rootKey.SetValue(GScrollRegistry.EnableGameMode, _enableGameModeCheckBox.Checked, RegistryValueKind.DWord);
                rootKey.SetValue(GScrollRegistry.EnableSmoothScroll, _smoothScrollCheckBox.Checked, RegistryValueKind.DWord);
                rootKey.SetValue(GScrollRegistry.EnablePrecisionMode, _enablePrecisionModeCheckBox.Checked, RegistryValueKind.DWord);
                rootKey.SetValue(GScrollRegistry.GameModeKeyRepeat, _gameModeRepeatUpDown.Value, RegistryValueKind.DWord);

                rootKey.SetValue(GScrollRegistry.PrecisionModeNavigateSize, _precisionNavigateUpDown.Value, RegistryValueKind.DWord);
                rootKey.SetValue(GScrollRegistry.PrecisionModeScrollSize, _precisionScrollUpDown.Value, RegistryValueKind.DWord);
            }
        }

        private void _isReservedCheckBox_CheckStateChanged(object sender, EventArgs e)
        {
            _smoothScrollCheckBox.Enabled = _enablePrecisionModeCheckBox.Enabled = _enableGameModeCheckBox.Enabled = _enableScrollWheelCheckBox.Enabled = _panelSwipeComboBox.Enabled = _tapTiltComoBox.Enabled = !_isReservedCheckBox.Checked;
        }

        private void _enableGameModeCheckBox_CheckStateChanged(object sender, EventArgs e)
        {
            _gameModeRepeatUpDown.Enabled = _gameModeRepeatDescription.Enabled = _enableGameModeCheckBox.Checked;
        }

        private void _enablePrecisionModeCheckBox_CheckStateChanged(object sender, EventArgs e)
        {
            _precisionNavigateUpDown.Enabled = _precisionNavigateDescription.Enabled = _enablePrecisionModeCheckBox.Checked;
            _precisionScrollUpDown.Enabled = _precisionScrollDescription.Enabled = _enablePrecisionModeCheckBox.Checked;
        }

        private void helpLabel_Click(object sender, EventArgs e)
        {
            using (HelpDialog dialog = new HelpDialog())
            {
                dialog.ShowDialog(Program.Documentation.FindFeature(((LinkLabel)sender).Tag as string));
            }
        }
    }
}

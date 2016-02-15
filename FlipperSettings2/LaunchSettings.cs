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
    public partial class LaunchSettings : UserControl
    {
        public LaunchSettings()
        {
            InitializeComponent();
            InitializeSettings();
        }
        private int _maxLength = 0;

        private void InitializeSettings()
        {
            this._launchListView.Items.Clear();
            using (RegistryKey rootKey = Registry.CurrentUser.CreateSubKey(Path.Combine(GScrollRegistry.GScrollKey, GScrollRegistry.LaunchKey)))
            {
                _enableLaunchNotifyCheckBox.Checked = (Convert.ToInt32(rootKey.GetValue(GScrollRegistry.Launch_Notify, 0)) != 0);

                using (RegistryKey sequencesKey = rootKey.CreateSubKey(GScrollRegistry.Launch_SequencesKey))
                {
                    foreach (string launchValue in sequencesKey.GetValueNames())
                    {
                        ListViewItem item = new ListViewItem(new string[] { string.Empty, Convert.ToString(sequencesKey.GetValue(launchValue)) });
                        item.Tag = launchValue;

                        this._launchListView.Items.Add(item);
                    }
                }
            }
            RefreshLaunchImages();
        }

        public void Save()
        {
            // save here
            using (RegistryKey launchKey = Registry.CurrentUser.CreateSubKey(Path.Combine(GScrollRegistry.GScrollKey, GScrollRegistry.LaunchKey)))
            {
                using (RegistryKey sequencesKey = launchKey.CreateSubKey(GScrollRegistry.Launch_SequencesKey))
                {
                    foreach (string keyName in sequencesKey.GetValueNames())
                    {
                        sequencesKey.DeleteValue(keyName, false);
                    }
                    foreach (ListViewItem item in _launchListView.Items)
                    {
                        string sequence = item.Tag as string;
                        string action = item.SubItems[1].Text;

                        sequencesKey.SetValue(sequence, action, RegistryValueKind.String);
                    }
                }
                launchKey.SetValue(GScrollRegistry.Launch_Notify, _enableLaunchNotifyCheckBox.Checked, RegistryValueKind.DWord);
            }
        }

        private void RefreshLaunchImages()
        {
            int maxLength = 0;

            if (_launchListView.Items.Count > 0)
            {
                foreach (ListViewItem item in _launchListView.Items)
                {
                    maxLength = Math.Max(Convert.ToString(item.Tag).Length, maxLength);
                }

                if (maxLength != _maxLength)
                {
                    this._actionImageList.ImageSize = new Size(32 * maxLength, 32);

                    foreach (ListViewItem item in _launchListView.Items)
                    {
                        SetListViewItemImage(item, maxLength);
                    }
                    _maxLength = maxLength;
                }
            }
        }

        private void SetListViewItemImage(ListViewItem item, int maxLength)
        {
            string sequence = Convert.ToString(item.Tag);

            Bitmap bmp = GScrollSettings.BuildSequenceImage(sequence, maxLength, _launchListView.BackColor);

            this._actionImageList.Images.Add(bmp);
            item.ImageIndex = (this._actionImageList.Images.Count - 1);
        }

        private void _addLaunchMenuItem_Click(object sender, EventArgs e)
        {
            // show a dialog here to create
            using (EditLaunch editLaunch = new EditLaunch())
            {
                if (DialogResult.OK == editLaunch.ShowDialog())
                {
                    // add the item here
                    foreach (ListViewItem item in _launchListView.Items)
                    {
                        string sequence = Convert.ToString(item.Tag);

                        if (string.Equals(sequence, editLaunch.Sequence, StringComparison.InvariantCultureIgnoreCase))
                        {
                            MessageBox.Show("Sepcified sequence already exists.", "Sequence Exists", MessageBoxButtons.OK, MessageBoxIcon.Asterisk, MessageBoxDefaultButton.Button1);
                            return;
                        }
                    }
                    ListViewItem newItem = new ListViewItem(new string[] { string.Empty, editLaunch.Action });
                    newItem.Tag = editLaunch.Sequence;
                    _launchListView.Items.Add(newItem);

                    SetListViewItemImage(newItem, Math.Max(_maxLength, editLaunch.Sequence.Length));
                    RefreshLaunchImages();
                }
            }
        }

        private void _editLaunchMenuItem_Click(object sender, EventArgs e)
        {
            if (_launchListView.SelectedIndices.Count > 0)
            {
                ListViewItem item = _launchListView.Items[_launchListView.SelectedIndices[0]];
                // show a dialog here to create
                using (EditLaunch editLaunch = new EditLaunch())
                {
                    if (DialogResult.OK == editLaunch.ShowDialog(item.Tag as string, item.SubItems[1].Text))
                    {
                        item.Tag = editLaunch.Sequence;
                        item.SubItems[1].Text = editLaunch.Action;
                        SetListViewItemImage(item, Math.Max(_maxLength, editLaunch.Sequence.Length));
                        RefreshLaunchImages();
                    }
                }
            }
        }

        private void _removeLaunchMenuItem_Click(object sender, EventArgs e)
        {
            if (_launchListView.SelectedIndices.Count > 0)
            {
                List<ListViewItem> selectedItems = new List<ListViewItem>();

                _launchListView.BeginUpdate();
                foreach (int index in _launchListView.SelectedIndices)
                {
                    selectedItems.Add(_launchListView.Items[index]);
                }
                foreach (ListViewItem item in selectedItems)
                {
                    _launchListView.Items.Remove(item);
                }
                _launchListView.EndUpdate();
            }
        }

        private void _launchContextMenu_Popup(object sender, EventArgs e)
        {
            _editLaunchMenuItem.Enabled = _removeLaunchMenuItem.Enabled = (_launchListView.SelectedIndices.Count > 0);
        }
    }
}

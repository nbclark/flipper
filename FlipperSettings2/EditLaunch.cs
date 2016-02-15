using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.IO;

namespace MobileSRC.GScroll
{
    public partial class EditLaunch : Form
    {
        private string _sequence = string.Empty;
        private bool _isClosed = true;

        public EditLaunch()
        {
            InitializeComponent();
            _availablePictureBox.Image = GScrollSettings.BuildSequenceImage("01234", 5, SystemColors.Window, 64, 64);
        }

        public string Sequence
        {
            get { return _sequence; }
        }

        public string Action
        {
            get
            {
                return _processNameTextBox.Text;
            }
        }

        public DialogResult ShowDialog(string sequence, string action)
        {
            _sequence = sequence;
            _processNameTextBox.Text = action;

            if (_sequence.Length > 0)
            {
                _sequencePictureBox.Image = GScrollSettings.BuildSequenceImage(_sequence, _sequence.Length, SystemColors.Window, 64, 64);
            }

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

        private void EditLaunch_Load(object sender, EventArgs e)
        {
            foreach (ListViewItem item in _launchListView.Items)
            {
                if (!string.IsNullOrEmpty(_processNameTextBox.Text) && string.Equals(_processNameTextBox.Text, item.Tag as string, StringComparison.InvariantCultureIgnoreCase))
                {
                    item.Selected = true;
                    _launchListView.EnsureVisible(item.Index);
                }
            }
            _isClosed = false;
        }

        private void _availablePictureBox_Click(object sender, EventArgs e)
        {
            //
        }

        private void _availablePictureBox_MouseDown(object sender, MouseEventArgs e)
        {
            int clickedRegion = -1;

            if (e.X <= (5 * 64) && _sequence.Length < 7)
            {
                clickedRegion = (e.X / 64);
                _sequence += clickedRegion.ToString();

                if (null != _sequencePictureBox.Image)
                {
                    _sequencePictureBox.Image.Dispose();
                }
                _sequencePictureBox.Image = GScrollSettings.BuildSequenceImage(_sequence, _sequence.Length, SystemColors.Window, 64, 64);
            }
        }

        private void _sequencePictureBox_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.X <= (_sequence.Length * 64))
            {
                int clickedIndex = (e.X / 64);
                _sequence = _sequence.Remove(clickedIndex, 1);

                if (null != _sequencePictureBox.Image)
                {
                    _sequencePictureBox.Image.Dispose();
                }
                if (_sequence.Length > 0)
                {
                    _sequencePictureBox.Image = GScrollSettings.BuildSequenceImage(_sequence, _sequence.Length, SystemColors.Window, 64, 64);
                }
                else
                {
                    _sequencePictureBox.Image = null;
                }
            }
        }

        private void EditLaunch_Closed(object sender, EventArgs e)
        {
            _isClosed = true;
            _launchListView.Items.Clear();
        }

        private void _launchListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (!_isClosed)
            {
                if (_launchListView.SelectedIndices.Count > 0)
                {
                    _processNameTextBox.Text = Convert.ToString(_launchListView.Items[_launchListView.SelectedIndices[0]].Tag);
                }
                else
                {
                    _processNameTextBox.Text = string.Empty;
                }
            }
        }

        private void EditLaunch_Closing(object sender, CancelEventArgs e)
        {
            if (this.DialogResult != DialogResult.Cancel)
            {
                if (_sequence.Length < 1)
                {
                    e.Cancel = true;
                    MessageBox.Show("Sequence cannot be blank.", "Invalid Sequence", MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
                }
                else if (_processNameTextBox.Text.Length < 1)
                {
                    e.Cancel = true;
                    MessageBox.Show("Action cannot be blank.", "Invalid Action", MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
                }
            }
        }
    }
}
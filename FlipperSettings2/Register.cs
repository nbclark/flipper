using System;

using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MobileSRC.GScroll
{
    internal partial class Register : Form
    {
        private static Microsoft.WindowsCE.Forms.InputPanel _inputPanel = new Microsoft.WindowsCE.Forms.InputPanel();
        public Register()
        {
            InitializeComponent();
            this._splashPicture.Image = Properties.Resources.gscroll_logo;
            _ownerTextBox.Text = GScrollRegistry.GetOwnerName();
            this.DialogResult = DialogResult.Cancel;
        }

        private void _passTextBox_GotFocus(object sender, EventArgs e)
        {
            _inputPanel.Enabled = true;
        }

        private void _passTextBox_LostFocus(object sender, EventArgs e)
        {
            _inputPanel.Enabled = false;
        }

        private void Register_Load(object sender, EventArgs e)
        {
            _passTextBox.Focus();
        }

        private void menuItem1_Click(object sender, EventArgs e)
        {
            MessageBox.Show("GScroll will function normally, but will automatically exist after 15 minutes.", "GScroll Demo", MessageBoxButtons.OK, MessageBoxIcon.Asterisk, MessageBoxDefaultButton.Button1);
            this.Close();
        }

        private void menuItem2_Click(object sender, EventArgs e)
        {
            try
            {
                int registrationCode = Int32.Parse(_passTextBox.Text);
                if (GScrollRegistry.CheckRegistration(registrationCode))
                {
                    GScrollRegistry.SetRegistration(registrationCode);
                    MessageBox.Show("GScroll successfully activated", "GScroll Activated", MessageBoxButtons.OK, MessageBoxIcon.Asterisk, MessageBoxDefaultButton.Button1);

                    this.DialogResult = DialogResult.OK;
                    this.Close();
                    return;
                }
            }
            catch
            {
                //
            }
            MessageBox.Show("Please check your registration code and try again.", "Invalid Code", MessageBoxButtons.OK, MessageBoxIcon.Exclamation, MessageBoxDefaultButton.Button1);
        }
    }
}
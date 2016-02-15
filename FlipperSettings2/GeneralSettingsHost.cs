using System;

using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MobileSRC.GScroll
{
    public partial class GeneralSettingsHost : Form
    {
        public GeneralSettingsHost()
        {
            InitializeComponent();
        }

        public DialogResult ShowDialog(string title, Control settings)
        {
            settings.Dock = DockStyle.Fill;
            this.Controls.Add(settings);
            this.Text = title;

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

        protected override void OnClosed(EventArgs e)
        {
            this.Controls.Clear();
            base.OnClosed(e);
        }
    }
}
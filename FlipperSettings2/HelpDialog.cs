using System;

using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MobileSRC.GScroll
{
    public partial class HelpDialog : Form
    {
        public HelpDialog()
        {
            InitializeComponent();
        }

        public DialogResult ShowDialog(Documentation.Feature feature)
        {
            if (null != feature)
            {
                _helpTitle.Text = feature.Title;
                _webBrowser.DocumentText = feature.Description;
                return base.ShowDialog();
            }
            return DialogResult.Cancel;
        }

        private void _closeMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
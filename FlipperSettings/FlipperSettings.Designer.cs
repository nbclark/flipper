namespace FlipperSettings
{
    partial class FlipperSettings
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.MainMenu mainMenu1;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FlipperSettings));
            this.mainMenu1 = new System.Windows.Forms.MainMenu();
            this._tabControl = new System.Windows.Forms.TabControl();
            this._aboutTabPage = new System.Windows.Forms.TabPage();
            this._versionLabel = new System.Windows.Forms.Label();
            this._aboutPictureBox = new System.Windows.Forms.PictureBox();
            this._mobileSrcLinkLabel = new System.Windows.Forms.LinkLabel();
            this._appearanceTabPage = new System.Windows.Forms.TabPage();
            this.menuItem1 = new System.Windows.Forms.MenuItem();
            this.menuItem2 = new System.Windows.Forms.MenuItem();
            this._itemsTabPage = new System.Windows.Forms.TabPage();
            this._animateCheckbox = new System.Windows.Forms.CheckBox();
            this._tabControl.SuspendLayout();
            this._aboutTabPage.SuspendLayout();
            this._appearanceTabPage.SuspendLayout();
            this.SuspendLayout();
            // 
            // mainMenu1
            // 
            this.mainMenu1.MenuItems.Add(this.menuItem1);
            this.mainMenu1.MenuItems.Add(this.menuItem2);
            // 
            // _tabControl
            // 
            this._tabControl.Controls.Add(this._appearanceTabPage);
            this._tabControl.Controls.Add(this._itemsTabPage);
            this._tabControl.Controls.Add(this._aboutTabPage);
            this._tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this._tabControl.Font = new System.Drawing.Font("Tahoma", 9F, System.Drawing.FontStyle.Bold);
            this._tabControl.Location = new System.Drawing.Point(0, 0);
            this._tabControl.Name = "_tabControl";
            this._tabControl.SelectedIndex = 0;
            this._tabControl.Size = new System.Drawing.Size(480, 536);
            this._tabControl.TabIndex = 1;
            // 
            // _aboutTabPage
            // 
            this._aboutTabPage.Controls.Add(this._versionLabel);
            this._aboutTabPage.Controls.Add(this._aboutPictureBox);
            this._aboutTabPage.Controls.Add(this._mobileSrcLinkLabel);
            this._aboutTabPage.Location = new System.Drawing.Point(0, 0);
            this._aboutTabPage.Name = "_aboutTabPage";
            this._aboutTabPage.Size = new System.Drawing.Size(472, 490);
            this._aboutTabPage.Text = "About";
            // 
            // _versionLabel
            // 
            this._versionLabel.Dock = System.Windows.Forms.DockStyle.Top;
            this._versionLabel.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this._versionLabel.Location = new System.Drawing.Point(0, 0);
            this._versionLabel.Name = "_versionLabel";
            this._versionLabel.Size = new System.Drawing.Size(472, 47);
            this._versionLabel.Text = "mobileSRC Flipper 1.0";
            this._versionLabel.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // _aboutPictureBox
            // 
            this._aboutPictureBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this._aboutPictureBox.Image = ((System.Drawing.Image)(resources.GetObject("_aboutPictureBox.Image")));
            this._aboutPictureBox.Location = new System.Drawing.Point(0, 0);
            this._aboutPictureBox.Name = "_aboutPictureBox";
            this._aboutPictureBox.Size = new System.Drawing.Size(472, 460);
            this._aboutPictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            // 
            // _mobileSrcLinkLabel
            // 
            this._mobileSrcLinkLabel.Dock = System.Windows.Forms.DockStyle.Bottom;
            this._mobileSrcLinkLabel.Font = new System.Drawing.Font("Tahoma", 10F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Underline))));
            this._mobileSrcLinkLabel.Location = new System.Drawing.Point(0, 460);
            this._mobileSrcLinkLabel.Name = "_mobileSrcLinkLabel";
            this._mobileSrcLinkLabel.Size = new System.Drawing.Size(472, 30);
            this._mobileSrcLinkLabel.TabIndex = 6;
            this._mobileSrcLinkLabel.Text = "http://www.mobilesrc.com/";
            this._mobileSrcLinkLabel.TextAlign = System.Drawing.ContentAlignment.TopCenter;
            // 
            // _appearanceTabPage
            // 
            this._appearanceTabPage.Controls.Add(this._animateCheckbox);
            this._appearanceTabPage.Location = new System.Drawing.Point(0, 0);
            this._appearanceTabPage.Name = "_appearanceTabPage";
            this._appearanceTabPage.Size = new System.Drawing.Size(480, 489);
            this._appearanceTabPage.Text = "Appearance";
            // 
            // menuItem1
            // 
            this.menuItem1.Text = "Accept";
            // 
            // menuItem2
            // 
            this.menuItem2.Text = "Cancel";
            // 
            // _itemsTabPage
            // 
            this._itemsTabPage.Location = new System.Drawing.Point(0, 0);
            this._itemsTabPage.Name = "_itemsTabPage";
            this._itemsTabPage.Size = new System.Drawing.Size(472, 490);
            this._itemsTabPage.Text = "Items";
            // 
            // _animateCheckbox
            // 
            this._animateCheckbox.Dock = System.Windows.Forms.DockStyle.Top;
            this._animateCheckbox.Location = new System.Drawing.Point(0, 0);
            this._animateCheckbox.Name = "_animateCheckbox";
            this._animateCheckbox.Size = new System.Drawing.Size(480, 40);
            this._animateCheckbox.TabIndex = 0;
            this._animateCheckbox.Text = "checkBox1";
            // 
            // _flipperSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(192F, 192F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(480, 536);
            this.Controls.Add(this._tabControl);
            this.Location = new System.Drawing.Point(0, 52);
            this.Menu = this.mainMenu1;
            this.Name = "_flipperSettings";
            this.Text = "Flipper Settings";
            this._tabControl.ResumeLayout(false);
            this._aboutTabPage.ResumeLayout(false);
            this._appearanceTabPage.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl _tabControl;
        private System.Windows.Forms.TabPage _aboutTabPage;
        private System.Windows.Forms.Label _versionLabel;
        private System.Windows.Forms.PictureBox _aboutPictureBox;
        private System.Windows.Forms.LinkLabel _mobileSrcLinkLabel;
        private System.Windows.Forms.MenuItem menuItem1;
        private System.Windows.Forms.MenuItem menuItem2;
        private System.Windows.Forms.TabPage _appearanceTabPage;
        private System.Windows.Forms.TabPage _itemsTabPage;
        private System.Windows.Forms.CheckBox _animateCheckbox;
    }
}


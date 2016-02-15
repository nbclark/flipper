namespace MobileSRC.GScroll
{
    partial class GScrollSettings
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(GScrollSettings));
            this._tabControl = new System.Windows.Forms.TabControl();
            this._settingsTabPage = new System.Windows.Forms.TabPage();
            this._launchTabPage = new System.Windows.Forms.TabPage();
            this._appSwitchTabPage = new System.Windows.Forms.TabPage();
            this._perProcessTabPage = new System.Windows.Forms.TabPage();
            this._aboutTabPage = new System.Windows.Forms.TabPage();
            this._versionLabel = new System.Windows.Forms.Label();
            this._aboutPictureBox = new System.Windows.Forms.PictureBox();
            this._mobileSrcLinkLabel = new System.Windows.Forms.LinkLabel();
            this.mainMenu1 = new System.Windows.Forms.MainMenu();
            this.menuItem1 = new System.Windows.Forms.MenuItem();
            this.menuItem2 = new System.Windows.Forms.MenuItem();
            this._registerMenuItem = new System.Windows.Forms.MenuItem();
            this._checkForUpdatesMenuItem = new System.Windows.Forms.MenuItem();
            this._cancelMenuItem = new System.Windows.Forms.MenuItem();
            this._generalSettings = new MobileSRC.GScroll.GeneralSettings();
            this._launchSettings = new MobileSRC.GScroll.LaunchSettings();
            this._appSwitchSettings = new MobileSRC.GScroll.AppSwitchSettings();
            this._perProcessSettings = new MobileSRC.GScroll.PerProcessSettings();
            this._tabControl.SuspendLayout();
            this._settingsTabPage.SuspendLayout();
            this._launchTabPage.SuspendLayout();
            this._appSwitchTabPage.SuspendLayout();
            this._perProcessTabPage.SuspendLayout();
            this._aboutTabPage.SuspendLayout();
            this.SuspendLayout();
            // 
            // _tabControl
            // 
            this._tabControl.Controls.Add(this._settingsTabPage);
            this._tabControl.Controls.Add(this._launchTabPage);
            this._tabControl.Controls.Add(this._appSwitchTabPage);
            this._tabControl.Controls.Add(this._perProcessTabPage);
            this._tabControl.Controls.Add(this._aboutTabPage);
            this._tabControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this._tabControl.Font = new System.Drawing.Font("Tahoma", 10F, System.Drawing.FontStyle.Bold);
            this._tabControl.Location = new System.Drawing.Point(0, 0);
            this._tabControl.Name = "_tabControl";
            this._tabControl.SelectedIndex = 0;
            this._tabControl.Size = new System.Drawing.Size(480, 536);
            this._tabControl.TabIndex = 0;
            // 
            // _settingsTabPage
            // 
            this._settingsTabPage.Controls.Add(this._generalSettings);
            this._settingsTabPage.Location = new System.Drawing.Point(0, 0);
            this._settingsTabPage.Name = "_settingsTabPage";
            this._settingsTabPage.Size = new System.Drawing.Size(480, 485);
            this._settingsTabPage.Text = "Settings";
            // 
            // _launchTabPage
            // 
            this._launchTabPage.Controls.Add(this._launchSettings);
            this._launchTabPage.Location = new System.Drawing.Point(0, 0);
            this._launchTabPage.Name = "_launchTabPage";
            this._launchTabPage.Size = new System.Drawing.Size(472, 490);
            this._launchTabPage.Text = "Launch";
            // 
            // _appSwitchTabPage
            // 
            this._appSwitchTabPage.Controls.Add(this._appSwitchSettings);
            this._appSwitchTabPage.Location = new System.Drawing.Point(0, 0);
            this._appSwitchTabPage.Name = "_appSwitchTabPage";
            this._appSwitchTabPage.Size = new System.Drawing.Size(472, 490);
            this._appSwitchTabPage.Text = "App Switch";
            // 
            // _perProcessTabPage
            // 
            this._perProcessTabPage.Controls.Add(this._perProcessSettings);
            this._perProcessTabPage.Location = new System.Drawing.Point(0, 0);
            this._perProcessTabPage.Name = "_perProcessTabPage";
            this._perProcessTabPage.Size = new System.Drawing.Size(472, 490);
            this._perProcessTabPage.Text = "Per Process";
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
            this._versionLabel.Text = "mobileSRC GScroll 2.0";
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
            this._mobileSrcLinkLabel.Click += new System.EventHandler(this._mobileSrcLinkLabel_Click);
            // 
            // mainMenu1
            // 
            this.mainMenu1.MenuItems.Add(this.menuItem1);
            this.mainMenu1.MenuItems.Add(this.menuItem2);
            // 
            // menuItem1
            // 
            this.menuItem1.Text = "Accept";
            this.menuItem1.Click += new System.EventHandler(this.menuItem1_Click);
            // 
            // menuItem2
            // 
            this.menuItem2.MenuItems.Add(this._registerMenuItem);
            this.menuItem2.MenuItems.Add(this._checkForUpdatesMenuItem);
            this.menuItem2.MenuItems.Add(this._cancelMenuItem);
            this.menuItem2.Text = "Menu";
            // 
            // _registerMenuItem
            // 
            this._registerMenuItem.Text = "Register";
            this._registerMenuItem.Click += new System.EventHandler(this.menuItem3_Click);
            // 
            // _checkForUpdatesMenuItem
            // 
            this._checkForUpdatesMenuItem.Text = "Check for Updates";
            this._checkForUpdatesMenuItem.Click += new System.EventHandler(this._checkForUpdatesMenuItem_Click);
            // 
            // _cancelMenuItem
            // 
            this._cancelMenuItem.Text = "Cancel";
            this._cancelMenuItem.Click += new System.EventHandler(this.menuItem2_Click);
            // 
            // _generalSettings
            // 
            this._generalSettings.Dock = System.Windows.Forms.DockStyle.Fill;
            this._generalSettings.Location = new System.Drawing.Point(0, 0);
            this._generalSettings.Name = "_generalSettings";
            this._generalSettings.Size = new System.Drawing.Size(480, 485);
            this._generalSettings.TabIndex = 0;
            // 
            // _launchSettings
            // 
            this._launchSettings.Dock = System.Windows.Forms.DockStyle.Fill;
            this._launchSettings.Location = new System.Drawing.Point(0, 0);
            this._launchSettings.Name = "_launchSettings";
            this._launchSettings.Size = new System.Drawing.Size(472, 490);
            this._launchSettings.TabIndex = 0;
            // 
            // _appSwitchSettings
            // 
            this._appSwitchSettings.Dock = System.Windows.Forms.DockStyle.Fill;
            this._appSwitchSettings.Location = new System.Drawing.Point(0, 0);
            this._appSwitchSettings.Name = "_appSwitchSettings";
            this._appSwitchSettings.Size = new System.Drawing.Size(472, 490);
            this._appSwitchSettings.TabIndex = 0;
            // 
            // _perProcessSettings
            // 
            this._perProcessSettings.Dock = System.Windows.Forms.DockStyle.Fill;
            this._perProcessSettings.Location = new System.Drawing.Point(0, 0);
            this._perProcessSettings.Name = "_perProcessSettings";
            this._perProcessSettings.Size = new System.Drawing.Size(472, 490);
            this._perProcessSettings.TabIndex = 0;
            // 
            // GScrollSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(192F, 192F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(480, 536);
            this.Controls.Add(this._tabControl);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Location = new System.Drawing.Point(0, 52);
            this.Menu = this.mainMenu1;
            this.MinimizeBox = false;
            this.Name = "GScrollSettings";
            this.Text = "GScroll Settings";
            this.Closed += new System.EventHandler(this.GScrollSettings_Closed);
            this._tabControl.ResumeLayout(false);
            this._settingsTabPage.ResumeLayout(false);
            this._launchTabPage.ResumeLayout(false);
            this._appSwitchTabPage.ResumeLayout(false);
            this._perProcessTabPage.ResumeLayout(false);
            this._aboutTabPage.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        private System.Windows.Forms.TabControl _tabControl;

        #endregion
        private System.Windows.Forms.TabPage _settingsTabPage;
        private System.Windows.Forms.TabPage _launchTabPage;
        private System.Windows.Forms.TabPage _appSwitchTabPage;
        private System.Windows.Forms.TabPage _aboutTabPage;
        private System.Windows.Forms.TabPage _perProcessTabPage;
        private GeneralSettings _generalSettings;
        private LaunchSettings _launchSettings;
        private PerProcessSettings _perProcessSettings;
        private AppSwitchSettings _appSwitchSettings;
        private System.Windows.Forms.MainMenu mainMenu1;
        private System.Windows.Forms.MenuItem menuItem1;
        private System.Windows.Forms.MenuItem menuItem2;
        private System.Windows.Forms.PictureBox _aboutPictureBox;
        private System.Windows.Forms.Label _versionLabel;
        private System.Windows.Forms.LinkLabel _mobileSrcLinkLabel;
        private System.Windows.Forms.MenuItem _registerMenuItem;
        private System.Windows.Forms.MenuItem _cancelMenuItem;
        private System.Windows.Forms.MenuItem _checkForUpdatesMenuItem;
    }
}


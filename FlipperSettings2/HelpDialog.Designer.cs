namespace MobileSRC.GScroll
{
    partial class HelpDialog
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(HelpDialog));
            this._webBrowser = new System.Windows.Forms.WebBrowser();
            this.mainMenu1 = new System.Windows.Forms.MainMenu();
            this._closeMenuItem = new System.Windows.Forms.MenuItem();
            this._helpTitle = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // _webBrowser
            // 
            this._webBrowser.Dock = System.Windows.Forms.DockStyle.Fill;
            this._webBrowser.Location = new System.Drawing.Point(0, 50);
            this._webBrowser.Name = "_webBrowser";
            this._webBrowser.ScriptErrorsSuppressed = true;
            this._webBrowser.Size = new System.Drawing.Size(480, 486);
            // 
            // mainMenu1
            // 
            this.mainMenu1.MenuItems.Add(this._closeMenuItem);
            // 
            // _closeMenuItem
            // 
            this._closeMenuItem.Text = "Close";
            this._closeMenuItem.Click += new System.EventHandler(this._closeMenuItem_Click);
            // 
            // _helpTitle
            // 
            this._helpTitle.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this._helpTitle.Dock = System.Windows.Forms.DockStyle.Top;
            this._helpTitle.Font = new System.Drawing.Font("Tahoma", 14F, System.Drawing.FontStyle.Bold);
            this._helpTitle.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this._helpTitle.Location = new System.Drawing.Point(0, 0);
            this._helpTitle.Name = "_helpTitle";
            this._helpTitle.Size = new System.Drawing.Size(480, 50);
            this._helpTitle.Text = "Help Title";
            // 
            // HelpDialog
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(192F, 192F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(480, 536);
            this.Controls.Add(this._webBrowser);
            this.Controls.Add(this._helpTitle);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Location = new System.Drawing.Point(0, 52);
            this.Menu = this.mainMenu1;
            this.Name = "HelpDialog";
            this.Text = "HelpDialog";
            this.ResumeLayout(false);

        }

        private System.Windows.Forms.WebBrowser _webBrowser;

        #endregion
        private System.Windows.Forms.Label _helpTitle;
        private System.Windows.Forms.MenuItem _closeMenuItem;
    }
}
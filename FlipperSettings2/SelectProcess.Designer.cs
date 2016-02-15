namespace MobileSRC.GScroll
{
    partial class SelectProcess
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SelectProcess));
            this.mainMenu1 = new System.Windows.Forms.MainMenu();
            this.menuItem1 = new System.Windows.Forms.MenuItem();
            this.menuItem2 = new System.Windows.Forms.MenuItem();
            this._processListView = new MobileSRC.GScroll.ApplicationListView();
            this._iconColumnHeader = new System.Windows.Forms.ColumnHeader();
            this._actionColumnHeader = new System.Windows.Forms.ColumnHeader();
            this._processNameLabel = new System.Windows.Forms.Label();
            this._processNameTextBox = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
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
            this.menuItem2.Text = "Cancel";
            this.menuItem2.Click += new System.EventHandler(this.menuItem2_Click);
            // 
            // _processListView
            // 
            this._processListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._processListView.FullRowSelect = true;
            this._processListView.Location = new System.Drawing.Point(3, 50);
            this._processListView.Name = "_processListView";
            this._processListView.Size = new System.Drawing.Size(474, 483);
            this._processListView.TabIndex = 14;
            this._processListView.View = System.Windows.Forms.View.Details;
            this._processListView.SelectedIndexChanged += new System.EventHandler(this._processListView_SelectedIndexChanged);
            // 
            // _iconColumnHeader
            // 
            this._iconColumnHeader.Text = "";
            this._iconColumnHeader.Width = 40;
            // 
            // _actionColumnHeader
            // 
            this._actionColumnHeader.Text = "Launch Action";
            this._actionColumnHeader.Width = 412;
            // 
            // _processNameLabel
            // 
            this._processNameLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._processNameLabel.Location = new System.Drawing.Point(3, 3);
            this._processNameLabel.Name = "_processNameLabel";
            this._processNameLabel.Size = new System.Drawing.Size(183, 41);
            this._processNameLabel.Text = "Process Name";
            this._processNameLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // _processNameTextBox
            // 
            this._processNameTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._processNameTextBox.Location = new System.Drawing.Point(192, 3);
            this._processNameTextBox.Name = "_processNameTextBox";
            this._processNameTextBox.Size = new System.Drawing.Size(285, 41);
            this._processNameTextBox.TabIndex = 15;
            // 
            // SelectProcess
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(192F, 192F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(480, 536);
            this.Controls.Add(this._processNameTextBox);
            this.Controls.Add(this._processNameLabel);
            this.Controls.Add(this._processListView);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Location = new System.Drawing.Point(0, 52);
            this.Menu = this.mainMenu1;
            this.MinimizeBox = false;
            this.Name = "SelectProcess";
            this.Text = "Select Process";
            this.Closing += new System.ComponentModel.CancelEventHandler(this.EditLaunch_Closing);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.MenuItem menuItem1;
        private System.Windows.Forms.MenuItem menuItem2;
        private ApplicationListView _processListView;
        private System.Windows.Forms.ColumnHeader _iconColumnHeader;
        private System.Windows.Forms.ColumnHeader _actionColumnHeader;
        private System.Windows.Forms.Label _processNameLabel;
        private System.Windows.Forms.TextBox _processNameTextBox;
    }
}
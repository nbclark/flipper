namespace MobileSRC.GScroll
{
    partial class LaunchSettings
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this._launchListView = new System.Windows.Forms.ListView();
            this._sequenceColumnHeader = new System.Windows.Forms.ColumnHeader();
            this._actionColumnHeader = new System.Windows.Forms.ColumnHeader();
            this._launchContextMenu = new System.Windows.Forms.ContextMenu();
            this._addLaunchMenuItem = new System.Windows.Forms.MenuItem();
            this._removeLaunchMenuItem = new System.Windows.Forms.MenuItem();
            this._editLaunchMenuItem = new System.Windows.Forms.MenuItem();
            this._actionImageList = new System.Windows.Forms.ImageList();
            this._enableLaunchNotifyCheckBox = new System.Windows.Forms.CheckBox();
            this._launchSettingsDescription = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // _launchListView
            // 
            this._launchListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._launchListView.Columns.Add(this._sequenceColumnHeader);
            this._launchListView.Columns.Add(this._actionColumnHeader);
            this._launchListView.ContextMenu = this._launchContextMenu;
            this._launchListView.FullRowSelect = true;
            this._launchListView.Location = new System.Drawing.Point(3, 70);
            this._launchListView.Name = "_launchListView";
            this._launchListView.Size = new System.Drawing.Size(474, 371);
            this._launchListView.SmallImageList = this._actionImageList;
            this._launchListView.TabIndex = 14;
            this._launchListView.View = System.Windows.Forms.View.Details;
            // 
            // _sequenceColumnHeader
            // 
            this._sequenceColumnHeader.Text = "Sequence";
            this._sequenceColumnHeader.Width = 157;
            // 
            // _actionColumnHeader
            // 
            this._actionColumnHeader.Text = "Launch Action";
            this._actionColumnHeader.Width = 305;
            // 
            // _launchContextMenu
            // 
            this._launchContextMenu.MenuItems.Add(this._addLaunchMenuItem);
            this._launchContextMenu.MenuItems.Add(this._removeLaunchMenuItem);
            this._launchContextMenu.MenuItems.Add(this._editLaunchMenuItem);
            this._launchContextMenu.Popup += new System.EventHandler(this._launchContextMenu_Popup);
            // 
            // _addLaunchMenuItem
            // 
            this._addLaunchMenuItem.Text = "Add Launch";
            this._addLaunchMenuItem.Click += new System.EventHandler(this._addLaunchMenuItem_Click);
            // 
            // _removeLaunchMenuItem
            // 
            this._removeLaunchMenuItem.Text = "Remove Launch";
            this._removeLaunchMenuItem.Click += new System.EventHandler(this._removeLaunchMenuItem_Click);
            // 
            // _editLaunchMenuItem
            // 
            this._editLaunchMenuItem.Text = "Edit Launch";
            this._editLaunchMenuItem.Click += new System.EventHandler(this._editLaunchMenuItem_Click);
            // 
            // _actionImageList
            // 
            this._actionImageList.ImageSize = new System.Drawing.Size(16, 16);
            // 
            // _enableLaunchNotifyCheckBox
            // 
            this._enableLaunchNotifyCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._enableLaunchNotifyCheckBox.Location = new System.Drawing.Point(3, 447);
            this._enableLaunchNotifyCheckBox.Name = "_enableLaunchNotifyCheckBox";
            this._enableLaunchNotifyCheckBox.Size = new System.Drawing.Size(474, 30);
            this._enableLaunchNotifyCheckBox.TabIndex = 15;
            this._enableLaunchNotifyCheckBox.Text = "Vibrate on Application Launch";
            // 
            // _launchSettingsDescription
            // 
            this._launchSettingsDescription.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._launchSettingsDescription.Location = new System.Drawing.Point(3, 3);
            this._launchSettingsDescription.Name = "_launchSettingsDescription";
            this._launchSettingsDescription.Size = new System.Drawing.Size(474, 64);
            this._launchSettingsDescription.Text = "Specify sequences of taps to launch applications.";
            // 
            // LaunchSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(192F, 192F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.Controls.Add(this._launchSettingsDescription);
            this.Controls.Add(this._enableLaunchNotifyCheckBox);
            this.Controls.Add(this._launchListView);
            this.Name = "LaunchSettings";
            this.Size = new System.Drawing.Size(480, 480);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListView _launchListView;
        private System.Windows.Forms.ColumnHeader _sequenceColumnHeader;
        private System.Windows.Forms.ColumnHeader _actionColumnHeader;
        private System.Windows.Forms.ContextMenu _launchContextMenu;
        private System.Windows.Forms.MenuItem _addLaunchMenuItem;
        private System.Windows.Forms.MenuItem _removeLaunchMenuItem;
        private System.Windows.Forms.MenuItem _editLaunchMenuItem;
        private System.Windows.Forms.ImageList _actionImageList;
        private System.Windows.Forms.CheckBox _enableLaunchNotifyCheckBox;
        private System.Windows.Forms.Label _launchSettingsDescription;
    }
}

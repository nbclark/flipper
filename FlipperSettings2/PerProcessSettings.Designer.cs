namespace MobileSRC.GScroll
{
    partial class PerProcessSettings
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
            this._processContextMenu = new System.Windows.Forms.ContextMenu();
            this._customizeProcessMenuItem = new System.Windows.Forms.MenuItem();
            this._removeProcessMenuItem = new System.Windows.Forms.MenuItem();
            this._editProcessMenuItem = new System.Windows.Forms.MenuItem();
            this._perProcessListView = new System.Windows.Forms.ListView();
            this._processColumnHeader = new System.Windows.Forms.ColumnHeader();
            this._availableLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // _processContextMenu
            // 
            this._processContextMenu.MenuItems.Add(this._customizeProcessMenuItem);
            this._processContextMenu.MenuItems.Add(this._removeProcessMenuItem);
            this._processContextMenu.MenuItems.Add(this._editProcessMenuItem);
            this._processContextMenu.Popup += new System.EventHandler(this._processContextMenu_Popup);
            // 
            // _customizeProcessMenuItem
            // 
            this._customizeProcessMenuItem.Text = "Add Process";
            this._customizeProcessMenuItem.Click += new System.EventHandler(this._customizeProcessMenuItem_Click);
            // 
            // _removeProcessMenuItem
            // 
            this._removeProcessMenuItem.Text = "Remove Process";
            this._removeProcessMenuItem.Click += new System.EventHandler(this._removeProcessMenuItem_Click);
            // 
            // _editProcessMenuItem
            // 
            this._editProcessMenuItem.Text = "Edit Process";
            this._editProcessMenuItem.Click += new System.EventHandler(this._editProcessMenuItem_Click);
            // 
            // _perProcessListView
            // 
            this._perProcessListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._perProcessListView.Columns.Add(this._processColumnHeader);
            this._perProcessListView.ContextMenu = this._processContextMenu;
            this._perProcessListView.FullRowSelect = true;
            this._perProcessListView.Location = new System.Drawing.Point(3, 134);
            this._perProcessListView.Name = "_perProcessListView";
            this._perProcessListView.Size = new System.Drawing.Size(474, 343);
            this._perProcessListView.TabIndex = 13;
            this._perProcessListView.View = System.Windows.Forms.View.Details;
            // 
            // _processColumnHeader
            // 
            this._processColumnHeader.Text = "Process Name";
            this._processColumnHeader.Width = 460;
            // 
            // _availableLabel
            // 
            this._availableLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._availableLabel.Location = new System.Drawing.Point(3, 0);
            this._availableLabel.Name = "_availableLabel";
            this._availableLabel.Size = new System.Drawing.Size(474, 131);
            this._availableLabel.Text = "GScroll allows for complete customization of individual applications.  Simply add" +
                " a process to the list and specify the settings you want for just that applicati" +
                "on.\r\n";
            // 
            // PerProcessSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(192F, 192F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.Controls.Add(this._availableLabel);
            this.Controls.Add(this._perProcessListView);
            this.Name = "PerProcessSettings";
            this.Size = new System.Drawing.Size(480, 480);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ContextMenu _processContextMenu;
        private System.Windows.Forms.MenuItem _customizeProcessMenuItem;
        private System.Windows.Forms.MenuItem _removeProcessMenuItem;
        private System.Windows.Forms.MenuItem _editProcessMenuItem;
        private System.Windows.Forms.ListView _perProcessListView;
        private System.Windows.Forms.ColumnHeader _processColumnHeader;
        private System.Windows.Forms.Label _availableLabel;
    }
}

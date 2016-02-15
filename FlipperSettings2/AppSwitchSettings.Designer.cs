namespace MobileSRC.GScroll
{
    partial class AppSwitchSettings
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
            this._enableAutoCloseCheckBox = new System.Windows.Forms.CheckBox();
            this._autoCloseDelayUpDown = new System.Windows.Forms.NumericUpDown();
            this._enableShrinkAnimationCheckBox = new System.Windows.Forms.CheckBox();
            this._autoCloseDescription = new System.Windows.Forms.Label();
            this._enableThumbnailsCheckBox = new System.Windows.Forms.CheckBox();
            this._reservedProcessListView = new System.Windows.Forms.ListView();
            this._processColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this._processContextMenu = new System.Windows.Forms.ContextMenu();
            this._customizeProcessMenuItem = new System.Windows.Forms.MenuItem();
            this._removeProcessMenuItem = new System.Windows.Forms.MenuItem();
            this._editProcessMenuItem = new System.Windows.Forms.MenuItem();
            this._enableScrollAnimationCheckBox = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // _enableAutoCloseCheckBox
            // 
            this._enableAutoCloseCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._enableAutoCloseCheckBox.Location = new System.Drawing.Point(3, 3);
            this._enableAutoCloseCheckBox.Name = "_enableAutoCloseCheckBox";
            this._enableAutoCloseCheckBox.Size = new System.Drawing.Size(474, 30);
            this._enableAutoCloseCheckBox.TabIndex = 14;
            this._enableAutoCloseCheckBox.Text = "Enable AppSwitch Auto-Select";
            this._enableAutoCloseCheckBox.CheckStateChanged += new System.EventHandler(this._enableAutoCloseCheckBox_CheckStateChanged);
            // 
            // _autoCloseDelayUpDown
            // 
            this._autoCloseDelayUpDown.Enabled = false;
            this._autoCloseDelayUpDown.Location = new System.Drawing.Point(52, 39);
            this._autoCloseDelayUpDown.Name = "_autoCloseDelayUpDown";
            this._autoCloseDelayUpDown.Size = new System.Drawing.Size(150, 36);
            this._autoCloseDelayUpDown.TabIndex = 15;
            this._autoCloseDelayUpDown.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // _enableShrinkAnimationCheckBox
            // 
            this._enableShrinkAnimationCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._enableShrinkAnimationCheckBox.Location = new System.Drawing.Point(3, 81);
            this._enableShrinkAnimationCheckBox.Name = "_enableShrinkAnimationCheckBox";
            this._enableShrinkAnimationCheckBox.Size = new System.Drawing.Size(474, 30);
            this._enableShrinkAnimationCheckBox.TabIndex = 16;
            this._enableShrinkAnimationCheckBox.Text = "Enable Fade In/Out Animations";
            // 
            // _autoCloseDescription
            // 
            this._autoCloseDescription.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._autoCloseDescription.Enabled = false;
            this._autoCloseDescription.Location = new System.Drawing.Point(217, 41);
            this._autoCloseDescription.Name = "_autoCloseDescription";
            this._autoCloseDescription.Size = new System.Drawing.Size(260, 36);
            this._autoCloseDescription.Text = "(delay in seconds)";
            // 
            // _enableThumbnailsCheckBox
            // 
            this._enableThumbnailsCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._enableThumbnailsCheckBox.Location = new System.Drawing.Point(3, 153);
            this._enableThumbnailsCheckBox.Name = "_enableThumbnailsCheckBox";
            this._enableThumbnailsCheckBox.Size = new System.Drawing.Size(474, 30);
            this._enableThumbnailsCheckBox.TabIndex = 17;
            this._enableThumbnailsCheckBox.Text = "Enable Thumbnails";
            // 
            // _reservedProcessListView
            // 
            this._reservedProcessListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._reservedProcessListView.Columns.Add(this._processColumnHeader);
            this._reservedProcessListView.Columns.Add(this.columnHeader1);
            this._reservedProcessListView.Columns.Add(this.columnHeader2);
            this._reservedProcessListView.Columns.Add(this.columnHeader3);
            this._reservedProcessListView.ContextMenu = this._processContextMenu;
            this._reservedProcessListView.FullRowSelect = true;
            this._reservedProcessListView.Location = new System.Drawing.Point(3, 208);
            this._reservedProcessListView.Name = "_reservedProcessListView";
            this._reservedProcessListView.Size = new System.Drawing.Size(474, 269);
            this._reservedProcessListView.TabIndex = 18;
            this._reservedProcessListView.View = System.Windows.Forms.View.Details;
            // 
            // _processColumnHeader
            // 
            this._processColumnHeader.Text = "Process Name";
            this._processColumnHeader.Width = 180;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Critical";
            this.columnHeader1.Width = 80;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Terminate";
            this.columnHeader2.Width = 110;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Minimize";
            this.columnHeader3.Width = 100;
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
            // _enableScrollAnimationCheckBox
            // 
            this._enableScrollAnimationCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._enableScrollAnimationCheckBox.Location = new System.Drawing.Point(3, 117);
            this._enableScrollAnimationCheckBox.Name = "_enableScrollAnimationCheckBox";
            this._enableScrollAnimationCheckBox.Size = new System.Drawing.Size(474, 30);
            this._enableScrollAnimationCheckBox.TabIndex = 20;
            this._enableScrollAnimationCheckBox.Text = "Enable Scroll Animations";
            // 
            // AppSwitchSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(192F, 192F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.Controls.Add(this._enableScrollAnimationCheckBox);
            this.Controls.Add(this._reservedProcessListView);
            this.Controls.Add(this._enableThumbnailsCheckBox);
            this.Controls.Add(this._autoCloseDescription);
            this.Controls.Add(this._enableShrinkAnimationCheckBox);
            this.Controls.Add(this._autoCloseDelayUpDown);
            this.Controls.Add(this._enableAutoCloseCheckBox);
            this.Name = "AppSwitchSettings";
            this.Size = new System.Drawing.Size(480, 480);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.CheckBox _enableAutoCloseCheckBox;
        private System.Windows.Forms.NumericUpDown _autoCloseDelayUpDown;
        private System.Windows.Forms.CheckBox _enableShrinkAnimationCheckBox;
        private System.Windows.Forms.Label _autoCloseDescription;
        private System.Windows.Forms.CheckBox _enableThumbnailsCheckBox;
        private System.Windows.Forms.ListView _reservedProcessListView;
        private System.Windows.Forms.ColumnHeader _processColumnHeader;
        private System.Windows.Forms.ContextMenu _processContextMenu;
        private System.Windows.Forms.MenuItem _customizeProcessMenuItem;
        private System.Windows.Forms.MenuItem _removeProcessMenuItem;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.MenuItem _editProcessMenuItem;
        private System.Windows.Forms.CheckBox _enableScrollAnimationCheckBox;
    }
}

namespace MobileSRC.GScroll
{
    partial class EditLaunch
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(EditLaunch));
            this.mainMenu1 = new System.Windows.Forms.MainMenu();
            this.menuItem1 = new System.Windows.Forms.MenuItem();
            this.menuItem2 = new System.Windows.Forms.MenuItem();
            this._availablePictureBox = new System.Windows.Forms.PictureBox();
            this._availableLabel = new System.Windows.Forms.Label();
            this._sequenceLabel = new System.Windows.Forms.Label();
            this._sequencePictureBox = new System.Windows.Forms.PictureBox();
            this._processNameTextBox = new System.Windows.Forms.TextBox();
            this._processNameLabel = new System.Windows.Forms.Label();
            this._launchListView = new MobileSRC.GScroll.ApplicationListView();
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
            // _availablePictureBox
            // 
            this._availablePictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this._availablePictureBox.Location = new System.Drawing.Point(145, 3);
            this._availablePictureBox.Name = "_availablePictureBox";
            this._availablePictureBox.Size = new System.Drawing.Size(332, 64);
            this._availablePictureBox.Click += new System.EventHandler(this._availablePictureBox_Click);
            this._availablePictureBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this._availablePictureBox_MouseDown);
            // 
            // _availableLabel
            // 
            this._availableLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._availableLabel.Location = new System.Drawing.Point(3, 19);
            this._availableLabel.Name = "_availableLabel";
            this._availableLabel.Size = new System.Drawing.Size(136, 32);
            this._availableLabel.Text = "Available";
            this._availableLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // _sequenceLabel
            // 
            this._sequenceLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._sequenceLabel.Location = new System.Drawing.Point(3, 89);
            this._sequenceLabel.Name = "_sequenceLabel";
            this._sequenceLabel.Size = new System.Drawing.Size(136, 32);
            this._sequenceLabel.Text = "Sequence";
            this._sequenceLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // _sequencePictureBox
            // 
            this._sequencePictureBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this._sequencePictureBox.Location = new System.Drawing.Point(145, 73);
            this._sequencePictureBox.Name = "_sequencePictureBox";
            this._sequencePictureBox.Size = new System.Drawing.Size(332, 64);
            this._sequencePictureBox.MouseDown += new System.Windows.Forms.MouseEventHandler(this._sequencePictureBox_MouseDown);
            // 
            // _processNameTextBox
            // 
            this._processNameTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._processNameTextBox.Location = new System.Drawing.Point(145, 143);
            this._processNameTextBox.Name = "_processNameTextBox";
            this._processNameTextBox.Size = new System.Drawing.Size(332, 41);
            this._processNameTextBox.TabIndex = 17;
            // 
            // _processNameLabel
            // 
            this._processNameLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._processNameLabel.Location = new System.Drawing.Point(6, 143);
            this._processNameLabel.Name = "_processNameLabel";
            this._processNameLabel.Size = new System.Drawing.Size(133, 41);
            this._processNameLabel.Text = "Action";
            this._processNameLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // _launchListView
            // 
            this._launchListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._launchListView.Location = new System.Drawing.Point(6, 190);
            this._launchListView.Name = "_launchListView";
            this._launchListView.Size = new System.Drawing.Size(471, 343);
            this._launchListView.TabIndex = 1;
            this._launchListView.SelectedIndexChanged += new System.EventHandler(this._launchListView_SelectedIndexChanged);
            // 
            // EditLaunch
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(192F, 192F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(480, 536);
            this.Controls.Add(this._processNameTextBox);
            this.Controls.Add(this._processNameLabel);
            this.Controls.Add(this._sequenceLabel);
            this.Controls.Add(this._sequencePictureBox);
            this.Controls.Add(this._availableLabel);
            this.Controls.Add(this._launchListView);
            this.Controls.Add(this._availablePictureBox);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Location = new System.Drawing.Point(0, 52);
            this.Menu = this.mainMenu1;
            this.MinimizeBox = false;
            this.Name = "EditLaunch";
            this.Text = "Edit Launch Action";
            this.Load += new System.EventHandler(this.EditLaunch_Load);
            this.Closed += new System.EventHandler(this.EditLaunch_Closed);
            this.Closing += new System.ComponentModel.CancelEventHandler(this.EditLaunch_Closing);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.MenuItem menuItem1;
        private System.Windows.Forms.MenuItem menuItem2;
        private System.Windows.Forms.PictureBox _availablePictureBox;
        private ApplicationListView _launchListView;
        private System.Windows.Forms.Label _availableLabel;
        private System.Windows.Forms.Label _sequenceLabel;
        private System.Windows.Forms.PictureBox _sequencePictureBox;
        private System.Windows.Forms.TextBox _processNameTextBox;
        private System.Windows.Forms.Label _processNameLabel;
    }
}
namespace MobileSRC.GScroll
{
    partial class AppSwitchProcessSettings
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
            this._ignoreCheckBox = new System.Windows.Forms.CheckBox();
            this._terminateCheckBox = new System.Windows.Forms.CheckBox();
            this._minimizeCheckBox = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // _ignoreCheckBox
            // 
            this._ignoreCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._ignoreCheckBox.Location = new System.Drawing.Point(3, 3);
            this._ignoreCheckBox.Name = "_ignoreCheckBox";
            this._ignoreCheckBox.Size = new System.Drawing.Size(474, 30);
            this._ignoreCheckBox.TabIndex = 15;
            this._ignoreCheckBox.Text = "Ignore Process in AppSwitch";
            // 
            // _terminateCheckBox
            // 
            this._terminateCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._terminateCheckBox.Location = new System.Drawing.Point(3, 39);
            this._terminateCheckBox.Name = "_terminateCheckBox";
            this._terminateCheckBox.Size = new System.Drawing.Size(474, 30);
            this._terminateCheckBox.TabIndex = 16;
            this._terminateCheckBox.Text = "Close Won\'t Terminate";
            // 
            // _minimizeCheckBox
            // 
            this._minimizeCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._minimizeCheckBox.Location = new System.Drawing.Point(3, 75);
            this._minimizeCheckBox.Name = "_minimizeCheckBox";
            this._minimizeCheckBox.Size = new System.Drawing.Size(474, 30);
            this._minimizeCheckBox.TabIndex = 17;
            this._minimizeCheckBox.Text = "Close Won\'t Minimize";
            // 
            // AppSwitchProcessSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(192F, 192F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.Controls.Add(this._minimizeCheckBox);
            this.Controls.Add(this._terminateCheckBox);
            this.Controls.Add(this._ignoreCheckBox);
            this.Name = "AppSwitchProcessSettings";
            this.Size = new System.Drawing.Size(480, 480);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.CheckBox _ignoreCheckBox;
        private System.Windows.Forms.CheckBox _terminateCheckBox;
        private System.Windows.Forms.CheckBox _minimizeCheckBox;
    }
}

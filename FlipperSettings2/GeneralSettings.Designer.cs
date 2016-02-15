namespace MobileSRC.GScroll
{
    partial class GeneralSettings
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
            this._panelSwipeLinkLabel = new System.Windows.Forms.LinkLabel();
            this._enableScrollWheelCheckBox = new System.Windows.Forms.CheckBox();
            this._helpLinkButton = new System.Windows.Forms.LinkLabel();
            this._enableGameModeLinkLabel = new System.Windows.Forms.LinkLabel();
            this._enableGameModeCheckBox = new System.Windows.Forms.CheckBox();
            this._isReservedLinkLabel = new System.Windows.Forms.LinkLabel();
            this._isReservedCheckBox = new System.Windows.Forms.CheckBox();
            this._enablePrecisionModeLinkLabel = new System.Windows.Forms.LinkLabel();
            this._enablePrecisionModeCheckBox = new System.Windows.Forms.CheckBox();
            this._gameModeRepeatDescription = new System.Windows.Forms.Label();
            this._gameModeRepeatUpDown = new System.Windows.Forms.NumericUpDown();
            this._precisionScrollDescription = new System.Windows.Forms.Label();
            this._precisionScrollUpDown = new System.Windows.Forms.NumericUpDown();
            this._precisionNavigateDescription = new System.Windows.Forms.Label();
            this._precisionNavigateUpDown = new System.Windows.Forms.NumericUpDown();
            this._panelSwipeComboBox = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this._tapTiltLinkLabel = new System.Windows.Forms.LinkLabel();
            this._tapTiltComoBox = new System.Windows.Forms.ComboBox();
            this._smoothScrollLinkLabel = new System.Windows.Forms.LinkLabel();
            this._smoothScrollCheckBox = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // _panelSwipeLinkLabel
            // 
            this._panelSwipeLinkLabel.Location = new System.Drawing.Point(372, 325);
            this._panelSwipeLinkLabel.Name = "_panelSwipeLinkLabel";
            this._panelSwipeLinkLabel.Size = new System.Drawing.Size(93, 30);
            this._panelSwipeLinkLabel.TabIndex = 13;
            this._panelSwipeLinkLabel.Tag = "PanelSwipeMode";
            this._panelSwipeLinkLabel.Text = "Help";
            this._panelSwipeLinkLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
            this._panelSwipeLinkLabel.Click += new System.EventHandler(this.helpLabel_Click);
            // 
            // _enableScrollWheelCheckBox
            // 
            this._enableScrollWheelCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._enableScrollWheelCheckBox.Location = new System.Drawing.Point(3, 44);
            this._enableScrollWheelCheckBox.Name = "_enableScrollWheelCheckBox";
            this._enableScrollWheelCheckBox.Size = new System.Drawing.Size(359, 30);
            this._enableScrollWheelCheckBox.TabIndex = 2;
            this._enableScrollWheelCheckBox.Text = "Enable Scroll Wheel";
            // 
            // _helpLinkButton
            // 
            this._helpLinkButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this._helpLinkButton.Location = new System.Drawing.Point(372, 44);
            this._helpLinkButton.Name = "_helpLinkButton";
            this._helpLinkButton.Size = new System.Drawing.Size(93, 30);
            this._helpLinkButton.TabIndex = 3;
            this._helpLinkButton.Tag = "EnableScrollWheel";
            this._helpLinkButton.Text = "Help";
            this._helpLinkButton.TextAlign = System.Drawing.ContentAlignment.TopRight;
            this._helpLinkButton.Click += new System.EventHandler(this.helpLabel_Click);
            // 
            // _enableGameModeLinkLabel
            // 
            this._enableGameModeLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this._enableGameModeLinkLabel.Location = new System.Drawing.Point(372, 203);
            this._enableGameModeLinkLabel.Name = "_enableGameModeLinkLabel";
            this._enableGameModeLinkLabel.Size = new System.Drawing.Size(93, 30);
            this._enableGameModeLinkLabel.TabIndex = 9;
            this._enableGameModeLinkLabel.Tag = "EnableGameMode";
            this._enableGameModeLinkLabel.Text = "Help";
            this._enableGameModeLinkLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
            this._enableGameModeLinkLabel.Click += new System.EventHandler(this.helpLabel_Click);
            // 
            // _enableGameModeCheckBox
            // 
            this._enableGameModeCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._enableGameModeCheckBox.Location = new System.Drawing.Point(3, 203);
            this._enableGameModeCheckBox.Name = "_enableGameModeCheckBox";
            this._enableGameModeCheckBox.Size = new System.Drawing.Size(359, 30);
            this._enableGameModeCheckBox.TabIndex = 8;
            this._enableGameModeCheckBox.Text = "Enable Game Mode";
            this._enableGameModeCheckBox.CheckStateChanged += new System.EventHandler(this._enableGameModeCheckBox_CheckStateChanged);
            // 
            // _isReservedLinkLabel
            // 
            this._isReservedLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this._isReservedLinkLabel.Location = new System.Drawing.Point(372, 3);
            this._isReservedLinkLabel.Name = "_isReservedLinkLabel";
            this._isReservedLinkLabel.Size = new System.Drawing.Size(93, 30);
            this._isReservedLinkLabel.TabIndex = 1;
            this._isReservedLinkLabel.Tag = "ReservedProcess";
            this._isReservedLinkLabel.Text = "Help";
            this._isReservedLinkLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
            this._isReservedLinkLabel.Click += new System.EventHandler(this.helpLabel_Click);
            // 
            // _isReservedCheckBox
            // 
            this._isReservedCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._isReservedCheckBox.Location = new System.Drawing.Point(3, 3);
            this._isReservedCheckBox.Name = "_isReservedCheckBox";
            this._isReservedCheckBox.Size = new System.Drawing.Size(359, 30);
            this._isReservedCheckBox.TabIndex = 0;
            this._isReservedCheckBox.Text = "Reserved Process";
            this._isReservedCheckBox.CheckStateChanged += new System.EventHandler(this._isReservedCheckBox_CheckStateChanged);
            // 
            // _enablePrecisionModeLinkLabel
            // 
            this._enablePrecisionModeLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this._enablePrecisionModeLinkLabel.Location = new System.Drawing.Point(372, 85);
            this._enablePrecisionModeLinkLabel.Name = "_enablePrecisionModeLinkLabel";
            this._enablePrecisionModeLinkLabel.Size = new System.Drawing.Size(93, 30);
            this._enablePrecisionModeLinkLabel.TabIndex = 5;
            this._enablePrecisionModeLinkLabel.Tag = "EnablePrecisionMode";
            this._enablePrecisionModeLinkLabel.Text = "Help";
            this._enablePrecisionModeLinkLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
            this._enablePrecisionModeLinkLabel.Click += new System.EventHandler(this.helpLabel_Click);
            // 
            // _enablePrecisionModeCheckBox
            // 
            this._enablePrecisionModeCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._enablePrecisionModeCheckBox.Location = new System.Drawing.Point(3, 85);
            this._enablePrecisionModeCheckBox.Name = "_enablePrecisionModeCheckBox";
            this._enablePrecisionModeCheckBox.Size = new System.Drawing.Size(359, 30);
            this._enablePrecisionModeCheckBox.TabIndex = 4;
            this._enablePrecisionModeCheckBox.Text = "Enable Precision Mode";
            this._enablePrecisionModeCheckBox.CheckStateChanged += new System.EventHandler(this._enablePrecisionModeCheckBox_CheckStateChanged);
            // 
            // _gameModeRepeatDescription
            // 
            this._gameModeRepeatDescription.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._gameModeRepeatDescription.Enabled = false;
            this._gameModeRepeatDescription.Location = new System.Drawing.Point(172, 244);
            this._gameModeRepeatDescription.Name = "_gameModeRepeatDescription";
            this._gameModeRepeatDescription.Size = new System.Drawing.Size(289, 36);
            this._gameModeRepeatDescription.Text = "(key-repeat in milliseconds)";
            // 
            // _gameModeRepeatUpDown
            // 
            this._gameModeRepeatUpDown.Enabled = false;
            this._gameModeRepeatUpDown.Location = new System.Drawing.Point(51, 244);
            this._gameModeRepeatUpDown.Maximum = new decimal(new int[] {
            2000,
            0,
            0,
            0});
            this._gameModeRepeatUpDown.Name = "_gameModeRepeatUpDown";
            this._gameModeRepeatUpDown.Size = new System.Drawing.Size(115, 36);
            this._gameModeRepeatUpDown.TabIndex = 10;
            this._gameModeRepeatUpDown.Value = new decimal(new int[] {
            2000,
            0,
            0,
            0});
            // 
            // _precisionScrollDescription
            // 
            this._precisionScrollDescription.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._precisionScrollDescription.Enabled = false;
            this._precisionScrollDescription.Location = new System.Drawing.Point(172, 118);
            this._precisionScrollDescription.Name = "_precisionScrollDescription";
            this._precisionScrollDescription.Size = new System.Drawing.Size(300, 36);
            this._precisionScrollDescription.Text = "(trigger of scroll event)";
            // 
            // _precisionScrollUpDown
            // 
            this._precisionScrollUpDown.Enabled = false;
            this._precisionScrollUpDown.Location = new System.Drawing.Point(51, 118);
            this._precisionScrollUpDown.Maximum = new decimal(new int[] {
            75,
            0,
            0,
            0});
            this._precisionScrollUpDown.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this._precisionScrollUpDown.Name = "_precisionScrollUpDown";
            this._precisionScrollUpDown.Size = new System.Drawing.Size(115, 36);
            this._precisionScrollUpDown.TabIndex = 6;
            this._precisionScrollUpDown.Value = new decimal(new int[] {
            75,
            0,
            0,
            0});
            // 
            // _precisionNavigateDescription
            // 
            this._precisionNavigateDescription.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._precisionNavigateDescription.Enabled = false;
            this._precisionNavigateDescription.Location = new System.Drawing.Point(172, 159);
            this._precisionNavigateDescription.Name = "_precisionNavigateDescription";
            this._precisionNavigateDescription.Size = new System.Drawing.Size(300, 36);
            this._precisionNavigateDescription.Text = "(trigger of d-pad event)";
            // 
            // _precisionNavigateUpDown
            // 
            this._precisionNavigateUpDown.Enabled = false;
            this._precisionNavigateUpDown.Location = new System.Drawing.Point(51, 159);
            this._precisionNavigateUpDown.Maximum = new decimal(new int[] {
            75,
            0,
            0,
            0});
            this._precisionNavigateUpDown.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this._precisionNavigateUpDown.Name = "_precisionNavigateUpDown";
            this._precisionNavigateUpDown.Size = new System.Drawing.Size(115, 36);
            this._precisionNavigateUpDown.TabIndex = 7;
            this._precisionNavigateUpDown.Value = new decimal(new int[] {
            75,
            0,
            0,
            0});
            // 
            // _panelSwipeComboBox
            // 
            this._panelSwipeComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._panelSwipeComboBox.Location = new System.Drawing.Point(3, 358);
            this._panelSwipeComboBox.Name = "_panelSwipeComboBox";
            this._panelSwipeComboBox.Size = new System.Drawing.Size(462, 41);
            this._panelSwipeComboBox.TabIndex = 14;
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(3, 323);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(363, 32);
            this.label1.Text = "Panel Swipe Mode";
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(3, 402);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(363, 32);
            this.label2.Text = "Tap && Tilt Mode";
            // 
            // _tapTiltLinkLabel
            // 
            this._tapTiltLinkLabel.Location = new System.Drawing.Point(372, 404);
            this._tapTiltLinkLabel.Name = "_tapTiltLinkLabel";
            this._tapTiltLinkLabel.Size = new System.Drawing.Size(93, 30);
            this._tapTiltLinkLabel.TabIndex = 15;
            this._tapTiltLinkLabel.Tag = "TapTiltMode";
            this._tapTiltLinkLabel.Text = "Help";
            this._tapTiltLinkLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
            this._tapTiltLinkLabel.Click += new System.EventHandler(this.helpLabel_Click);
            // 
            // _tapTiltComoBox
            // 
            this._tapTiltComoBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._tapTiltComoBox.Location = new System.Drawing.Point(3, 437);
            this._tapTiltComoBox.Name = "_tapTiltComoBox";
            this._tapTiltComoBox.Size = new System.Drawing.Size(462, 41);
            this._tapTiltComoBox.TabIndex = 16;
            // 
            // _smoothScrollLinkLabel
            // 
            this._smoothScrollLinkLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this._smoothScrollLinkLabel.Location = new System.Drawing.Point(372, 286);
            this._smoothScrollLinkLabel.Name = "_smoothScrollLinkLabel";
            this._smoothScrollLinkLabel.Size = new System.Drawing.Size(93, 30);
            this._smoothScrollLinkLabel.TabIndex = 12;
            this._smoothScrollLinkLabel.Tag = "SmoothScrollMode";
            this._smoothScrollLinkLabel.Text = "Help";
            this._smoothScrollLinkLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
            this._smoothScrollLinkLabel.Click += new System.EventHandler(this.helpLabel_Click);
            // 
            // _smoothScrollCheckBox
            // 
            this._smoothScrollCheckBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this._smoothScrollCheckBox.Location = new System.Drawing.Point(3, 286);
            this._smoothScrollCheckBox.Name = "_smoothScrollCheckBox";
            this._smoothScrollCheckBox.Size = new System.Drawing.Size(359, 30);
            this._smoothScrollCheckBox.TabIndex = 11;
            this._smoothScrollCheckBox.Text = "Enable \"Opera\" Scroll Mode";
            // 
            // GeneralSettings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(192F, 192F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.AutoScroll = true;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.Controls.Add(this._smoothScrollLinkLabel);
            this.Controls.Add(this._smoothScrollCheckBox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this._tapTiltLinkLabel);
            this.Controls.Add(this._tapTiltComoBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this._precisionNavigateDescription);
            this.Controls.Add(this._panelSwipeLinkLabel);
            this.Controls.Add(this._panelSwipeComboBox);
            this.Controls.Add(this._precisionNavigateUpDown);
            this.Controls.Add(this._precisionScrollDescription);
            this.Controls.Add(this._precisionScrollUpDown);
            this.Controls.Add(this._gameModeRepeatDescription);
            this.Controls.Add(this._gameModeRepeatUpDown);
            this.Controls.Add(this._enablePrecisionModeLinkLabel);
            this.Controls.Add(this._enablePrecisionModeCheckBox);
            this.Controls.Add(this._isReservedLinkLabel);
            this.Controls.Add(this._isReservedCheckBox);
            this.Controls.Add(this._enableGameModeLinkLabel);
            this.Controls.Add(this._enableGameModeCheckBox);
            this.Controls.Add(this._helpLinkButton);
            this.Controls.Add(this._enableScrollWheelCheckBox);
            this.Name = "GeneralSettings";
            this.Size = new System.Drawing.Size(480, 527);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.CheckBox _enableScrollWheelCheckBox;
        private System.Windows.Forms.LinkLabel _helpLinkButton;
        private System.Windows.Forms.LinkLabel _enableGameModeLinkLabel;
        private System.Windows.Forms.CheckBox _enableGameModeCheckBox;
        private System.Windows.Forms.LinkLabel _isReservedLinkLabel;
        private System.Windows.Forms.CheckBox _isReservedCheckBox;
        private System.Windows.Forms.LinkLabel _enablePrecisionModeLinkLabel;
        private System.Windows.Forms.CheckBox _enablePrecisionModeCheckBox;
        private System.Windows.Forms.Label _gameModeRepeatDescription;
        private System.Windows.Forms.NumericUpDown _gameModeRepeatUpDown;
        private System.Windows.Forms.Label _precisionScrollDescription;
        private System.Windows.Forms.NumericUpDown _precisionScrollUpDown;
        private System.Windows.Forms.Label _precisionNavigateDescription;
        private System.Windows.Forms.NumericUpDown _precisionNavigateUpDown;
        private System.Windows.Forms.LinkLabel _panelSwipeLinkLabel;
        private System.Windows.Forms.ComboBox _panelSwipeComboBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.LinkLabel _tapTiltLinkLabel;
        private System.Windows.Forms.ComboBox _tapTiltComoBox;
        private System.Windows.Forms.LinkLabel _smoothScrollLinkLabel;
        private System.Windows.Forms.CheckBox _smoothScrollCheckBox;

    }
}

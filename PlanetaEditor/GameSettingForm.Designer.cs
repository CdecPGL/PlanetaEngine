namespace PlanetaEditor
{
	partial class GameSettingForm
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
			this.titleLabel = new System.Windows.Forms.Label();
			this.titleTextBox = new System.Windows.Forms.TextBox();
			this.internalSizeLabel = new System.Windows.Forms.Label();
			this.internalWidthBox = new System.Windows.Forms.NumericUpDown();
			this.internalHeightBox = new System.Windows.Forms.NumericUpDown();
			this.drawSizeLabel = new System.Windows.Forms.Label();
			this.drawHeightBox = new System.Windows.Forms.NumericUpDown();
			this.drawWidthBox = new System.Windows.Forms.NumericUpDown();
			this.versionLabel = new System.Windows.Forms.Label();
			this.versionTextBox = new System.Windows.Forms.TextBox();
			this.UserConfigurableLabel = new System.Windows.Forms.Label();
			this.windowModeCheckBox = new System.Windows.Forms.CheckBox();
			this.userConfigurableCheckedListBox = new System.Windows.Forms.CheckedListBox();
			this.OKButton = new System.Windows.Forms.Button();
			this.cancelButton = new System.Windows.Forms.Button();
			((System.ComponentModel.ISupportInitialize)(this.internalWidthBox)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.internalHeightBox)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.drawHeightBox)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.drawWidthBox)).BeginInit();
			this.SuspendLayout();
			// 
			// titleLabel
			// 
			this.titleLabel.AutoSize = true;
			this.titleLabel.Location = new System.Drawing.Point(13, 13);
			this.titleLabel.Name = "titleLabel";
			this.titleLabel.Size = new System.Drawing.Size(40, 12);
			this.titleLabel.TabIndex = 0;
			this.titleLabel.Text = "タイトル";
			// 
			// titleTextBox
			// 
			this.titleTextBox.Location = new System.Drawing.Point(15, 29);
			this.titleTextBox.Name = "titleTextBox";
			this.titleTextBox.Size = new System.Drawing.Size(257, 19);
			this.titleTextBox.TabIndex = 1;
			this.titleTextBox.TextChanged += new System.EventHandler(this.titleTextBox_TextChanged);
			// 
			// internalSizeLabel
			// 
			this.internalSizeLabel.AutoSize = true;
			this.internalSizeLabel.Location = new System.Drawing.Point(15, 93);
			this.internalSizeLabel.Name = "internalSizeLabel";
			this.internalSizeLabel.Size = new System.Drawing.Size(82, 12);
			this.internalSizeLabel.TabIndex = 2;
			this.internalSizeLabel.Text = "内部画面サイズ";
			// 
			// internalWidthBox
			// 
			this.internalWidthBox.Location = new System.Drawing.Point(15, 108);
			this.internalWidthBox.Name = "internalWidthBox";
			this.internalWidthBox.Size = new System.Drawing.Size(131, 19);
			this.internalWidthBox.TabIndex = 3;
			this.internalWidthBox.ValueChanged += new System.EventHandler(this.internalWidthBox_ValueChanged);
			// 
			// internalHeightBox
			// 
			this.internalHeightBox.Location = new System.Drawing.Point(152, 108);
			this.internalHeightBox.Name = "internalHeightBox";
			this.internalHeightBox.Size = new System.Drawing.Size(120, 19);
			this.internalHeightBox.TabIndex = 4;
			this.internalHeightBox.ValueChanged += new System.EventHandler(this.internalHeightBox_ValueChanged);
			// 
			// drawSizeLabel
			// 
			this.drawSizeLabel.AutoSize = true;
			this.drawSizeLabel.Location = new System.Drawing.Point(15, 130);
			this.drawSizeLabel.Name = "drawSizeLabel";
			this.drawSizeLabel.Size = new System.Drawing.Size(58, 12);
			this.drawSizeLabel.TabIndex = 5;
			this.drawSizeLabel.Text = "描画サイズ";
			// 
			// drawHeightBox
			// 
			this.drawHeightBox.Location = new System.Drawing.Point(152, 145);
			this.drawHeightBox.Name = "drawHeightBox";
			this.drawHeightBox.Size = new System.Drawing.Size(120, 19);
			this.drawHeightBox.TabIndex = 7;
			this.drawHeightBox.ValueChanged += new System.EventHandler(this.drawHeightBox_ValueChanged);
			// 
			// drawWidthBox
			// 
			this.drawWidthBox.Location = new System.Drawing.Point(15, 145);
			this.drawWidthBox.Name = "drawWidthBox";
			this.drawWidthBox.Size = new System.Drawing.Size(131, 19);
			this.drawWidthBox.TabIndex = 6;
			this.drawWidthBox.ValueChanged += new System.EventHandler(this.drawWidthBox_ValueChanged);
			// 
			// versionLabel
			// 
			this.versionLabel.AutoSize = true;
			this.versionLabel.Location = new System.Drawing.Point(15, 55);
			this.versionLabel.Name = "versionLabel";
			this.versionLabel.Size = new System.Drawing.Size(50, 12);
			this.versionLabel.TabIndex = 8;
			this.versionLabel.Text = "バージョン";
			// 
			// versionTextBox
			// 
			this.versionTextBox.Location = new System.Drawing.Point(17, 71);
			this.versionTextBox.Name = "versionTextBox";
			this.versionTextBox.Size = new System.Drawing.Size(100, 19);
			this.versionTextBox.TabIndex = 9;
			this.versionTextBox.TextChanged += new System.EventHandler(this.versionTextBox_TextChanged);
			// 
			// UserConfigurableLabel
			// 
			this.UserConfigurableLabel.AutoSize = true;
			this.UserConfigurableLabel.Location = new System.Drawing.Point(13, 190);
			this.UserConfigurableLabel.Name = "UserConfigurableLabel";
			this.UserConfigurableLabel.Size = new System.Drawing.Size(137, 12);
			this.UserConfigurableLabel.TabIndex = 10;
			this.UserConfigurableLabel.Text = "ユーザー設定可能パラメータ";
			// 
			// windowModeCheckBox
			// 
			this.windowModeCheckBox.AutoSize = true;
			this.windowModeCheckBox.Location = new System.Drawing.Point(15, 171);
			this.windowModeCheckBox.Name = "windowModeCheckBox";
			this.windowModeCheckBox.Size = new System.Drawing.Size(97, 16);
			this.windowModeCheckBox.TabIndex = 12;
			this.windowModeCheckBox.Text = "ウインドウモード";
			this.windowModeCheckBox.UseVisualStyleBackColor = true;
			this.windowModeCheckBox.CheckedChanged += new System.EventHandler(this.windowModeCheckBox_CheckedChanged);
			// 
			// userConfigurableCheckedListBox
			// 
			this.userConfigurableCheckedListBox.FormattingEnabled = true;
			this.userConfigurableCheckedListBox.Items.AddRange(new object[] {
            "描画サイズ",
            "ウインドウモード"});
			this.userConfigurableCheckedListBox.Location = new System.Drawing.Point(13, 206);
			this.userConfigurableCheckedListBox.Name = "userConfigurableCheckedListBox";
			this.userConfigurableCheckedListBox.Size = new System.Drawing.Size(259, 46);
			this.userConfigurableCheckedListBox.TabIndex = 13;
			// 
			// OKButton
			// 
			this.OKButton.Location = new System.Drawing.Point(116, 258);
			this.OKButton.Name = "OKButton";
			this.OKButton.Size = new System.Drawing.Size(75, 23);
			this.OKButton.TabIndex = 14;
			this.OKButton.Text = "オッケー";
			this.OKButton.UseVisualStyleBackColor = true;
			// 
			// cancelButton
			// 
			this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.cancelButton.Location = new System.Drawing.Point(197, 258);
			this.cancelButton.Name = "cancelButton";
			this.cancelButton.Size = new System.Drawing.Size(75, 23);
			this.cancelButton.TabIndex = 15;
			this.cancelButton.Text = "キャンセル";
			this.cancelButton.UseVisualStyleBackColor = true;
			// 
			// GameSettingForm
			// 
			this.AcceptButton = this.OKButton;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.cancelButton;
			this.ClientSize = new System.Drawing.Size(284, 288);
			this.Controls.Add(this.cancelButton);
			this.Controls.Add(this.OKButton);
			this.Controls.Add(this.userConfigurableCheckedListBox);
			this.Controls.Add(this.windowModeCheckBox);
			this.Controls.Add(this.UserConfigurableLabel);
			this.Controls.Add(this.versionTextBox);
			this.Controls.Add(this.versionLabel);
			this.Controls.Add(this.drawHeightBox);
			this.Controls.Add(this.drawWidthBox);
			this.Controls.Add(this.drawSizeLabel);
			this.Controls.Add(this.internalHeightBox);
			this.Controls.Add(this.internalWidthBox);
			this.Controls.Add(this.internalSizeLabel);
			this.Controls.Add(this.titleTextBox);
			this.Controls.Add(this.titleLabel);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "GameSettingForm";
			this.ShowInTaskbar = false;
			this.Text = "ゲーム情報設定";
			((System.ComponentModel.ISupportInitialize)(this.internalWidthBox)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.internalHeightBox)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.drawHeightBox)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.drawWidthBox)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label titleLabel;
		private System.Windows.Forms.TextBox titleTextBox;
		private System.Windows.Forms.Label internalSizeLabel;
		private System.Windows.Forms.NumericUpDown internalWidthBox;
		private System.Windows.Forms.NumericUpDown internalHeightBox;
		private System.Windows.Forms.Label drawSizeLabel;
		private System.Windows.Forms.NumericUpDown drawHeightBox;
		private System.Windows.Forms.NumericUpDown drawWidthBox;
		private System.Windows.Forms.Label versionLabel;
		private System.Windows.Forms.TextBox versionTextBox;
		private System.Windows.Forms.Label UserConfigurableLabel;
		private System.Windows.Forms.CheckBox windowModeCheckBox;
		private System.Windows.Forms.CheckedListBox userConfigurableCheckedListBox;
		private System.Windows.Forms.Button OKButton;
		private System.Windows.Forms.Button cancelButton;
	}
}
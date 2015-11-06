namespace PlanetaEditor
{
	partial class MainForm
	{
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		/// <param name="disposing">マネージ リソースを破棄する場合は true を指定し、その他の場合は false を指定します。</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows フォーム デザイナーで生成されたコード

		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.出力OToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.リソースアーカイブとシステム設定ファイルの出力ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.設定ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.ゲームToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.ユーザー設定INIファイルのテンプレートを出力ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.ファイルToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.プロジェクトを保存ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.プロジェクトを読み込みToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
			this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
			this.プロジェクト設定ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.menuStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ファイルToolStripMenuItem,
            this.出力OToolStripMenuItem,
            this.設定ToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(284, 24);
			this.menuStrip1.TabIndex = 0;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// 出力OToolStripMenuItem
			// 
			this.出力OToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.リソースアーカイブとシステム設定ファイルの出力ToolStripMenuItem,
            this.ユーザー設定INIファイルのテンプレートを出力ToolStripMenuItem});
			this.出力OToolStripMenuItem.Name = "出力OToolStripMenuItem";
			this.出力OToolStripMenuItem.Size = new System.Drawing.Size(60, 20);
			this.出力OToolStripMenuItem.Text = "出力(&O)";
			// 
			// リソースアーカイブとシステム設定ファイルの出力ToolStripMenuItem
			// 
			this.リソースアーカイブとシステム設定ファイルの出力ToolStripMenuItem.Name = "リソースアーカイブとシステム設定ファイルの出力ToolStripMenuItem";
			this.リソースアーカイブとシステム設定ファイルの出力ToolStripMenuItem.Size = new System.Drawing.Size(287, 22);
			this.リソースアーカイブとシステム設定ファイルの出力ToolStripMenuItem.Text = "リソースアーカイブとシステム設定ファイルを出力";
			// 
			// 設定ToolStripMenuItem
			// 
			this.設定ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ゲームToolStripMenuItem,
            this.プロジェクト設定ToolStripMenuItem});
			this.設定ToolStripMenuItem.Name = "設定ToolStripMenuItem";
			this.設定ToolStripMenuItem.Size = new System.Drawing.Size(57, 20);
			this.設定ToolStripMenuItem.Text = "設定(&S)";
			// 
			// ゲームToolStripMenuItem
			// 
			this.ゲームToolStripMenuItem.Name = "ゲームToolStripMenuItem";
			this.ゲームToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.ゲームToolStripMenuItem.Text = "ゲーム情報設定";
			this.ゲームToolStripMenuItem.Click += new System.EventHandler(this.ゲームToolStripMenuItem_Click);
			// 
			// ユーザー設定INIファイルのテンプレートを出力ToolStripMenuItem
			// 
			this.ユーザー設定INIファイルのテンプレートを出力ToolStripMenuItem.Name = "ユーザー設定INIファイルのテンプレートを出力ToolStripMenuItem";
			this.ユーザー設定INIファイルのテンプレートを出力ToolStripMenuItem.Size = new System.Drawing.Size(287, 22);
			this.ユーザー設定INIファイルのテンプレートを出力ToolStripMenuItem.Text = "ユーザー設定INIファイルのテンプレートを出力";
			// 
			// ファイルToolStripMenuItem
			// 
			this.ファイルToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.プロジェクトを保存ToolStripMenuItem,
            this.プロジェクトを読み込みToolStripMenuItem});
			this.ファイルToolStripMenuItem.Name = "ファイルToolStripMenuItem";
			this.ファイルToolStripMenuItem.Size = new System.Drawing.Size(66, 20);
			this.ファイルToolStripMenuItem.Text = "ファイル(&F)";
			// 
			// プロジェクトを保存ToolStripMenuItem
			// 
			this.プロジェクトを保存ToolStripMenuItem.Name = "プロジェクトを保存ToolStripMenuItem";
			this.プロジェクトを保存ToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
			this.プロジェクトを保存ToolStripMenuItem.Size = new System.Drawing.Size(201, 22);
			this.プロジェクトを保存ToolStripMenuItem.Text = "プロジェクトを保存";
			this.プロジェクトを保存ToolStripMenuItem.Click += new System.EventHandler(this.プロジェクトを保存ToolStripMenuItem_Click);
			// 
			// プロジェクトを読み込みToolStripMenuItem
			// 
			this.プロジェクトを読み込みToolStripMenuItem.Name = "プロジェクトを読み込みToolStripMenuItem";
			this.プロジェクトを読み込みToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
			this.プロジェクトを読み込みToolStripMenuItem.Size = new System.Drawing.Size(201, 22);
			this.プロジェクトを読み込みToolStripMenuItem.Text = "プロジェクトを読込";
			this.プロジェクトを読み込みToolStripMenuItem.Click += new System.EventHandler(this.プロジェクトを読み込みToolStripMenuItem_Click);
			// 
			// openFileDialog1
			// 
			this.openFileDialog1.FileName = "openFileDialog1";
			// 
			// プロジェクト設定ToolStripMenuItem
			// 
			this.プロジェクト設定ToolStripMenuItem.Name = "プロジェクト設定ToolStripMenuItem";
			this.プロジェクト設定ToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.プロジェクト設定ToolStripMenuItem.Text = "プロジェクト設定";
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(284, 261);
			this.Controls.Add(this.menuStrip1);
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "MainForm";
			this.Text = "PlanetaEditor";
			this.Load += new System.EventHandler(this.MainForm_Load);
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem 出力OToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem リソースアーカイブとシステム設定ファイルの出力ToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem 設定ToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem ゲームToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem ユーザー設定INIファイルのテンプレートを出力ToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem ファイルToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem プロジェクトを保存ToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem プロジェクトを読み込みToolStripMenuItem;
		private System.Windows.Forms.OpenFileDialog openFileDialog1;
		private System.Windows.Forms.SaveFileDialog saveFileDialog1;
		private System.Windows.Forms.ToolStripMenuItem プロジェクト設定ToolStripMenuItem;
	}
}


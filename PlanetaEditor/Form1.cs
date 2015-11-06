using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PlanetaEditor
{
	public partial class MainForm : Form
	{
		private string project_path_ = "";
		private ProjectData project_data_ = new ProjectData();

		private GameSettingForm game_setting_form_ = new GameSettingForm();

		public MainForm()
		{
			InitializeComponent();
		}

		private void MainForm_Load(object sender, EventArgs e)
		{
		
		}

		private void ゲームToolStripMenuItem_Click(object sender, EventArgs e)
		{
			game_setting_form_.game_information = project_data_.game_information;
			var result = game_setting_form_.ShowDialog();
			if (result == DialogResult.OK) { project_data_.game_information = game_setting_form_.game_information; }
		}

		private void プロジェクトを保存ToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (project_path_ == "") {
				if (saveFileDialog1.ShowDialog() == DialogResult.OK)
				{
					project_path_ = saveFileDialog1.FileName;
				}
				else { return; }
			}
			System.Xml.Serialization.XmlSerializer serializer = new System.Xml.Serialization.XmlSerializer(typeof(ProjectData));
			System.IO.StreamWriter sw = new System.IO.StreamWriter(project_path_, false, new System.Text.UTF8Encoding(false));
			serializer.Serialize(sw, project_data_);
		}

		private void プロジェクトを読み込みToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (openFileDialog1.ShowDialog() == DialogResult.OK)
			{
				System.Xml.Serialization.XmlSerializer serializer = new System.Xml.Serialization.XmlSerializer(typeof(ProjectData));
				System.IO.StreamReader sr = new System.IO.StreamReader(openFileDialog1.FileName, new System.Text.UTF8Encoding(false));
				project_data_ = (ProjectData)serializer.Deserialize(sr);
				project_path_ = openFileDialog1.FileName;
			}
		}
	}
}
